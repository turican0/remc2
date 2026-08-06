
// support for gamepads and joysticks
//
// adds configurable game specific controls to be bound to axes, buttons and hats present on a controller
// see README_controllers for sample configurations
// this implementation basically simulates mouse moves and key presses
//
// tested with Logitech Attack 3, Logitech Extreme PRO 3D and XBOX Elite Series 2
//
// author: Petre Rodan, 2023

#include <cstdint>
#include <stdio.h>

#include <SDL2/SDL.h>"

#include "engine/EventDispatcher.h"
#include "engine/sub_main_mouse.h"
#include "engine/read_config.h"
#include "utilities/Maths.h"
#include "port_sdl_vga_mouse.h"
#include "port_sdl_joystick.h"

SDL_Joystick *m_gameController = NULL;
SDL_Haptic *m_haptic = NULL;

#define              JOY_MIN_X  0   ///< minimum bounds for mouse position value for x axis
#define              JOY_MIN_Y  0   ///< minimum bounds for mouse position value for y axis
#define            JOY_NAV_INC  4   ///< coefficient that defines how many pixels to skip while navigating a menu

#define       GP_FLIGHT_UPDATE  0x1 ///< bitflag set if the axes controlling flight are out of the stick dead zone
#define          GP_NAV_UPDATE  0x2 ///< bitflag set if the axes controlling navigation are out of the stick dead zone
#define          GP_MOV_UPDATE  0x4 ///< bitflag set if the axes controlling movement are out of the stick dead zone

#define GP_MAX_KEY_RELEASE_ANN  4   ///< maximum number of key release announcements

///< structure that defines the current gamepad state ad it's simulated output
struct gamepad_state {
	int32_t x;                      ///< currently simulated x mouse position
	int32_t y;                      ///< currently simulated y mouse position
	int32_t rest_x;                 ///< mouse x position to use when the stick is in the rest position (or dead zone)
	int32_t rest_y;                 ///< mouse y position to use when the stick is in the rest position (or dead zone)
	int32_t max_x;                  ///< maximum bounds for mouse position on the x axis based on current scene (sometimes we get garbage values here from upstream)
	int32_t max_y;                  ///< maximum bounds for mouse position on the y axis based on current scene (sometimes we get garbage values here from upstream)
	uint8_t dead_zone_announced;    ///< slow infinite spin mitigation when joystick is in the resting position while in the flying window
	uint8_t mov_key_announced;      ///< counter of consecutive setPress(false, KEY) requests 
	uint8_t initialized;            ///< gamepad was initialized and it's ready to be queried
	Scene scene_id;                 ///< current scene displayed by the recode. one of SCENE_PREAMBLE_MENU, SCENE_FLIGHT, SCENE_FLIGHT_MENU
	uint8_t nav_mode;               ///< true during menu navigation
	uint8_t last_trig_fire_R;       ///< detection of movement based on the right trigger button's axis value
	uint8_t last_trig_fire_L;       ///< detection of movement based on the left trigger button's axis value
};
typedef struct gamepad_state gamepad_state_t;

///< structure that defines the mouse pointer position
struct pointer_sys {
	int16_t x;
	int16_t y;
};
typedef struct pointer_sys pointer_sys_t;

///< hat position data
struct vec1d {
	int16_t x;                      ///< bitwise flags. SDL_HAT_UP | SDL_HAT_DOWN | SDL_HAT_LEFT | SDL_HAT_RIGHT or 0
	uint8_t x_conf;                 ///< bitwise flags. cound be 0 or a combination of GAMEPAD_AXIS_INVERTED | GAMEPAD_ITEM_ENABLED
};
typedef struct vec1d vec1d_t;

///< axis position data
struct vec2d {
	int16_t x;                      ///< x axis value [-32767..32768]
	int16_t y;                      ///< y axis value [-32767..32768]
	uint8_t x_conf;                 ///< bitwise flags. cound be 0 or a combination of GAMEPAD_AXIS_INVERTED | GAMEPAD_ITEM_ENABLED
	uint8_t y_conf;                 ///< bitwise flags. cound be 0 or a combination of GAMEPAD_AXIS_INVERTED | GAMEPAD_ITEM_ENABLED
};
typedef struct vec2d vec2d_t;

