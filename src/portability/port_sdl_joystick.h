#pragma once
#ifndef PORT_SDL_JOYSTICK_H
#define PORT_SDL_JOYSTICK_H

#include "engine/Scene.h"
#include "engine/globals.h"

///< gamepad_event_t flags
#define      GP_BTN_RELEASED  0x40
#define       GP_BTN_PRESSED  0x80

struct gamepad_event {
	int16_t axis_yaw;
	int16_t axis_pitch;
	int16_t axis_long;
	int16_t axis_trans;
	int16_t axis_nav_ns;
	int16_t axis_nav_ew;
	int16_t axis_fire_R;
	int16_t axis_fire_L;
	uint8_t hat_nav;
	uint8_t hat_mov;
	uint64_t btn_released;  ///< bitwise OR of every released button
	uint64_t btn_pressed;   ///< bitwise OR of every pressed button
	uint8_t flag;           ///< bitwise OR of every parameter that was updated
};
typedef struct gamepad_event gamepad_event_t;

void set_scene(const Scene scene_id);
void joystick_set_env(const int32_t x, const int32_t y);
void gamepad_sdl_init(void);
void gamepad_sdl_close(void);
void gamepad_init(const int gameResWidth, const int gameResHeight);
void gamepad_poll_data(gamepad_event_t *gpe);
void OnResolutionChanged(uint32_t width, uint32_t height);

void haptic_run_effect(const int effect_id);
void haptic_rumble_effect(const float strength, const uint32_t length);
void haptic_rumble_triggers_effect(const uint16_t strength_l, const uint16_t strength_r, const uint32_t length);

#endif //PORT_SDL_JOYSTICK
