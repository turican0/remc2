#include "read_config.h"

#include <cstdlib>
#include <ctype.h>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
    #include <direct.h>  
    #include <io.h>  
    #include <windows.h>
    #include "portability/dirent-x.h"
#else
    #include "dirent.h"
#endif

#include "portability/port_filesystem.h"
#include "portability/port_sdl_sound.h"
#include "portability/port_time.h"

int config_skip_screen;
int texturepixels = 32;
int maxGameFps = 30;
int menuFps = 30;
int displayIndex = 0;
int windowResWidth = 640;
int windowResHeight = 480;
int gameResWidth = 640;
int gameResHeight = 480;
int gameUiScale = 1;
bool maintainAspectRatio = false;
bool startWindowed = false;
bool bigTextures = false;
bool bigSprites = false;
bool fixedMenuGraphics = false;
bool extendedFonts = false;
bool sky = true;
bool reflections = false;
bool dynamicLighting = false;
int viewDistanceScale = 1;
bool multiThreadedRender = false;
float sizePercentToThreadRender = 0;
int numberOfRenderThreads = 0;
bool assignToSpecificCores = false;
bool openGLRender = false;
bool invertYAxis = true;
bool invertXAxis = false;
float mouseScaleX = 1.0f;
float mouseScaleY = 1.0f;
bool disableLRButtonsMenuOpen = false;

gamepad_config_t gpc;
MouseInputMapping_t mouseMapping;
KeyboardInputMapping_t inputMapping;

std::string findConfigFile() {
	// find location of Config and read it
	std::vector<std::string> config_locations;
	if (CommandLineParams.GetConfigFilePath().length() > 0) {
		config_locations.push_back(CommandLineParams.GetConfigFilePath());
	}
	else {
#if defined(__linux__) || defined(__APPLE__)
		auto env_home_dir = std::getenv("HOME");
		auto env_xdg_config_home_dir = std::getenv("XDG_CONFIG_HOME");
		std::filesystem::path home_dir;
		std::filesystem::path xdg_config_home_dir;
		if (env_home_dir) home_dir = env_home_dir;
		if (env_xdg_config_home_dir) xdg_config_home_dir = env_xdg_config_home_dir;

		if (std::filesystem::exists(xdg_config_home_dir)) {
			config_locations.emplace_back(xdg_config_home_dir / "remc2" / "config.json");
		}
		if (std::filesystem::exists(home_dir)) {
			config_locations.emplace_back(home_dir / ".config" / "remc2" / "config.json");
		}
#else //__linux__
		auto home_drive = std::getenv("HOMEDRIVE");
		auto home_path = std::getenv("HOMEPATH");
		if (home_drive && home_path) {
			std::string home_dir = std::string(home_drive) + "/" + std::string(home_path);
			config_locations.push_back(home_dir + "/remc2/config.json");
		}
#endif //__linux__
		config_locations.push_back(get_exe_path() + "/config.json");
	}
	std::string configfile;
	// first location at which an configfile can be found is chosen
	for (auto configfile_location: config_locations) {
		if (std::filesystem::exists(configfile_location)) {
			configfile = configfile_location;
			break;
		}
	}

	return configfile;
}

std::vector<Maths::Zone> ReadZones(std::vector<Maths::Zone> zonesArray) {

	std::vector<Maths::Zone> zones;

	for (int i = 0; i < zonesArray.size(); i++) // Uses SizeType instead of size_t
	{
		zones.push_back(Maths::Zone{ zonesArray[i].m_xStart, zonesArray[i].m_xEnd, zonesArray[i].m_factor });
	}
	return zones;
}