///< force-feedback subsystem state
struct haptic_state {
	uint8_t enabled;                ///< if subsystem is currently enabled
	uint8_t initialized;            ///< subsystem initialized and ready
	uint8_t rumble;                 ///< rumble is initialized
	uint8_t rumble_trig;            ///< rumble trigger is present
	uint32_t cap;                   ///< controller capabilities
	int quake;                      ///< quake effect identifier
};
typedef struct haptic_state haptic_state_t;

gamepad_state_t gps = {};
haptic_state_t hs = {};

int8_t haptic_load_effects(void);

/// \brief initialization of the SDL joystick subsystem
void gamepad_sdl_init(void)
{
	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
		Logger->error("SDL joystick could not be initialized! SDL_Error: {}", SDL_GetError());
	} else {
		m_gameController = SDL_JoystickOpen(0);
		if(m_gameController == NULL) {
			Logger->debug("joystick/gamepad not detected. SDL Error: {}", SDL_GetError() );
		} else {
			Logger->info("Found '{}' joystick", SDL_JoystickName(m_gameController) );
			if (SDL_JoystickEventState(SDL_ENABLE) != 1) {
				Logger->error("unable to initialize joystick/gamepad events. SDL Error: {}", SDL_GetError() );
			} else {
				gps.initialized = 1;
				std::function<void(Scene)> callBack = set_scene;
				EventDispatcher::I->RegisterEvent(new Event<Scene>(EventType::E_SCENE_CHANGE, callBack));

				std::function<void(uint32_t, uint32_t)> resCallBack = OnResolutionChanged;
				EventDispatcher::I->RegisterEvent(new Event<uint32_t, uint32_t>(EventType::E_RESOLUTION_CHANGE, resCallBack));
			}
			if (gpc.haptic_enabled && (SDL_InitSubSystem(SDL_INIT_HAPTIC) == 0) && SDL_JoystickIsHaptic(m_gameController)) {
				m_haptic = SDL_HapticOpenFromJoystick(m_gameController);
				if (m_haptic == NULL) {
					Logger->info("unable to init force feedback. SDL Error: {}", SDL_GetError() );
				} else {
					hs.cap = SDL_HapticQuery(m_haptic);
					hs.rumble = SDL_HapticRumbleSupported(m_haptic);
#ifdef _MSC_VER
					//hs.rumble_trig = SDL_JoystickHasRumbleTriggers(m_gameController);
#endif
					hs.initialized = 1;
					hs.enabled = 1;
					haptic_load_effects();
					if (hs.rumble) {
						if (SDL_HapticRumbleInit(m_haptic) < 0) {
							hs.rumble = 0;
						}
					}
					if (hs.cap & SDL_HAPTIC_GAIN) {
						SDL_HapticSetGain(m_haptic, gpc.haptic_gain_max);
					}
					Logger->info("found haptic support (cap {}), effect cnt {}, rumble {}, rumble_trig {}", hs.cap, SDL_HapticNumEffects(m_haptic), hs.rumble, hs.rumble_trig);
				}
			}
		}
	}
}

/// \brief cleanup of the SDL joystick subsystem, to be used only on program exit
void gamepad_sdl_close(void)
{
	SDL_JoystickClose(m_gameController);
	m_gameController = NULL;
	if (m_haptic) {
		SDL_HapticClose(m_haptic);
		m_haptic = NULL;
	}
}

/// \brief initialize gamepad maximal coordinate values, default operating mode, etc
/// \param gameResWidth maximum x value
/// \param gameResHeight maximum y value
void gamepad_init(const int gameResWidth, const int gameResHeight)
{
	gps.max_x = gameResWidth;
	gps.max_y = gameResHeight;
	joystick_set_env(gps.max_x >> 1, gps.max_y >> 1);
	EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::PREAMBLE_MENU);
}

