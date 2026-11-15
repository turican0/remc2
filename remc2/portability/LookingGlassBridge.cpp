#include "LookingGlassBridge.h"

#include <cmath>
#include <filesystem>
#include <vector>

#include "port_filesystem.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace
{
	// bridge_inproc's exports (Bridge's runtime/bridge.h), loaded by name so the game doesn't link against Bridge
	using InitializeBridge = bool (*)(const wchar_t* appName);
	using UninitializeBridge = bool (*)();
	using GetDisplays = bool (*)(int* count, unsigned long* indices);
	using GetCalibrationForDisplay = bool (*)(unsigned long display, float* center, float* pitch, float* slope, int* width,
		int* height, float* dpi, float* flipX, int* invView, float* viewCone, float* fringe, int* cellPatternMode,
		int* cellCount, void* cells);
	using GetIntForDisplay = bool (*)(unsigned long display, int* value);
	using GetDimensionsForDisplay = bool (*)(unsigned long display, unsigned long* width, unsigned long* height);
	using GetWindowPositionForDisplay = bool (*)(unsigned long display, long* x, long* y);
	using GetStringForDisplay = bool (*)(unsigned long display, int* length, wchar_t* text);
	using GetDefaultQuiltSettingsForDisplay = bool (*)(unsigned long display, float* aspect, int* width, int* height,
		int* columns, int* rows);

	// The newest "Looking Glass Bridge <version>" folder in the default install location
	std::filesystem::path FindBridgeFolder()
	{
		std::filesystem::path best;
		std::vector<int> bestVersion;
		std::error_code error;
		const std::filesystem::path root = "C:/Program Files/Looking Glass";
		for (const auto& entry : std::filesystem::directory_iterator(root, error))
		{
			const std::string name = entry.path().filename().string();
			const std::string prefix = "Looking Glass Bridge ";
			if (!entry.is_directory() || name.rfind(prefix, 0) != 0
				|| !std::filesystem::exists(entry.path() / "bridge_inproc.dll"))
				continue;
			std::vector<int> version;
			for (size_t at = prefix.size(); at < name.size();)
			{
				version.push_back(std::atoi(name.c_str() + at));
				at = name.find('.', at);
				if (at == std::string::npos)
					break;
				at++;
			}
			if (best.empty() || version > bestVersion)
			{
				best = entry.path();
				bestVersion = version;
			}
		}
		return best;
	}

	std::string Narrow(const std::wstring& text)
	{
		if (text.empty())
			return std::string();
		const int length = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(), nullptr, 0, nullptr, nullptr);
		std::string result(length, '\0');
		WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(), result.data(), length, nullptr, nullptr);
		return result;
	}

	std::string GetString(GetStringForDisplay get, unsigned long display)
	{
		int length = 0;
		if (!get || !get(display, &length, nullptr) || length <= 0)
			return std::string();
		std::wstring text(length, L'\0');
		if (!get(display, &length, text.data()))
			return std::string();
		text.resize(wcsnlen(text.c_str(), text.size()));
		return Narrow(text);
	}
}

