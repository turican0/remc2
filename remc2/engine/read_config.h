#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <filesystem>
#include <cstdlib>

#ifdef _MSC_VER
    #include <direct.h>  
    #include <io.h>  
    #include <windows.h>
    #include "../portability/dirent-x.h"
#else
    #include "dirent.h"
#endif

#include "../portability/Config.h"
#include "../portability/port_time.h"
#include "../portability/port_filesystem.h"
#include "../portability/port_sdl_sound.h"
#include "../portability/MouseInputMapping.h"
#include "../portability/KeyboardInputMapping.h"
#include "../engine/CommandLineParser.h"
#include "../utilities/Maths.h"
#include "defs.h"

#ifndef READ_CONFIG
#define READ_CONFIG

// Horizontal alignment of an in-game UI element on the screen
enum class UiAlignment
{
	Left,
	Center,
	Right
};

// Where an in-game UI element sits on the screen: aligned to the left, center or right, then moved by the margins
// (in screen pixels). Only used when not at the default 320x200 / 640x480 resolutions
struct UiElementLayout
{
	UiAlignment alignment = UiAlignment::Left;
	int marginX = 0;   // from the aligned edge (left/right), or from the middle (center, + = right)
	int marginY = 0;   // down from the top (top bar, spell icons) or up from the bottom (spell menu)
};

bool SetConfig();
std::vector<Maths::Zone> ReadZones(std::vector<Maths::Zone> zonesArray);
UiAlignment ParseUiAlignment(std::string alignment, UiAlignment defaultAlignment);
UiElementLayout ReadUiElementLayout(const Config::Settings::UiElement& element, UiAlignment defaultAlignment);
extern int config_skip_screen;
extern int texturepixels;
extern int maxGameFps;
extern int menuFps;
extern int displayIndex;
extern int windowResWidth;
extern int windowResHeight;
extern int gameResWidth;
extern int gameResHeight;
extern int gameUiScale;
extern bool maintainAspectRatio;
extern bool startWindowed;
extern bool bigTextures;
extern bool bigSprites;
extern bool fixedMenuGraphics;
extern bool fixedTmaps;
extern bool extendedFonts;
extern bool sky;
extern bool reflections;
extern bool dynamicLighting;
extern int viewDistanceScale;
extern float fogScale;
extern bool openGLRender;
extern float sizePercentToThreadRender;
extern float sizePercentToThreadRenderSprites;
extern bool multiThreadedRender;
extern int numberOfRenderThreads;
extern bool assignToSpecificCores;
extern bool bilinearFiltering;
extern int antiAliasing;   // MSAA samples, 0 = off
extern bool invertYAxis;
extern bool invertXAxis;
extern float mouseScaleX;
extern float mouseScaleY;
extern bool disableLRButtonsMenuOpen;
extern UiElementLayout topBarLayout;     // the mini-map and the health/mana bars
extern UiElementLayout spellIconsLayout; // the selected left/right spell icons
extern UiElementLayout spellMenuLayout;  // the spell selection menu at the bottom

#define   GAMEPAD_ITEM_DISABLED  0x0
#define    GAMEPAD_ITEM_ENABLED  0x1
#define   GAMEPAD_AXIS_INVERTED  0x2

struct gamepad_config {
	uint16_t axis_yaw;
	uint16_t axis_pitch;
	uint16_t axis_long;
	uint16_t axis_trans;
	uint16_t axis_nav_ns;
	uint16_t axis_nav_ew;
	uint16_t axis_fire_R;
	uint16_t axis_fire_L;
	uint8_t axis_yaw_conf;
	uint8_t axis_pitch_conf;
	uint8_t axis_long_conf;
	uint8_t axis_trans_conf;
	uint8_t axis_nav_ns_conf;
	uint8_t axis_nav_ew_conf;
	uint8_t axis_fire_R_conf;
	uint8_t axis_fire_L_conf;
	std::vector<Maths::Zone> axis_yaw_sensitivity;
	uint16_t axis_yaw_dead_zone;
	std::vector<Maths::Zone> axis_pitch_sensitivity;
	uint16_t axis_pitch_dead_zone;
	uint16_t axis_long_dead_zone;
	uint16_t axis_trans_dead_zone;
	uint16_t axis_long_nav_dead_zone;
	uint16_t axis_trans_nav_dead_zone;
	uint16_t trigger_dead_zone;
	uint16_t hat_nav;
	uint16_t hat_mov;
	uint8_t hat_mov_conf;
	uint8_t hat_nav_conf;
	uint16_t controller_id;
	uint16_t button_spell;
	uint16_t button_minimap;
	uint16_t button_fire_L;
	uint16_t button_fire_R;
	uint16_t button_fwd;
	uint16_t button_back;
	uint16_t button_pause_menu;
	uint16_t button_esc;
	uint16_t button_menu_select;
	bool haptic_enabled;
	uint16_t haptic_gain_max;
};

typedef struct gamepad_config gamepad_config_t;
extern gamepad_config_t gpc;

#endif //READ_CONFIG