void AdjustStickCoords(vec2d_t* stick, std::vector<Maths::Zone>* zonesX, std::vector<Maths::Zone>* zonesY)
{
	if (stick->x >= 0)
	{
		stick->x = ((int16_t)Maths::CurveCoords(stick->x, stick->x, *zonesX));
	}
	else
	{
		stick->x = -((int16_t)Maths::CurveCoords(-stick->x, -stick->x, *zonesX));
	}

	if (stick->y >= 0)
	{
		stick->y = ((int16_t)Maths::CurveCoords(stick->y, stick->y, *zonesY));
	}
	else
	{
		stick->y = -((int16_t)Maths::CurveCoords(-stick->y, -stick->y, *zonesY));
	}
}

/// \brief flight support via conversion from stick coordinates to pointer coordinates
/// \param  stick input axis values
/// \param  point output simulated mouse pointer values
/// \return 0 if stick is in the dead zone or GP_FLIGHT_UPDATE otherwise
uint16_t gamepad_axis_flight_conv(vec2d_t *stick, pointer_sys_t *point)
{
	uint16_t ret = 0;
	AdjustStickCoords(stick, &gpc.axis_yaw_sensitivity, &gpc.axis_pitch_sensitivity);
	int16_t axis_yaw = stick->x;
	int16_t axis_pitch = stick->y;

	if ((axis_yaw < gpc.axis_yaw_dead_zone) && (axis_yaw > -gpc.axis_yaw_dead_zone)) {
		point->x = gps.rest_x;
	} else {
		// use two different linear interpolation equations since the
		// resting coordinate is not always the center of the display
		if (axis_yaw > 0) {
			point->x = (((gps.max_x - gps.rest_x) * axis_yaw) >> 15) + gps.rest_x;
		} else {
			point->x = ((gps.rest_x * axis_yaw) >> 15) + gps.rest_x;
		}
		ret = GP_FLIGHT_UPDATE;
	}

	if ((axis_pitch < gpc.axis_pitch_dead_zone) && (axis_pitch > -gpc.axis_pitch_dead_zone)) {
		point->y = gps.rest_y;
	} else {
		// use two different linear interpolation equations since the
		// resting coordinate is not always the center of the display
		if (axis_pitch > 0) {
			point->y = (((gps.max_y - gps.rest_y) * axis_pitch) >> 15) + gps.rest_y;
		} else {
			point->y = ((gps.rest_y * axis_pitch) >> 15) + gps.rest_y;
		}
		ret = GP_FLIGHT_UPDATE;
	}

	return ret;
}

/// \brief menu navigation support via conversion from stick coordinates to pointer coordinates
/// \param  stick input axis values
/// \param  point output simulated mouse pointer values
/// \return 0 if stick is in the dead zone or GP_NAV_UPDATE otherwise
uint16_t gamepad_axis_nav_conv(const vec2d_t *stick, pointer_sys_t *point)
{
	uint16_t ret = 0;
	int16_t axis_nav_ns = stick->x;
	int16_t axis_nav_ew = stick->y;

	if ((axis_nav_ns < gpc.axis_long_nav_dead_zone) && (axis_nav_ns > -gpc.axis_long_nav_dead_zone)) {
		// point->x remains unchanged
	} else {
		point->y += JOY_NAV_INC * (axis_nav_ns >> 13);
		ret = GP_NAV_UPDATE;
	}

	if ((axis_nav_ew < gpc.axis_trans_nav_dead_zone) && (axis_nav_ew > -gpc.axis_trans_nav_dead_zone)) {
		// point->y remains unchanged
	} else {
		point->x += JOY_NAV_INC * (axis_nav_ew >> 13);
		ret = GP_NAV_UPDATE;
	}

	return ret;
}