bool SetConfig() {
	uint8_t gp_temp;
	std::string configFilePath = findConfigFile();
	if (std::filesystem::exists(configFilePath)) {
		if (CommandLineParams.DoShowDebugMessages1())
			std::cout << "Using Config File: " << configFilePath << "\n";
	}
	else {
		if (CommandLineParams.DoShowDebugMessages1())
			std::cout << "Config File cannot be found... Exiting\n";
#ifndef __ANDROID__
		throw std::invalid_argument("Config.json not found!");
#endif
		return false;
	}

	auto config = Config(configFilePath);
	auto settingsValue = config.GetSettingsFromDoc();

	//Game
	maxGameFps = settingsValue.m_Game.m_MaxGameFps;
	if (settingsValue.m_Game.m_SkipIntro)
		config_skip_screen = 1;
	else
		config_skip_screen = 0;

	//Path
	gameFolder = settingsValue.m_Paths.m_GameFolder;
	cdFolder = settingsValue.m_Paths.m_CdFolder;

	//Sound
	hqsound = settingsValue.m_Sound.m_HqSound;
	fixspeedsound = settingsValue.m_Sound.m_FixSpeedSound;
	autoShowObjectivesForForeignLanguages = settingsValue.m_Sound.m_AutoShowObjectivesForForeignLanguages;
	if (settingsValue.m_Sound.m_OggMusic)
	{
		oggmusic = true;
		hqsound = true;//for mp3 music must be activate hqsound
	}
	else
		oggmusic = false;

	if (settingsValue.m_Sound.m_MaxSimultaniousSounds > 10)
		maxSimultaniousSounds = settingsValue.m_Sound.m_MaxSimultaniousSounds;

	oggmusicalternative = settingsValue.m_Sound.m_OggMusicAlternative;
	strcpy(oggmusicFolder, settingsValue.m_Sound.m_OggFolder.c_str());
	strcpy(speechFolder, settingsValue.m_Sound.m_SpeechFolder.c_str());

	//Graphics
	displayIndex = settingsValue.m_Graphics.m_DisplayIndex;
	windowResWidth = settingsValue.m_Graphics.m_WindowResWidth;
	windowResHeight = settingsValue.m_Graphics.m_WindowResHeight;
	if (windowResWidth < 640 || windowResHeight < 480)
	{
		windowResWidth = 640;
		windowResHeight = 480;
	}
	maintainAspectRatio = settingsValue.m_Graphics.m_MaintainAspectRatio;
	startWindowed = settingsValue.m_Graphics.m_StartWindowed;
	gameResWidth = settingsValue.m_Graphics.m_GameDetail.m_GameResWidth;
	gameResHeight = settingsValue.m_Graphics.m_GameDetail.m_GameResHeight;
	if (gameResWidth < 320 || gameResHeight < 200)
	{
		gameResWidth = 320;
		gameResHeight = 200;
	}
	gameUiScale = settingsValue.m_Graphics.m_GameDetail.m_GameUiScale;
	if (gameUiScale < 1)
		gameUiScale = 1;
	if (gameUiScale > 8 || (640 * gameUiScale) > gameResWidth)
	{
		while (gameUiScale > 1 && (640 * gameUiScale) > gameResWidth)
		{
			gameUiScale--;
		}
	}
	highResGraphicsFolder = settingsValue.m_Graphics.m_GameDetail.m_HighResGraphicsFolder;
	if (settingsValue.m_Graphics.m_GameDetail.m_UseHighResGraphics && strlen(highResGraphicsFolder.c_str()) > 0
		&& std::filesystem::is_directory(GetSubDirectoryPath(highResGraphicsFolder.c_str())))
	{
		bigSprites = true;
		bigTextures = true;
		texturepixels = 128;
	}
	else
	{
		texturepixels = 32;
	}

	fixedMenuGraphicsFolder = settingsValue.m_Graphics.m_GameDetail.m_FixedMenuGraphicsFolder;
	if (settingsValue.m_Graphics.m_GameDetail.m_UseFixedMenuGraphics && strlen(fixedMenuGraphicsFolder.c_str()) > 0
		&& std::filesystem::is_directory(GetSubDirectoryPath(fixedMenuGraphicsFolder.c_str())))
	{
		fixedMenuGraphics = true;
	}

	extendedFontsFolder = settingsValue.m_Graphics.m_GameDetail.m_ExtendedFontsFolder;
	if (settingsValue.m_Graphics.m_GameDetail.m_UseExtendedFonts && strlen(extendedFontsFolder.c_str()) > 0
		&& std::filesystem::is_directory(GetSubDirectoryPath(extendedFontsFolder.c_str())))
	{
		extendedFonts = true;
	}

	sky = settingsValue.m_Graphics.m_GameDetail.m_Sky;
	reflections = settingsValue.m_Graphics.m_GameDetail.m_Reflections;
	dynamicLighting = settingsValue.m_Graphics.m_GameDetail.m_DynamicLighting;
	if (settingsValue.m_Graphics.m_GameDetail.m_ViewDistanceScale > 0 && settingsValue.m_Graphics.m_GameDetail.m_ViewDistanceScale < 4)
		viewDistanceScale = settingsValue.m_Graphics.m_GameDetail.m_ViewDistanceScale;

	multiThreadedRender = settingsValue.m_Graphics.m_Threading.m_IsActive;
	sizePercentToThreadRender = settingsValue.m_Graphics.m_Threading.m_SizePercentToThreadRender;
	numberOfRenderThreads = settingsValue.m_Graphics.m_Threading.m_NumberOfRenderThreads;
	if (multiThreadedRender)
	{
		assignToSpecificCores = settingsValue.m_Graphics.m_Threading.m_AssignToSpecificCores;

		if (numberOfRenderThreads < 1)
		{
			numberOfRenderThreads = 1;
		}
	}
	else
	{
		numberOfRenderThreads = 0;
	}

	//Controls
	invertYAxis = settingsValue.m_Controls.m_Mouse.m_InvertYAxis;
	invertXAxis = settingsValue.m_Controls.m_Mouse.m_InvertXAxis;

	//Mouse
	mouseScaleX = settingsValue.m_Controls.m_Mouse.m_MouseScaleX;
	mouseScaleY = settingsValue.m_Controls.m_Mouse.m_MouseScaleY;
	disableLRButtonsMenuOpen = settingsValue.m_Controls.m_Mouse.m_DisableLRButtonsMenuOpen;
	mouseMapping.SpellLeft = settingsValue.m_Controls.m_Mouse.m_SpellLeft;
	mouseMapping.SpellRight = settingsValue.m_Controls.m_Mouse.m_SpellRight;
	mouseMapping.map = settingsValue.m_Controls.m_Mouse.m_Map;
	mouseMapping.SpellMenu = settingsValue.m_Controls.m_Mouse.m_SpellMenu;
	mouseMapping.SpellMenuMark = settingsValue.m_Controls.m_Mouse.m_SpellMenuMark;

	//Keyboard
	inputMapping.Forward = settingsValue.m_Controls.m_Keyboard.m_Forward;
	inputMapping.Backwards = settingsValue.m_Controls.m_Keyboard.m_Backwards;
	inputMapping.Left = settingsValue.m_Controls.m_Keyboard.m_Left;
	inputMapping.Right = settingsValue.m_Controls.m_Keyboard.m_Right;
	inputMapping.Map = settingsValue.m_Controls.m_Keyboard.m_Map;
	inputMapping.SpellMenu = settingsValue.m_Controls.m_Keyboard.m_SpellMenu;
	inputMapping.SpellMenuMark = settingsValue.m_Controls.m_Keyboard.m_SpellMenuMark;

	gpc.axis_yaw = settingsValue.m_Controls.m_GamePad.m_AxisYaw;
	gpc.axis_pitch = settingsValue.m_Controls.m_GamePad.m_AxisPitch;
	gpc.axis_long = settingsValue.m_Controls.m_GamePad.m_AxisLong;
	gpc.axis_trans = settingsValue.m_Controls.m_GamePad.m_AxisTrans;
	gpc.axis_nav_ns = settingsValue.m_Controls.m_GamePad.m_AxisNavNs;
	gpc.axis_nav_ew = settingsValue.m_Controls.m_GamePad.m_AxisNavEw;
	gpc.axis_fire_R = settingsValue.m_Controls.m_GamePad.m_AxisFireR;
	gpc.axis_fire_L = settingsValue.m_Controls.m_GamePad.m_AxisFireL;

	gp_temp = settingsValue.m_Controls.m_GamePad.m_AxisYawInv;
	if (gpc.axis_yaw) {
		gpc.axis_yaw -= 1; // go back to SDL axis notation
		gpc.axis_yaw_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	gp_temp = settingsValue.m_Controls.m_GamePad.m_AxisPitchInv;
	if (gpc.axis_pitch) {
		gpc.axis_pitch -= 1; // go back to SDL axis notation
		gpc.axis_pitch_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	gp_temp = settingsValue.m_Controls.m_GamePad.m_AxisLongInv;
	if (gpc.axis_long) {
		gpc.axis_long -= 1; // go back to SDL axis notation
		gpc.axis_long_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	gp_temp = settingsValue.m_Controls.m_GamePad.m_AxisTransInv;
	if (gpc.axis_trans) {
		gpc.axis_trans -= 1; // go back to SDL axis notation
		gpc.axis_trans_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	gp_temp = settingsValue.m_Controls.m_GamePad.m_AxisNavNsInv;
	if (gpc.axis_nav_ns) {
		gpc.axis_nav_ns -= 1; // go back to SDL axis notation
		gpc.axis_nav_ns_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	gp_temp = settingsValue.m_Controls.m_GamePad.m_AxisNavEwInv;
	if (gpc.axis_nav_ew) {
		gpc.axis_nav_ew -= 1; // go back to SDL axis notation
		gpc.axis_nav_ew_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	if (gpc.axis_fire_R) {
		gpc.axis_fire_R -= 1; // go back to SDL axis notation
		gpc.axis_fire_R_conf = GAMEPAD_ITEM_ENABLED;
	}

	if (gpc.axis_fire_L) {
		gpc.axis_fire_L -= 1; // go back to SDL axis notation
		gpc.axis_fire_L_conf = GAMEPAD_ITEM_ENABLED;
	}

	gpc.controller_id = settingsValue.m_Controls.m_GamePad.m_ControllerId;
	gpc.button_fire_L = settingsValue.m_Controls.m_GamePad.m_ButtonFireL;
	gpc.button_fire_R = settingsValue.m_Controls.m_GamePad.m_ButtonFireR;
	gpc.button_spell = settingsValue.m_Controls.m_GamePad.m_ButtonSpell;
	gpc.button_minimap = settingsValue.m_Controls.m_GamePad.m_ButtonMiniMap;
	gpc.button_fwd = settingsValue.m_Controls.m_GamePad.m_ButtonFwd;
	gpc.button_back = settingsValue.m_Controls.m_GamePad.m_ButtonBack;
	gpc.button_pause_menu = settingsValue.m_Controls.m_GamePad.m_ButtonPauseMenu;
	gpc.button_esc = settingsValue.m_Controls.m_GamePad.m_ButtonEsc;
	gpc.button_menu_select = settingsValue.m_Controls.m_GamePad.m_ButtonMenuSelect;

	gpc.axis_yaw_sensitivity = ReadZones(settingsValue.m_Controls.m_GamePad.m_AxisYawSensitivity);
	gpc.axis_yaw_dead_zone = settingsValue.m_Controls.m_GamePad.m_AxisYawDeadZone;
	gpc.axis_pitch_sensitivity = ReadZones(settingsValue.m_Controls.m_GamePad.m_AxisPitchSensitivity);
	gpc.axis_pitch_dead_zone = settingsValue.m_Controls.m_GamePad.m_AxisPitchDeadZone;
	gpc.axis_long_dead_zone = settingsValue.m_Controls.m_GamePad.m_AxisLongDeadZone;
	gpc.axis_trans_dead_zone = settingsValue.m_Controls.m_GamePad.m_AxisTransDeadZone;

	gpc.axis_long_nav_dead_zone = settingsValue.m_Controls.m_GamePad.m_AxisLongNavDeadZone;
	gpc.axis_trans_nav_dead_zone = settingsValue.m_Controls.m_GamePad.m_AxisTransNavDeadZone;

	gpc.trigger_dead_zone = settingsValue.m_Controls.m_GamePad.m_TriggerDeadZone;

	gpc.hat_nav = settingsValue.m_Controls.m_GamePad.m_HatNav;
	gpc.hat_mov = settingsValue.m_Controls.m_GamePad.m_HatMov;

	gp_temp = settingsValue.m_Controls.m_GamePad.m_HatNavInv;
	if (gpc.hat_nav) {
		gpc.hat_nav -= 1; // go back to SDL axis notation
		gpc.hat_nav_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	gp_temp = settingsValue.m_Controls.m_GamePad.m_HatMovInv;
	if (gpc.hat_mov) {
		gpc.hat_mov -= 1; // go back to SDL axis notation
		gpc.hat_mov_conf = GAMEPAD_ITEM_ENABLED | (gp_temp ? GAMEPAD_AXIS_INVERTED : 0);
	}

	gpc.haptic_enabled = settingsValue.m_Controls.m_GamePad.m_HapticEnabled;
	gpc.haptic_gain_max = settingsValue.m_Controls.m_GamePad.m_HapticMaxGain;

	return true;
};
