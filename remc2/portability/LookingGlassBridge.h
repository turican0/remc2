#pragma once

#include <string>

// A Looking Glass display as Looking Glass Bridge reports it, with the values the lenticular interleave
// (lenticular.frag) needs. Bridge's own getters use other scales for some of them (its tilt), so the shader values
// are worked out here from the raw calibration, the way the HoloPlay lightfield shader does.
struct LookingGlassDevice
{
	bool found = false;
	std::string name;
	std::string serial;

	// Where the display is on the desktop and its size in pixels
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	// Raw calibration
	float rawPitch = 0.0f;
	float slope = 0.0f;
	float dpi = 0.0f;
	float viewCone = 0.0f;
	int cellPatternMode = 0;

	// For the shader
	float pitch = 0.0f;    // lenticules across the screen, along the lens direction
	float tilt = 0.0f;     // phase change over the screen's height, per unit of pitch: height / (width * slope)
	float center = 0.0f;   // phase offset
	float subp = 0.0f;     // one subpixel in screen widths: 1 / (3 * width)
	int invView = 0;       // 1 = the views run the other way
	int ri = 0;            // subpixel order: red is the ri'th, blue the bi'th
	int bi = 2;
	bool flipX = false;

	// What Bridge suggests for this display (for the log: the config sets the quilt)
	int quiltWidth = 0;
	int quiltHeight = 0;
	int quiltColumns = 0;
	int quiltRows = 0;
	float quiltAspect = 0.0f;
};

// Asks Looking Glass Bridge (its bridge_inproc library, loaded at run time) for the first Looking Glass display.
// bridgePath: Bridge's install folder; empty = the newest under the default install location. Returns found = false
// (and logs why) without Bridge or a display.
LookingGlassDevice QueryLookingGlassDevice(const std::string& bridgePath);