/// \brief menu navigation support via conversion from hat coordinates to pointer coordinates
/// \param  hat input value
/// \param  point output simulated mouse pointer values
/// \return 0 if stick is resting or GP_NAV_UPDATE otherwise
uint16_t gamepad_hat_nav_conv(const vec1d_t *hat, pointer_sys_t *point)
{
	uint16_t ret = 0;
	int16_t inv = 1;

	// dir can be a bitwise OR of two adjacent directions
	// so don't use 'else if' or 'switch'.

	if (hat->x_conf & GAMEPAD_AXIS_INVERTED) {
		inv = -1;
	}

	if (hat->x & SDL_HAT_UP) {
		point->y += inv * JOY_NAV_INC * 2;
		ret = GP_NAV_UPDATE;
	}

	if (hat->x & SDL_HAT_DOWN) {
		point->y -= inv * JOY_NAV_INC * 2;
		ret = GP_NAV_UPDATE;
	}

	if (hat->x & SDL_HAT_RIGHT) {
		point->x += JOY_NAV_INC * 2;
		ret = GP_NAV_UPDATE;
	}

	if (hat->x & SDL_HAT_LEFT) {
		point->x -= JOY_NAV_INC * 2;
		ret = GP_NAV_UPDATE;
	}

	return ret;
}

/// \brief longitudinal and transversal hat movement converted to hardcoded keyboard keypresses
/// \param  hat input value
void gamepad_hat_mov_conv(const vec1d_t *hat)
{
	uint16_t ret = 0;

	if (hat->x & SDL_HAT_UP) {
		SetPress(false, inputMapping.Backwards);
		SetPress(true, inputMapping.Forward);
		ret = GP_MOV_UPDATE;
	}

	if (hat->x & SDL_HAT_DOWN) {
		SetPress(false, inputMapping.Forward);
		SetPress(true, inputMapping.Backwards);
		ret = GP_MOV_UPDATE;
	}

	if (hat->x & SDL_HAT_RIGHT) {
		SetPress(false, inputMapping.Left);
		SetPress(true, inputMapping.Right);
		ret = GP_MOV_UPDATE;
	}

	if (hat->x & SDL_HAT_LEFT) {
		SetPress(false, inputMapping.Right);
		SetPress(true, inputMapping.Left);
		ret = GP_MOV_UPDATE;
	}

	if (hat->x == 0) {
		if (gps.mov_key_announced < GP_MAX_KEY_RELEASE_ANN) {
			SetPress(false, inputMapping.Forward);
			SetPress(false, inputMapping.Backwards);
			SetPress(false, inputMapping.Right);
			SetPress(false, inputMapping.Left);
			gps.mov_key_announced++;
		}
	}

	if (ret) {
		gps.mov_key_announced = 0;
	}
}

/// \brief longitudinal and transversal movement via conversion from stick coordinates to key presses
/// \param  stick input axis values
void gamepad_axis_mov_conv(vec2d_t *stick)
{
	uint16_t ret = 0;
	int16_t axis_long_inv = 1;
	int16_t axis_long = stick->x;
	int16_t axis_trans = stick->y;

	if (stick->x_conf & GAMEPAD_AXIS_INVERTED) {
		axis_long_inv = -1;
	}

	if ((axis_long < gpc.axis_long_dead_zone) && (axis_long > -gpc.axis_long_dead_zone)) {
		// player seems to always have some inertia, so the following wont't actually stop
		// longitudinal movement
		if (gps.mov_key_announced < GP_MAX_KEY_RELEASE_ANN) {
			SetPress(false, inputMapping.Forward);
			SetPress(false, inputMapping.Backwards);
		}
	} else {
		if (axis_long * axis_long_inv > 0) {
			SetPress(false, inputMapping.Backwards);
			SetPress(true, inputMapping.Forward);
		} else {
			SetPress(false, inputMapping.Forward);
			SetPress(true, inputMapping.Backwards);
		}
		ret = GP_MOV_UPDATE;
	}

	if ((axis_trans < gpc.axis_trans_dead_zone) && (axis_trans > -gpc.axis_trans_dead_zone)) {
		if (gps.mov_key_announced < GP_MAX_KEY_RELEASE_ANN) {
			SetPress(false, inputMapping.Right);
			SetPress(false, inputMapping.Left);
		}
	} else {
		if (axis_trans > 0) {
			SetPress(false, inputMapping.Left);
			SetPress(true, inputMapping.Right);
		} else {
			SetPress(false, inputMapping.Right);
			SetPress(true, inputMapping.Left);
		}
		ret = GP_MOV_UPDATE;
	}

	if (!ret) {
		if (gps.mov_key_announced < GP_MAX_KEY_RELEASE_ANN) {
			gps.mov_key_announced++;
		}
	} else {
		gps.mov_key_announced = 0;
	}
}