LookingGlassDevice QueryLookingGlassDevice(const std::string& bridgePath)
{
	LookingGlassDevice device;

	const std::filesystem::path folder = bridgePath.empty() ? FindBridgeFolder() : std::filesystem::path(bridgePath);
	if (folder.empty() || !std::filesystem::exists(folder / "bridge_inproc.dll"))
	{
		Logger->warn("Looking Glass: Bridge not found ({}), is Looking Glass Bridge installed?",
			folder.empty() ? std::string("C:/Program Files/Looking Glass") : folder.string());
		return device;
	}

	// bridge_inproc needs the other libraries in its folder
	SetDllDirectoryW(folder.wstring().c_str());
	HMODULE bridge = LoadLibraryExW((folder / "bridge_inproc.dll").wstring().c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
	SetDllDirectoryW(nullptr);
	if (!bridge)
	{
		Logger->warn("Looking Glass: could not load {} (error {})", (folder / "bridge_inproc.dll").string(), GetLastError());
		return device;
	}

	const auto initialize = reinterpret_cast<InitializeBridge>(GetProcAddress(bridge, "initialize_bridge"));
	const auto uninitialize = reinterpret_cast<UninitializeBridge>(GetProcAddress(bridge, "uninitialize_bridge"));
	const auto getDisplays = reinterpret_cast<GetDisplays>(GetProcAddress(bridge, "get_displays"));
	const auto getCalibration = reinterpret_cast<GetCalibrationForDisplay>(GetProcAddress(bridge, "get_calibration_for_display"));
	const auto getRi = reinterpret_cast<GetIntForDisplay>(GetProcAddress(bridge, "get_ri_for_display"));
	const auto getBi = reinterpret_cast<GetIntForDisplay>(GetProcAddress(bridge, "get_bi_for_display"));
	const auto getDimensions = reinterpret_cast<GetDimensionsForDisplay>(GetProcAddress(bridge, "get_dimensions_for_display"));
	const auto getPosition = reinterpret_cast<GetWindowPositionForDisplay>(GetProcAddress(bridge, "get_window_position_for_display"));
	const auto getName = reinterpret_cast<GetStringForDisplay>(GetProcAddress(bridge, "get_device_name_for_display"));
	const auto getSerial = reinterpret_cast<GetStringForDisplay>(GetProcAddress(bridge, "get_device_serial_for_display"));
	const auto getQuilt = reinterpret_cast<GetDefaultQuiltSettingsForDisplay>(GetProcAddress(bridge, "get_default_quilt_settings_for_display"));
	if (!initialize || !uninitialize || !getDisplays || !getCalibration || !getDimensions || !getPosition)
	{
		Logger->warn("Looking Glass: {} is missing functions this needs (a different Bridge version?)", folder.string());
		FreeLibrary(bridge);
		return device;
	}

	if (!initialize(L"remc2"))
	{
		Logger->warn("Looking Glass: Bridge did not initialise, is Looking Glass Bridge running?");
		FreeLibrary(bridge);
		return device;
	}

	int count = 0;
	std::vector<unsigned long> displays;
	if (getDisplays(&count, nullptr) && count > 0)
	{
		displays.resize(count);
		if (!getDisplays(&count, displays.data()))
			displays.clear();
	}

	if (displays.empty())
		Logger->warn("Looking Glass: Bridge reports no Looking Glass display, is it connected?");
	else
	{
		const unsigned long display = displays[0];
		float center = 0, pitch = 0, slope = 0, dpi = 0, flipX = 0, viewCone = 0, fringe = 0;
		int width = 0, height = 0, invView = 0, cellPatternMode = 0, cellCount = 0;
		unsigned long displayWidth = 0, displayHeight = 0;
		long x = 0, y = 0;
		if (getCalibration(display, &center, &pitch, &slope, &width, &height, &dpi, &flipX, &invView, &viewCone, &fringe,
				&cellPatternMode, &cellCount, nullptr)
			&& getDimensions(display, &displayWidth, &displayHeight) && getPosition(display, &x, &y)
			&& width > 0 && height > 0 && dpi > 0 && slope != 0)
		{
			device.found = true;
			device.name = GetString(getName, display);
			device.serial = GetString(getSerial, display);
			device.x = (int)x;
			device.y = (int)y;
			device.width = (int)displayWidth;
			device.height = (int)displayHeight;
			device.rawPitch = pitch;
			device.slope = slope;
			device.dpi = dpi;
			device.viewCone = viewCone;
			device.cellPatternMode = cellPatternMode;

			// The HoloPlay lightfield shader's values: the pitch along the lenses, the tilt of the lenses in screen units
			const float screenInches = (float)width / dpi;
			device.pitch = pitch * screenInches * std::cos(std::atan(1.0f / slope));
			device.tilt = (float)height / ((float)width * slope);
			device.flipX = flipX != 0.0f;
			if (device.flipX)
				device.tilt = -device.tilt;
			device.center = center;
			device.subp = 1.0f / (3.0f * (float)width);
			device.invView = invView;
			if (getRi)
				getRi(display, &device.ri);
			if (getBi)
				getBi(display, &device.bi);
			if (getQuilt)
				getQuilt(display, &device.quiltAspect, &device.quiltWidth, &device.quiltHeight, &device.quiltColumns, &device.quiltRows);

			Logger->info("Looking Glass: {} {} at {},{} {}x{}: pitch {} slope {} center {} dpi {} invView {} subpixels {}/{}",
				device.name, device.serial, device.x, device.y, device.width, device.height, pitch, slope, center, dpi,
				invView, device.ri, device.bi);
			Logger->info("Looking Glass: Bridge's quilt for it is {}x{} ({} x {} views, aspect {})",
				device.quiltWidth, device.quiltHeight, device.quiltColumns, device.quiltRows, device.quiltAspect);
			if (cellPatternMode != 0)
				Logger->warn("Looking Glass: the display uses subpixel cell pattern {}, only the plain RGB stripe is "
					"supported, the image may be wrong", cellPatternMode);
		}
		else
			Logger->warn("Looking Glass: Bridge has no calibration for display {}", display);
	}

	// Bridge keeps threads (and a crash handler) of its own after initialising, so the library stays loaded
	uninitialize();
	return device;
}

#else

LookingGlassDevice QueryLookingGlassDevice(const std::string& /*bridgePath*/)
{
	Logger->warn("Looking Glass: reading the display through Bridge is only supported on Windows");
	return LookingGlassDevice();
}

#endif