/// \brief button-like action via conversion from axis coordinates to a boolean (for xbox trigger buttons)
/// \param  input axis value
/// \return 0 is button is inside the dead zone, 1 otherwise
void gamepad_axis_bool_conv(const int16_t input, bool *ret)
{
	if (input > -32767 + gpc.trigger_dead_zone) {
		*ret = 1;
	} else {
		*ret = 0;
	}
}

/// \brief emulate a mouse based on data provided by a gamepad or joystick
/// \param gpe  gamepad_event_t event structure populated thru SDL_PollEvent()
void gamepad_event_mgr(gamepad_event_t *gpe)
{
	uint16_t button_state = 0;
	uint8_t flight_mode = 1; // are we doing flight or menu navigation
	uint16_t conv_state = 0;
	pointer_sys_t flight;
	pointer_sys_t nav;
	vec2d stick;
	vec1d hat;
	bool trig_fire_R = 0, trig_fire_L = 0;

	// decide if we are handling flight mode or menu navigation mode
	// default mode, based on scene
	if (gps.nav_mode) {
		flight_mode = 0;
	}

	// overwrite scene-based mode based on what button is pressed
	if (SDL_JoystickGetButton(m_gameController, gpc.button_spell - 1)) {
		flight_mode = 0;
	}

	if (SDL_GetModState() & KMOD_CTRL) {
		flight_mode = 0;
	}

	stick.x = gpe->axis_yaw;
	stick.y = gpe->axis_pitch;
	flight.x = gps.x;
	flight.y = gps.y;
	// flight yaw/pitch is always done via two axes
	conv_state |= gamepad_axis_flight_conv(&stick, &flight);

	// menu navigation
	nav.x = gps.x;
	nav.y = gps.y;

	if (gpc.hat_nav_conf & GAMEPAD_ITEM_ENABLED) {
		// menu navigation is done via a hat control
		hat.x = gpe->hat_nav;
		hat.x_conf = gpc.hat_nav_conf;
		conv_state |= gamepad_hat_nav_conv(&hat, &nav);
	} else if (gpc.axis_nav_ns_conf & gpc.axis_nav_ew_conf & GAMEPAD_ITEM_ENABLED) {
		// menu navigation is done via two axes
		stick.x = gpe->axis_nav_ns;
		stick.y = gpe->axis_nav_ew;
		conv_state |= gamepad_axis_nav_conv(&stick, &nav);
	}

	if (flight_mode) {
		gps.x = flight.x;
		gps.y = flight.y;
	} else {
		gps.x = nav.x;
		gps.y = nav.y;
	}

	if (gps.x < JOY_MIN_X) {
		gps.x = JOY_MIN_X;
	} else if (gps.x > gps.max_x) {
		gps.x = gps.max_x;
	}

	if (gps.y < JOY_MIN_Y) {
		gps.y = JOY_MIN_Y;
	} else if (gps.y > gps.max_y) {
		gps.y = gps.max_y;
	}

	// longitudinal/transversal movement 
	// (aka forward/back/strafe right/strafe left)
	if (gpc.hat_mov_conf & GAMEPAD_ITEM_ENABLED) {
		// if movement is done via a hat
		hat.x = gpe->hat_mov;
		hat.x_conf = gpc.hat_mov_conf;
		gamepad_hat_mov_conv(&hat);
	}

	if (((gpc.axis_long_conf & GAMEPAD_ITEM_ENABLED) || (gpc.axis_trans_conf & GAMEPAD_ITEM_ENABLED)) && (gps.scene_id != Scene::SPELL_MENU)) {
		// if movement is done via two axes
		stick.x = gpe->axis_long;
		stick.x_conf = gpc.axis_long_conf;
		stick.y = gpe->axis_trans;
		gamepad_axis_mov_conv(&stick);
	}

	// trigger commands
	if (gpc.axis_fire_R_conf & GAMEPAD_ITEM_ENABLED) {
		gamepad_axis_bool_conv(gpe->axis_fire_R, &trig_fire_R);
		if (trig_fire_R != gps.last_trig_fire_R) {
			if (trig_fire_R) {
				button_state |= 0x8;
			} else {
				button_state |= 0x10;
			}
			gps.last_trig_fire_R = trig_fire_R;
		}
	}

	if (gpc.axis_fire_L_conf & GAMEPAD_ITEM_ENABLED) {
		gamepad_axis_bool_conv(gpe->axis_fire_L, &trig_fire_L);
		if (trig_fire_L != gps.last_trig_fire_L) {
			if (trig_fire_L) {
				button_state |= 0x2;
			} else {
				button_state |= 0x4;
			}
			gps.last_trig_fire_L = trig_fire_L;
		}
	}

	// temporary place for testing haptic effects
	if (gpe->btn_pressed) {
		if (gpe->btn_pressed & (1 << gpc.button_fire_R)) {
			button_state |= 0x8;
		}
		if (gpe->btn_pressed & (1 << gpc.button_fire_L)) {
			button_state |= 0x2;
			//haptic_rumble_triggers_effect(0, 32000, 1000);
		}
		if (gps.scene_id != Scene::FLIGHT && (gpe->btn_pressed & (1 << gpc.button_menu_select))) {
			button_state |= 0x2;
		}
		if (gps.scene_id != Scene::FLIGHT_MENU && (gpe->btn_pressed & (1 << gpc.button_spell))) {
			SetPress(true, inputMapping.SpellMenu);
			//haptic_rumble_triggers_effect(32000, 0, 1000);
		}
		if (gpe->btn_pressed & (1 << gpc.button_minimap)) {
			SetPress(true, inputMapping.Map);
			//haptic_run_effect(hs.quake);
			//haptic_rumble_effect(0.5, 2000);
		}
		if (gpe->btn_pressed & (1 << gpc.button_pause_menu)) {
			SetPress(true, SDL_SCANCODE_P);
		}
		if (gpe->btn_pressed & (1 << gpc.button_fwd)) {
			SetPress(true, inputMapping.Forward);
		}
		if (gpe->btn_pressed & (1 << gpc.button_back)) {
			SetPress(true, inputMapping.Backwards);
		}
		if (gpe->btn_pressed & (1 << gpc.button_esc)) {
			SetPress(true, SDL_SCANCODE_ESCAPE);
		}
		if (gps.scene_id == Scene::DEAD) {
			SetPress(true, SDL_SCANCODE_SPACE);
		}
	}

	if (gpe->btn_released) {
		if (gpe->btn_released & (1 << gpc.button_fire_R)) {
			button_state |= 0x10;
		}
		if (gpe->btn_released & (1 << gpc.button_fire_L)) {
			button_state |= 0x4;
		}
		if (gps.scene_id != Scene::FLIGHT && (gpe->btn_released & (1 << gpc.button_menu_select))) {
			button_state |= 0x4;
		}
		if (gps.scene_id != Scene::FLIGHT_MENU && (gpe->btn_released & (1 << gpc.button_spell))) {
			SetPress(false, inputMapping.SpellMenu);
		}
		if (gpe->btn_released & (1 << gpc.button_minimap)) {
			SetPress(false, inputMapping.Map);
		}
		if (gpe->btn_released & (1 << gpc.button_pause_menu)) {
			SetPress(false, SDL_SCANCODE_P);
		}
		if (gpe->btn_released & (1 << gpc.button_fwd)) {
			SetPress(false, inputMapping.Forward);
		}
		if (gpe->btn_released & (1 << gpc.button_back)) {
			SetPress(false, inputMapping.Backwards);
		}
		if (gpe->btn_released & (1 << gpc.button_esc)) {
			SetPress(false, SDL_SCANCODE_ESCAPE);
		}
		if (gps.scene_id == Scene::DEAD) {
			SetPress(false, SDL_SCANCODE_SPACE);
		}
	}

	if (gpe->btn_pressed || gpe->btn_released) {
		Logger->trace("joy pressed {}  released {}  mouse {}", gpe->btn_pressed, gpe->btn_released, button_state);
	}

	if (button_state) {
		goto announce;
	}

	if (conv_state) {
		gps.dead_zone_announced = 0;
	} else {
		if (gps.dead_zone_announced) {
			// do NOT flood MouseEvents() on frames where the joystick is resting
			return;
		} else {
			gps.dead_zone_announced = 1;
		}
	}

announce:

	SetMouseEvents(button_state & 0x7f, gps.x, gps.y);

	//Logger->info("gpc.axis_dead_zone not big enough fly ({},{}) nav ({},{}) conv_state {}", ge->axis_yaw, ge->axis_pitch, ge->axis_nav_ns, ge->axis_nav_ew, conv_state);
}

/// \brief once per frame read out all axes and hats, for perfect smoothness
/// \param gpe event data
void gamepad_poll_data(gamepad_event_t *gpe)
{

	if (!gps.initialized) {
		return;
	}

	// make sure to poll for the axis data on every single frame
	// otherwise we get janky movement since there is no event
	// if the joystick is held still outside the rest position

	if (gpc.axis_yaw_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->axis_yaw = SDL_JoystickGetAxis(m_gameController, gpc.axis_yaw);
	}

	if (gpc.axis_pitch_conf & GAMEPAD_ITEM_ENABLED)  {
		gpe->axis_pitch = SDL_JoystickGetAxis(m_gameController, gpc.axis_pitch);
	}

	if (gpc.axis_long_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->axis_long = SDL_JoystickGetAxis(m_gameController, gpc.axis_long);
	}

	if (gpc.axis_trans_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->axis_trans = SDL_JoystickGetAxis(m_gameController, gpc.axis_trans);
	}

	if (gpc.axis_nav_ns_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->axis_nav_ns = SDL_JoystickGetAxis(m_gameController, gpc.axis_nav_ns);
	}

	if (gpc.axis_nav_ew_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->axis_nav_ew = SDL_JoystickGetAxis(m_gameController, gpc.axis_nav_ew);
	}

	if (gpc.hat_nav_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->hat_nav = SDL_JoystickGetHat(m_gameController, gpc.hat_nav);
	}

	if (gpc.hat_mov_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->hat_mov = SDL_JoystickGetHat(m_gameController, gpc.hat_mov);
	}

	if (gpc.axis_fire_R_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->axis_fire_R = SDL_JoystickGetAxis(m_gameController, gpc.axis_fire_R);
	}

	if (gpc.axis_fire_L_conf & GAMEPAD_ITEM_ENABLED) {
		gpe->axis_fire_L = SDL_JoystickGetAxis(m_gameController, gpc.axis_fire_L);
	}

	// call the event handler only once per frame not inside the SDL_PollEvent() loop
	// https://stackoverflow.com/questions/39376356/non-instantaneous-jerky-movement-using-sdl2-opengl
	gamepad_event_mgr(gpe);
}

/// \brief reconfigure gamepad maximum coverage and operating mode based on recode scene
/// \param scene_id one of SCENE_PREAMBLE_MENU, SCENE_FLIGHT, SCENE_FLIGHT_MENU
void set_scene(const Scene scene_id)
{
	int16_t maxX = 640;
	int16_t maxY = 480;

	gps.scene_id = scene_id;
	switch (scene_id) {
		case Scene::PREAMBLE_MENU:
			gps.max_x = 640;
			gps.max_y = 480;
			gps.nav_mode = 1;
			break;
		case Scene::FLIGHT:
			gps.rest_x = (maxX * mouseScaleX) / 2;
			gps.rest_y = (maxY * mouseScaleY) / 2;
			gps.max_x = maxX * mouseScaleX;
			gps.max_y = maxY * mouseScaleY;
			if (screenWidth_18062C > 640 && screenHeight_180624 > 480)
				VGA_Set_mouse(screenWidth_18062C / 2, screenHeight_180624 / 2);
			else
				VGA_Set_mouse(320, 200);

			gps.nav_mode = 0;
			break;
		case Scene::FLIGHT_MENU:
		case Scene::CHAT_MENU:
		case Scene::SPELL_MENU:
			gps.max_x = screenWidth_18062C;
			gps.max_y = screenHeight_180624;
			gps.nav_mode = 1;
			break;
		default:
			gps.max_x = screenWidth_18062C;
			gps.max_y = screenHeight_180624;
			break;
	}
	Logger->trace("set scene {}, nav_mode {}", (int)scene_id, gps.nav_mode);
}

void OnResolutionChanged(uint32_t width, uint32_t height)
{
	int16_t maxX = 640;
	int16_t maxY = 480;

	if (gps.scene_id == Scene::FLIGHT)
	{
		gps.rest_x = (maxX * mouseScaleX) / 2;
		gps.rest_y = (maxY * mouseScaleY) / 2;
		gps.max_x = maxX * mouseScaleX;
		gps.max_y = maxY * mouseScaleY;
		if (screenWidth_18062C > 640 && screenHeight_180624 > 480)
			VGA_Set_mouse(screenWidth_18062C / 2, screenHeight_180624 / 2);
		else
			VGA_Set_mouse(320, 200);
	}
}

/// \brief set the x,y simulated mouse pointer coordinates of the joystick rest position
/// \param x coordinate
/// \param y coordinate
void joystick_set_env(const int32_t x, const int32_t y)
{
	Logger->trace("pointer rest at {},{} scene {}, window size {},{}", x, y, (int)gps.scene_id, gps.max_x, gps.max_y);
	if (gps.scene_id != Scene::FLIGHT)
	{
		gps.rest_x = x;
		gps.rest_y = y;
	}
	gps.x = x;
	gps.y = y;
}

/// \brief unfinished load effects to be sent to the haptic subsystem
/// \return EXIT_FAILURE on error, EXIT_SUCCESS otherwise
int8_t haptic_load_effects(void) {
	SDL_HapticEffect effect;
	int16_t max_effects = 0;

	if ((hs.cap & SDL_HAPTIC_SINE)==0) {
		Logger->info("haptic sine not supported");
		return EXIT_FAILURE;
	}

	max_effects = SDL_HapticNumEffects(m_haptic);
	if (max_effects < 0) {
		Logger->info("SDL_HapticNumEffects() error {}", SDL_GetError());
	}

	// quake effect
	SDL_memset( &effect, 0, sizeof(SDL_HapticEffect) ); // 0 is safe default
	effect.type = SDL_HAPTIC_SINE;
	effect.periodic.direction.type = SDL_HAPTIC_POLAR; // polar coordinates
	effect.periodic.direction.dir[0] = 18000; // force comes from south
	effect.periodic.period = 1000; // time in ms
	effect.periodic.magnitude = 10000; // out of 32767 strength
	effect.periodic.length = 3000; // time in ms
	effect.periodic.attack_length = 1000; // takes 1 second to get max strength
	effect.periodic.fade_length = 1000; // takes 1 second to fade away

	hs.quake = SDL_HapticNewEffect(m_haptic, &effect);

	return EXIT_SUCCESS;
}

/// \brief send sine-based effect to the haptic subsystem
/// \param effect_id  identifier
void haptic_run_effect(const int effect_id) {
	if (!hs.enabled || ((hs.cap & SDL_HAPTIC_SINE)==0)) {
		return;
	}
	Logger->info("run_effect {}", effect_id);
	SDL_HapticRunEffect(m_haptic, effect_id, 1);
}

/// \brief send rumble effect to the haptic subsystem
/// \param strength defined in the [0-1.0] interval
/// \param length  effect duration in ms
void haptic_rumble_effect(const float strength, const uint32_t length) {
	if ((!hs.enabled) || (!hs.rumble)) {
		return;
	}
	Logger->info("run_rumble {},{}", strength, length);
	SDL_HapticRumblePlay(m_haptic, strength, length);
}

/// \brief send rumble effect to the trigger buttons
/// \param strength_l TL effect level
/// \param strength_r TR effect level
/// \param length effect duration in ms
void haptic_rumble_triggers_effect(const uint16_t strength_l, const uint16_t strength_r, const uint32_t length) {
	if ((!hs.enabled) || (!hs.rumble_trig)) {
		return;
	}
#ifdef _MSC_VER
	//SDL_JoystickRumbleTriggers(m_gameController, strength_l, strength_r, length);
#endif
}

