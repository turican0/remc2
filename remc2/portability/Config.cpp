#include <algorithm>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "Config.h"

using json = nlohmann::ordered_json;

Config::Config(std::string fileName)
{
	m_FileName = fileName;
	LoadFromFile(fileName);
}

bool Config::LoadFromFile()
{
	return LoadFromFile(m_FileName);
}

bool Config::LoadFromFile(std::string fileName)
{
	if (std::filesystem::exists(fileName))
	{
		auto jsonStr = ReadFileToString(fileName);

		if (jsonStr.size() > 0)
		{
			try
			{
				m_Document = json::parse(jsonStr);
				if (m_Document.contains("settings"))
				{
					return true;
				}
			}
			catch (const json::parse_error& e)
			{
				std::cout << "JSON parse error: " << e.what();
			}
		}
	}
	return false;
}

std::string Config::ReadStringValue(const json& settings, const char* name)
{
	if (settings.contains(name) && settings[name].is_string())
	{
		return settings[name].get<std::string>();
	}
	return "";
}

int Config::ReadIntValue(const json& settings, const char* name)
{
	if (settings.contains(name) && settings[name].is_number())
	{
		return settings[name].get<int>();
	}
	return 0;
}

float Config::ReadFloatValue(const json& settings, const char* name)
{
	if (settings.contains(name) && settings[name].is_number())
	{
		return settings[name].get<float>();
	}
	return 0.0f;
}

bool Config::ReadBoolValue(const json& settings, const char* name)
{
	if (settings.contains(name) && settings[name].is_boolean())
	{
		return settings[name].get<bool>();
	}
	return false;
}

SDL_Scancode Config::ReadKeyScancode(const json& settings, const char* name)
{
	if (settings.contains(name) && settings[name].is_string())
	{
		return m_ConfigToSdlScancode.GetScancode(settings[name].get<std::string>());
	}
	return SDL_Scancode::SDL_SCANCODE_UNKNOWN;
}

void Config::SetString(json& obj, const char* key, const std::string& value)
{
	obj[key] = value;
}

void Config::SetInt(json& obj, const char* key, int value)
{
	obj[key] = value;
}

void Config::SetFloat(json& obj, const char* key, float value)
{
	obj[key] = value;
}

void Config::SetBool(json& obj, const char* key, bool value)
{
	obj[key] = value;
}

void Config::SetZones(json& obj, const char* key, const std::vector<Maths::Zone>& zones)
{
	nlohmann::json axisYawSensitivity;

	axisYawSensitivity["zones"] = nlohmann::json::array();

	for (const auto& zone : zones)
	{
		axisYawSensitivity["zones"].push_back({
			{"start",  zone.m_xStart},
			{"end",    zone.m_xEnd},
			{"factor", zone.m_factor}
			});
	}

	obj[key] = axisYawSensitivity;
}

json& Config::GetOrCreate(json& parent, const char* key)
{
	if (!parent.contains(key))
	{
		parent[key] = json::object();
	}
	return parent[key];
}

Config::Settings Config::GetSettingsFromDoc()
{
	return GetSettings(m_Document);
}

Config::Settings Config::GetSettings(json& document)
{
	Settings settings;

	auto& settingsArray = m_Document["settings"];

	for (auto& entry : settingsArray)
	{
		if (entry.contains("name") && entry.contains("isActive") && entry["isActive"].get<bool>() == true)
		{
			settings.m_Name = ReadStringValue(entry, "name");
			settings.m_Version = ReadStringValue(entry, "version");
			settings.m_LaunchArguments = ReadStringValue(entry, "launchArguments");
			settings.m_Paths = GetSettingsPaths(entry);
			settings.m_Multiplayer = GetMultiplayer(entry);
			settings.m_Sound = GetSound(entry);
			settings.m_Graphics = GetGraphics(entry);
			settings.m_Game = GetGame(entry);
			settings.m_Controls = GetControls(entry);
			ReadOldFolders(entry, settings);
			break;
		}
	}
	return settings;
}

Config::Settings::Multiplayer Config::GetMultiplayer(const json& settings)
{
	Config::Settings::Multiplayer multiplayerValues;
	if (settings.contains("multiplayer"))
	{
		const auto& multiplayer = settings["multiplayer"];
		multiplayerValues.m_ServerPort = ReadIntValue(multiplayer, "serverPort");
		multiplayerValues.m_ClientPort = ReadIntValue(multiplayer, "clientPort");
		multiplayerValues.m_ClientServerPort = ReadIntValue(multiplayer, "clientServerPort");
		multiplayerValues.m_ClientServerIp = ReadStringValue(multiplayer, "clientServerIp");
		multiplayerValues.m_RecordPlayFile = ReadStringValue(multiplayer, "recordPlayFile");
		multiplayerValues.m_Debug = ReadBoolValue(multiplayer, "debug");
	}
	return multiplayerValues;
}

Config::Settings::Game Config::GetGame(const json& settings)
{
	Config::Settings::Game gameValues;
	if (settings.contains("game"))
	{
		const auto& game = settings["game"];
		gameValues.m_MaxGameFps = ReadIntValue(game, "maxGameFps");
		gameValues.m_SkipIntro = ReadBoolValue(game, "skipIntro");
	}
	return gameValues;
}

Config::Settings::Controls Config::GetControls(const json& settings)
{
	Config::Settings::Controls controlValues;

	const auto& controls = settings["controls"];

	if (controls.contains("mouse"))
	{
		for (const auto& mouse : controls["mouse"])
		{
			if (mouse.contains("isActive") && mouse["isActive"].get<bool>() == true)
			{
				controlValues.m_Mouse.m_InvertXAxis = ReadBoolValue(mouse, "invertXAxis");
				controlValues.m_Mouse.m_InvertYAxis = ReadBoolValue(mouse, "invertYAxis");
				controlValues.m_Mouse.m_MouseScaleX = ReadFloatValue(mouse, "mouseScaleX");
				controlValues.m_Mouse.m_MouseScaleY = ReadFloatValue(mouse, "mouseScaleY");
				controlValues.m_Mouse.m_DisableLRButtonsMenuOpen = ReadBoolValue(mouse, "disableLRButtonsMenuOpen");
				controlValues.m_Mouse.m_SpellLeft = ReadIntValue(mouse, "spellLeft");
				controlValues.m_Mouse.m_SpellRight = ReadIntValue(mouse, "spellRight");
				controlValues.m_Mouse.m_Map = ReadIntValue(mouse, "map");
				controlValues.m_Mouse.m_SpellMenu = ReadIntValue(mouse, "spellMenu");
				controlValues.m_Mouse.m_SpellMenuMark = ReadIntValue(mouse, "spellMenuMark");
				break;
			}
		}
	}

	if (controls.contains("keyboard"))
	{
		for (const auto& keyboard : controls["keyboard"])
		{
			if (keyboard.contains("isActive") && keyboard["isActive"].get<bool>() == true)
			{
				controlValues.m_Keyboard.m_Forward = ReadKeyScancode(keyboard, "forward");
				controlValues.m_Keyboard.m_Backwards = ReadKeyScancode(keyboard, "backwards");
				controlValues.m_Keyboard.m_Left = ReadKeyScancode(keyboard, "left");
				controlValues.m_Keyboard.m_Right = ReadKeyScancode(keyboard, "right");
				controlValues.m_Keyboard.m_Map = ReadKeyScancode(keyboard, "map");
				controlValues.m_Keyboard.m_SpellMenu = ReadKeyScancode(keyboard, "spellMenu");
				controlValues.m_Keyboard.m_SpellMenuMark = ReadKeyScancode(keyboard, "spellMenuMark");
				controlValues.m_Keyboard.m_DisableLRKeyRoll = ReadBoolValue(keyboard, "disableLRKeyRoll");
				break;
			}
		}
	}

	if (controls.contains("gamePad"))
	{
		for (const auto& gamePad : controls["gamePad"])
		{
			if (gamePad.contains("isActive") && gamePad["isActive"].get<bool>() == true)
			{
				controlValues.m_GamePad.m_Name = ReadStringValue(gamePad, "name");
				controlValues.m_GamePad.m_ButtonMiniMap = (uint16_t)ReadIntValue(gamePad, "buttonMiniMap");
				controlValues.m_GamePad.m_ButtonSpell = (uint16_t)ReadIntValue(gamePad, "buttonSpell");
				controlValues.m_GamePad.m_ButtonPauseMenu = (uint16_t)ReadIntValue(gamePad, "buttonPauseMenu");
				controlValues.m_GamePad.m_ButtonEsc = (uint16_t)ReadIntValue(gamePad, "buttonEsc");
				controlValues.m_GamePad.m_ButtonFireL = (uint16_t)ReadIntValue(gamePad, "buttonFireL");
				controlValues.m_GamePad.m_ButtonFireR = (uint16_t)ReadIntValue(gamePad, "buttonFireR");
				controlValues.m_GamePad.m_ButtonMenuSelect = (uint16_t)ReadIntValue(gamePad, "buttonMenuSelect");
				controlValues.m_GamePad.m_TriggerDeadZone = (uint16_t)ReadIntValue(gamePad, "triggerDeadZone");
				controlValues.m_GamePad.m_HapticEnabled = ReadBoolValue(gamePad, "hapticEnabled");
				controlValues.m_GamePad.m_HapticMaxGain = (uint16_t)ReadIntValue(gamePad, "hapticMaxGain");
				controlValues.m_GamePad.m_HatNav = (uint16_t)ReadIntValue(gamePad, "hatNav");
				controlValues.m_GamePad.m_HatMov = (uint16_t)ReadIntValue(gamePad, "hatMov");
				controlValues.m_GamePad.m_HatNavInv = ReadBoolValue(gamePad, "hatNavInv");
				controlValues.m_GamePad.m_HatMovInv = ReadBoolValue(gamePad, "hatMovInv");
				controlValues.m_GamePad.m_AxisLong = (uint16_t)ReadIntValue(gamePad, "axisLong");
				controlValues.m_GamePad.m_AxisLongDeadZone = (uint16_t)ReadIntValue(gamePad, "axisLongDeadZone");
				controlValues.m_GamePad.m_AxisLongNavDeadZone = (uint16_t)ReadIntValue(gamePad, "axisLongNavDeadZone");
				controlValues.m_GamePad.m_AxisLongInv = ReadBoolValue(gamePad, "axisLongInv");
				controlValues.m_GamePad.m_AxisTrans = (uint16_t)ReadIntValue(gamePad, "axisTrans");
				controlValues.m_GamePad.m_AxisTransDeadZone = (uint16_t)ReadIntValue(gamePad, "axisTransDeadZone");
				controlValues.m_GamePad.m_AxisLongNavDeadZone = (uint16_t)ReadIntValue(gamePad, "axisTransNavDeadZone");
				controlValues.m_GamePad.m_AxisNavNs = (uint16_t)ReadIntValue(gamePad, "axisNavNs");
				controlValues.m_GamePad.m_AxisNavNsInv = ReadBoolValue(gamePad, "axisNavNsInv");
				controlValues.m_GamePad.m_AxisNavEw = (uint16_t)ReadIntValue(gamePad, "axisNavEw");
				controlValues.m_GamePad.m_AxisNavEwInv = ReadBoolValue(gamePad, "axisNavEwInv");
				controlValues.m_GamePad.m_AxisFireR = (uint16_t)ReadIntValue(gamePad, "axisFireR");
				controlValues.m_GamePad.m_AxisFireL = (uint16_t)ReadIntValue(gamePad, "axisFireL");
				controlValues.m_GamePad.m_AxisYaw = (uint16_t)ReadIntValue(gamePad, "axisYaw");
				controlValues.m_GamePad.m_AxisYawInv = ReadBoolValue(gamePad, "axisYawInv");
				controlValues.m_GamePad.m_AxisYawDeadZone = (uint16_t)ReadIntValue(gamePad, "axisYawDeadZone");

				if (gamePad.contains("axisYawSensitivity"))
				{
					const auto& axisYawSensitivity = gamePad["axisYawSensitivity"];
					if (axisYawSensitivity.contains("zones"))
					{
						for (const auto& zone : axisYawSensitivity["zones"])
						{
							if (zone.contains("start") && zone.contains("end") && zone.contains("factor"))
							{
								controlValues.m_GamePad.m_AxisYawSensitivity.push_back(Maths::Zone{
									(uint16_t)zone["start"].get<int>(),
									(uint16_t)zone["end"].get<int>(),
									zone["factor"].get<double>()
									});
							}
						}
					}
				}

				controlValues.m_GamePad.m_AxisPitch = (uint16_t)ReadIntValue(gamePad, "axisPitch");
				controlValues.m_GamePad.m_AxisPitchInv = ReadBoolValue(gamePad, "axisPitchInv");
				controlValues.m_GamePad.m_AxisPitchDeadZone = (uint16_t)ReadIntValue(gamePad, "axisPitchDeadZone");

				if (gamePad.contains("axisPitchSensitivity"))
				{
					const auto& axisPitchSensitivity = gamePad["axisPitchSensitivity"];
					if (axisPitchSensitivity.contains("zones"))
					{
						for (const auto& zone : axisPitchSensitivity["zones"])
						{
							if (zone.contains("start") && zone.contains("end") && zone.contains("factor"))
							{
								controlValues.m_GamePad.m_AxisPitchSensitivity.push_back(Maths::Zone{
									(uint16_t)zone["start"].get<int>(),
									(uint16_t)zone["end"].get<int>(),
									zone["factor"].get<double>()
									});
							}
						}
					}
				}
				break;
			}
		}
	}
	return controlValues;
}

Config::Settings::Graphics Config::GetGraphics(const json& settings)
{
	Config::Settings::Graphics graphicsValues;

	if (settings.contains("graphics"))
	{
		const auto& graphics = settings["graphics"];
		graphicsValues.m_DisplayIndex = ReadIntValue(graphics, "displayIndex");
		graphicsValues.m_WindowResWidth = ReadIntValue(graphics, "windowResWidth");
		graphicsValues.m_WindowResHeight = ReadIntValue(graphics, "windowResHeight");
		graphicsValues.m_MaintainAspectRatio = ReadBoolValue(graphics, "maintainAspectRatio");
		graphicsValues.m_StartWindowed = ReadBoolValue(graphics, "startWindowed");
		graphicsValues.m_Render = ReadStringValue(graphics, "render");

		graphicsValues.m_GameDetail = GetGameDetail(graphics);
		graphicsValues.m_Threading = GetThreading(graphics);
		graphicsValues.m_Hardware = GetHardware(graphics);
		graphicsValues.m_InGameUi = GetInGameUi(graphics);
	}
	return graphicsValues;
}

Config::Settings::GameDetail Config::GetGameDetail(const json& graphics)
{
	Config::Settings::GameDetail gameDetailValues;

	if (graphics.contains("gameDetail"))
	{
		const auto& gameDetail = graphics["gameDetail"];
		gameDetailValues.m_GameResWidth = ReadIntValue(gameDetail, "gameResWidth");
		gameDetailValues.m_GameResHeight = ReadIntValue(gameDetail, "gameResHeight");
		gameDetailValues.m_GameUiScale = ReadIntValue(gameDetail, "gameUiScale");
		gameDetailValues.m_MapEntityScale = std::clamp(gameDetail.contains("mapEntityScale") ? ReadIntValue(gameDetail, "mapEntityScale") : 1, 1, 4);
		gameDetailValues.m_UseHighResGraphics = ReadBoolValue(gameDetail, "useHighResGraphics");
		gameDetailValues.m_UseFixedMenuGraphics = ReadBoolValue(gameDetail, "useFixedMenuGraphics");
		gameDetailValues.m_UseFixedTmaps = ReadBoolValue(gameDetail, "useFixedTmaps");
		gameDetailValues.m_UseExtendedFonts = ReadBoolValue(gameDetail, "useExtendedFonts");
		gameDetailValues.m_Sky = ReadBoolValue(gameDetail, "sky");
		gameDetailValues.m_Reflections = ReadBoolValue(gameDetail, "reflections");
		gameDetailValues.m_DynamicLighting = ReadBoolValue(gameDetail, "dynamicLighting");
		gameDetailValues.m_ViewDistanceScale = ReadIntValue(gameDetail, "viewDistanceScale");
		if (gameDetail.contains("fogScale"))
			gameDetailValues.m_FogScale = ReadFloatValue(gameDetail, "fogScale");
		gameDetailValues.m_Paths = GetPaths(gameDetail);
	}
	return gameDetailValues;
}

Config::Settings::Threading Config::GetThreading(const json& graphics)
{
	Config::Settings::Threading threadingValues;
	if (graphics.contains("threading"))
	{
		const auto& threading = graphics["threading"];
		threadingValues.m_IsActive = threading.value("isActive", false);
		threadingValues.m_SizePercentToThreadRender = ReadFloatValue(threading, "sizePercentToThreadRender");
		threadingValues.m_SizePercentToThreadRenderSprites = ReadFloatValue(threading, "sizePercentToThreadRenderSprites");
		threadingValues.m_NumberOfRenderThreads = (uint8_t)ReadIntValue(threading, "numberOfRenderThreads");
		threadingValues.m_AssignToSpecificCores = ReadBoolValue(threading, "assignToSpecificCores");
	}
	return threadingValues;
}

Config::Settings::Hardware Config::GetHardware(const json& graphics)
{
	Config::Settings::Hardware hardwareValues;
	if (graphics.contains("hardware"))
	{
		const auto& hardware = graphics["hardware"];
		hardwareValues.m_BilinearFiltering = ReadBoolValue(hardware, "bilinearFiltering");
		hardwareValues.m_AntiAliasing = ReadIntValue(hardware, "antiAliasing");
		if (hardware.contains("lookingGlass"))
		{
			const auto& lookingGlass = hardware["lookingGlass"];
			auto& values = hardwareValues.m_LookingGlass;
			values.m_Enabled = ReadBoolValue(lookingGlass, "enabled");
			if (lookingGlass.contains("rows"))
				values.m_Rows = ReadIntValue(lookingGlass, "rows");
			if (lookingGlass.contains("columns"))
				values.m_Columns = ReadIntValue(lookingGlass, "columns");
			if (lookingGlass.contains("cellWidth"))
				values.m_CellWidth = ReadIntValue(lookingGlass, "cellWidth");
			if (lookingGlass.contains("cellHeight"))
				values.m_CellHeight = ReadIntValue(lookingGlass, "cellHeight");
			if (lookingGlass.contains("viewSpacing"))
				values.m_ViewSpacing = ReadIntValue(lookingGlass, "viewSpacing");
			if (lookingGlass.contains("convergence"))
				values.m_Convergence = ReadIntValue(lookingGlass, "convergence");
			if (lookingGlass.contains("display"))
				values.m_Display = ReadBoolValue(lookingGlass, "display");
			if (lookingGlass.contains("bridgePath"))
				values.m_BridgePath = ReadStringValue(lookingGlass, "bridgePath");
		}
		if (hardware.contains("anaglyph"))
		{
			const auto& anaglyph = hardware["anaglyph"];
			if (anaglyph.contains("leftColour"))
				hardwareValues.m_Anaglyph.m_LeftColour = ReadStringValue(anaglyph, "leftColour");
			if (anaglyph.contains("rightColour"))
				hardwareValues.m_Anaglyph.m_RightColour = ReadStringValue(anaglyph, "rightColour");
			if (anaglyph.contains("ghostReduction"))
				hardwareValues.m_Anaglyph.m_GhostReduction = ReadFloatValue(anaglyph, "ghostReduction");
		}
	}
	return hardwareValues;
}

Config::Settings::InGameUi Config::GetInGameUi(const json& graphics)
{
	Config::Settings::InGameUi inGameUiValues;
	if (graphics.contains("inGameUi"))
	{
		const auto& inGameUi = graphics["inGameUi"];
		inGameUiValues.m_TopBar = GetUiElement(inGameUi, "topBar", inGameUiValues.m_TopBar);
		inGameUiValues.m_SpellIcons = GetUiElement(inGameUi, "spellIcons", inGameUiValues.m_SpellIcons);
		inGameUiValues.m_SpellMenu = GetUiElement(inGameUi, "spellMenu", inGameUiValues.m_SpellMenu);
	}
	return inGameUiValues;
}

Config::Settings::UiElement Config::GetUiElement(const json& inGameUi, const char* name, const Config::Settings::UiElement& defaults)
{
	Config::Settings::UiElement elementValues = defaults;
	if (inGameUi.contains(name) && inGameUi[name].is_object())
	{
		const auto& element = inGameUi[name];
		if (element.contains("alignment") && element["alignment"].is_string())
			elementValues.m_Alignment = ReadStringValue(element, "alignment");
		elementValues.m_MarginX = ReadIntValue(element, "marginX");
		elementValues.m_MarginY = ReadIntValue(element, "marginY");
	}
	return elementValues;
}

Config::Settings::Sound Config::GetSound(const json& settings)
{
	Config::Settings::Sound soundValues;
	if (settings.contains("sound"))
	{
		const auto& sound = settings["sound"];
		soundValues.m_HqSound = ReadBoolValue(sound, "hqSound");
		soundValues.m_FixSpeedSound = ReadBoolValue(sound, "fixSpeedSound");
		soundValues.m_AutoShowObjectivesForForeignLanguages = ReadBoolValue(sound, "autoShowObjectivesForForeignLanguages");
		soundValues.m_MaxSimultaniousSounds = ReadIntValue(sound, "maxSimultaniousSounds");
		soundValues.m_MusicType = ReadStringValue(sound, "musicType");
		soundValues.m_Paths = GetPaths(sound);
		soundValues.m_OggMusicAlternative = ReadBoolValue(sound, "oggMusicAlternative");
		soundValues.m_UseMagicCarpet1Music = ReadBoolValue(sound, "useMagicCarpet1Music");
		soundValues.m_Awe32GmIntroMusic = ReadBoolValue(sound, "awe32GmIntroMusic");
		soundValues.m_Awe32BufferMs = ReadIntValue(sound, "awe32BufferMs");
	}
	return soundValues;
}

std::string Config::Settings::Paths::Get(const std::string& name, const std::string& key) const
{
	for (const auto& entry : m_Entries)
		if (entry.m_Name == name)
			for (const auto& path : entry.m_Path)
				if (path.m_Key == key)
					return path.m_Value;
	return "";
}

bool Config::Settings::Paths::Has(const std::string& name, const std::string& key) const
{
	for (const auto& entry : m_Entries)
		if (entry.m_Name == name)
			for (const auto& path : entry.m_Path)
				if (path.m_Key == key)
					return true;
	return false;
}

void Config::Settings::Paths::Set(const std::string& name, const std::string& key, const std::string& value)
{
	auto entry = std::find_if(m_Entries.begin(), m_Entries.end(), [&name](const NamedPaths& e) { return e.m_Name == name; });
	if (entry == m_Entries.end())
	{
		m_Entries.push_back({ name, {} });
		entry = m_Entries.end() - 1;
	}
	for (auto& path : entry->m_Path)
	{
		if (path.m_Key == key)
		{
			path.m_Value = value;
			return;
		}
	}
	entry->m_Path.push_back({ key, value });
}

// "paths": [ { "name": "<game>", "path": [ { "key": "<key>", "value": "<value>" }, ... ] }, ... ]
Config::Settings::Paths Config::GetPaths(const json& parent)
{
	Config::Settings::Paths pathsValues;
	if (!parent.contains("paths") || !parent["paths"].is_array())
		return pathsValues;
	for (const auto& entry : parent["paths"])
	{
		if (!entry.is_object())
			continue;
		const std::string name = ReadStringValue(entry, "name");
		if (name.empty() || !entry.contains("path") || !entry["path"].is_array())
			continue;
		for (const auto& path : entry["path"])
		{
			if (path.is_object())
			{
				const std::string key = ReadStringValue(path, "key");
				if (!key.empty())
					pathsValues.Set(name, key, ReadStringValue(path, "value"));
			}
		}
	}
	return pathsValues;
}

Config::Settings::Paths Config::GetSettingsPaths(const json& settings)
{
	if (settings.contains("paths") && settings["paths"].is_array())
		return GetPaths(settings);

	// older configs
	Config::Settings::Paths pathsValues;
	const char* keys[] = { Settings::PathKey::GameFolder, Settings::PathKey::CdFolder };
	if (settings.contains("games") && settings["games"].is_object())
	{
		// version 2.0: "games": { "<game>": { "paths": { "gameFolder": ..., "cdFolder": ... } } }
		for (const auto& [name, game] : settings["games"].items())
			if (game.is_object() && game.contains("paths") && game["paths"].is_object())
				for (const char* key : keys)
					if (game["paths"].contains(key))
						pathsValues.Set(name, key, ReadStringValue(game["paths"], key));
	}
	else if (settings.contains("paths") && settings["paths"].is_object())
	{
		// version 1.0: "paths": { "gameFolder": ..., "cdFolder": ... } of Magic Carpet 2
		for (const char* key : keys)
			pathsValues.Set(Settings::GameName::MagicCarpet2, key, ReadStringValue(settings["paths"], key));
	}
	return pathsValues;
}

// older configs: the folders of Magic Carpet 2 were settings of sound and gameDetail (or of its "games" entry)
void Config::ReadOldFolders(const json& settings, Settings& values)
{
	const char* mc2 = Settings::GameName::MagicCarpet2;
	auto readOld = [&](Settings::Paths& paths, const json& section, const char* key) {
		if (!paths.Has(mc2, key) && section.contains(key) && section[key].is_string())
			paths.Set(mc2, key, section[key].get<std::string>());
	};

	if (settings.contains("sound"))
		for (const char* key : { Settings::PathKey::SpeechFolder, Settings::PathKey::OggFolder, Settings::PathKey::Awe32RomFolder })
			readOld(values.m_Sound.m_Paths, settings["sound"], key);

	const char* graphicsKeys[] = { Settings::PathKey::HighResGraphicsFolder, Settings::PathKey::FixedMenuGraphicsFolder,
		Settings::PathKey::FixedTmapsFolder, Settings::PathKey::ExtendedFontsFolder };
	auto& gameDetailPaths = values.m_Graphics.m_GameDetail.m_Paths;
	if (settings.contains("games") && settings["games"].contains(mc2) && settings["games"][mc2].contains("paths"))
		for (const char* key : graphicsKeys)
			readOld(gameDetailPaths, settings["games"][mc2]["paths"], key);
	if (settings.contains("graphics") && settings["graphics"].contains("gameDetail"))
		for (const char* key : graphicsKeys)
			readOld(gameDetailPaths, settings["graphics"]["gameDetail"], key);
}

std::string Config::ReadFileToString(std::string fileName)
{
	std::string jsonStr;

	try
	{
		std::ifstream is(fileName, std::ifstream::binary);
		std::string line;
		while (std::getline(is, line))
		{
			jsonStr += line;
			jsonStr.push_back('\n');
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Error Stopping Worker Thread: " << e.what();
	}
	return jsonStr;
}

void Config::SaveLaunchArgumentsToDoc(Config::Settings settings)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	SetString(settingsEntry, "launchArguments", settings.m_LaunchArguments);
}

void Config::SavePathsToDoc(const Config::Settings::Paths& paths)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	// the "games" of version 2.0 and the "paths" object of version 1.0 are replaced by the "paths" array
	settingsEntry.erase("games");
	if (settingsEntry.contains("paths") && !settingsEntry["paths"].is_array())
		settingsEntry.erase("paths");
	SetString(settingsEntry, "version", ConfigVersion);
	SetPaths(settingsEntry, paths);
}

void Config::SetPaths(json& parent, const Config::Settings::Paths& paths)
{
	json entries = json::array();
	for (const auto& namedPaths : paths.m_Entries)
	{
		json entry = json::object();
		entry["name"] = namedPaths.m_Name;
		json pathArray = json::array();
		for (const auto& path : namedPaths.m_Path)
		{
			json pathEntry = json::object();
			pathEntry["key"] = path.m_Key;
			pathEntry["value"] = path.m_Value;
			pathArray.push_back(pathEntry);
		}
		entry["path"] = pathArray;
		entries.push_back(entry);
	}
	parent["paths"] = entries;
}

void Config::SaveMultiplayerToDoc(Config::Settings::Multiplayer multiplayerSettings)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& multiplayer = GetOrCreate(settingsEntry, "multiplayer");

	SetInt(multiplayer, "serverPort", multiplayerSettings.m_ServerPort);

	SetInt(multiplayer, "clientPort", multiplayerSettings.m_ClientPort);
	SetInt(multiplayer, "clientServerPort", multiplayerSettings.m_ClientServerPort);
	SetString(multiplayer, "clientServerIp", multiplayerSettings.m_ClientServerIp);
	SetString(multiplayer, "recordPlayFile", multiplayerSettings.m_RecordPlayFile);
	SetBool(multiplayer, "debug", multiplayerSettings.m_Debug);
}

void Config::SaveSoundToDoc(Config::Settings::Sound soundSettings)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& sound = GetOrCreate(settingsEntry, "sound");
	SetPaths(sound, soundSettings.m_Paths);
	SetBool(sound, "hqSound", soundSettings.m_HqSound);
	SetBool(sound, "oggMusicAlternative", soundSettings.m_OggMusicAlternative);
	SetBool(sound, "fixSpeedSound", soundSettings.m_FixSpeedSound);
	SetBool(sound, "autoShowObjectivesForForeignLanguages", soundSettings.m_AutoShowObjectivesForForeignLanguages);
	SetInt(sound, "maxSimultaniousSounds", soundSettings.m_MaxSimultaniousSounds);
	SetString(sound, "musicType", soundSettings.m_MusicType);
	SetBool(sound, "useMagicCarpet1Music", soundSettings.m_UseMagicCarpet1Music);
	SetBool(sound, "awe32GmIntroMusic", soundSettings.m_Awe32GmIntroMusic);
	SetInt(sound, "awe32BufferMs", soundSettings.m_Awe32BufferMs);
	// the folders of older configs, now in the paths
	for (const char* key : { Settings::PathKey::SpeechFolder, Settings::PathKey::OggFolder, Settings::PathKey::Awe32RomFolder })
		sound.erase(key);
}

void Config::SaveGraphicsToDoc(Config::Settings::Graphics graphics)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& gfx = GetOrCreate(settingsEntry, "graphics");
	SetInt(gfx, "displayIndex", graphics.m_DisplayIndex);
	SetInt(gfx, "windowResWidth", graphics.m_WindowResWidth);
	SetInt(gfx, "windowResHeight", graphics.m_WindowResHeight);
	SetBool(gfx, "maintainAspectRatio", graphics.m_MaintainAspectRatio);
	SetBool(gfx, "startWindowed", graphics.m_StartWindowed);
	SetString(gfx, "render", graphics.m_Render);
	SaveGameDetailToDoc(graphics.m_GameDetail);
	SaveThreadingToDoc(graphics.m_Threading);
	SaveHardwareToDoc(graphics.m_Hardware);
	SaveInGameUiToDoc(graphics.m_InGameUi);
}

void Config::SaveGameDetailToDoc(Config::Settings::GameDetail gameDetail)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& gfx = GetOrCreate(settingsEntry, "graphics");
	auto& gd = GetOrCreate(gfx, "gameDetail");
	SetPaths(gd, gameDetail.m_Paths);
	SetInt(gd, "gameResWidth", gameDetail.m_GameResWidth);
	SetInt(gd, "gameResHeight", gameDetail.m_GameResHeight);
	SetInt(gd, "gameUiScale", gameDetail.m_GameUiScale);
	SetInt(gd, "mapEntityScale", gameDetail.m_MapEntityScale);
	SetBool(gd, "useHighResGraphics", gameDetail.m_UseHighResGraphics);
	SetBool(gd, "useFixedMenuGraphics", gameDetail.m_UseFixedMenuGraphics);
	SetBool(gd, "useFixedTmaps", gameDetail.m_UseFixedTmaps);
	SetBool(gd, "useExtendedFonts", gameDetail.m_UseExtendedFonts);
	SetBool(gd, "sky", gameDetail.m_Sky);
	SetBool(gd, "reflections", gameDetail.m_Reflections);
	SetBool(gd, "dynamicLighting", gameDetail.m_DynamicLighting);
	SetInt(gd, "viewDistanceScale", gameDetail.m_ViewDistanceScale);
	SetFloat(gd, "fogScale", gameDetail.m_FogScale);
	// the folders of older configs, now in the paths
	for (const char* key : { Settings::PathKey::HighResGraphicsFolder, Settings::PathKey::FixedMenuGraphicsFolder,
		Settings::PathKey::FixedTmapsFolder, Settings::PathKey::ExtendedFontsFolder })
		gd.erase(key);
}

void Config::SaveThreadingToDoc(Config::Settings::Threading threading)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& gfx = GetOrCreate(settingsEntry, "graphics");
	auto& t = GetOrCreate(gfx, "threading");
	SetBool(t, "isActive", threading.m_IsActive);
	SetFloat(t, "sizePercentToThreadRender", threading.m_SizePercentToThreadRender);
	SetFloat(t, "sizePercentToThreadRenderSprites", threading.m_SizePercentToThreadRenderSprites);
	SetInt(t, "numberOfRenderThreads", threading.m_NumberOfRenderThreads);
	SetBool(t, "assignToSpecificCores", threading.m_AssignToSpecificCores);
}

void Config::SaveHardwareToDoc(Config::Settings::Hardware hardware)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& gfx = GetOrCreate(settingsEntry, "graphics");
	auto& h = GetOrCreate(gfx, "hardware");
	SetBool(h, "bilinearFiltering", hardware.m_BilinearFiltering);
	SetInt(h, "antiAliasing", hardware.m_AntiAliasing);
	auto& lg = GetOrCreate(h, "lookingGlass");
	SetBool(lg, "enabled", hardware.m_LookingGlass.m_Enabled);
	SetInt(lg, "rows", hardware.m_LookingGlass.m_Rows);
	SetInt(lg, "columns", hardware.m_LookingGlass.m_Columns);
	SetInt(lg, "cellWidth", hardware.m_LookingGlass.m_CellWidth);
	SetInt(lg, "cellHeight", hardware.m_LookingGlass.m_CellHeight);
	SetInt(lg, "viewSpacing", hardware.m_LookingGlass.m_ViewSpacing);
	SetInt(lg, "convergence", hardware.m_LookingGlass.m_Convergence);
	SetBool(lg, "display", hardware.m_LookingGlass.m_Display);
	SetString(lg, "bridgePath", hardware.m_LookingGlass.m_BridgePath);
	auto& anaglyph = GetOrCreate(h, "anaglyph");
	SetString(anaglyph, "leftColour", hardware.m_Anaglyph.m_LeftColour);
	SetString(anaglyph, "rightColour", hardware.m_Anaglyph.m_RightColour);
	SetFloat(anaglyph, "ghostReduction", hardware.m_Anaglyph.m_GhostReduction);
}

void Config::SaveInGameUiToDoc(Config::Settings::InGameUi inGameUi)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& gfx = GetOrCreate(settingsEntry, "graphics");
	auto& ui = GetOrCreate(gfx, "inGameUi");
	const std::pair<const char*, const Settings::UiElement*> elements[] = {
		{ "topBar", &inGameUi.m_TopBar },
		{ "spellIcons", &inGameUi.m_SpellIcons },
		{ "spellMenu", &inGameUi.m_SpellMenu } };
	for (const auto& [name, element] : elements)
	{
		auto& e = GetOrCreate(ui, name);
		SetString(e, "alignment", element->m_Alignment);
		SetInt(e, "marginX", element->m_MarginX);
		SetInt(e, "marginY", element->m_MarginY);
	}
}

void Config::SaveGameToDoc(Config::Settings::Game gameSettings)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& game = GetOrCreate(settingsEntry, "game");
	SetInt(game, "maxGameFps", gameSettings.m_MaxGameFps);
	SetBool(game, "skipIntro", gameSettings.m_SkipIntro);
}

void Config::SaveControlsToDoc(Config::Settings::Controls controlSettings)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& controls = GetOrCreate(settingsEntry, "controls");

	if (!controls.contains("mouse"))
		controls["mouse"] = json::array({ json::object() });
	auto& mouse = controls["mouse"][0];
	SetBool(mouse, "isActive", true);
	SetBool(mouse, "invertXAxis", controlSettings.m_Mouse.m_InvertXAxis);
	SetBool(mouse, "invertYAxis", controlSettings.m_Mouse.m_InvertYAxis);
	SetFloat(mouse, "mouseScaleX", controlSettings.m_Mouse.m_MouseScaleX);
	SetFloat(mouse, "mouseScaleY", controlSettings.m_Mouse.m_MouseScaleY);
	SetBool(mouse, "disableLRButtonsMenuOpen", controlSettings.m_Mouse.m_DisableLRButtonsMenuOpen);
	SetInt(mouse, "spellLeft", controlSettings.m_Mouse.m_SpellLeft);
	SetInt(mouse, "spellRight", controlSettings.m_Mouse.m_SpellRight);
	SetInt(mouse, "map", controlSettings.m_Mouse.m_Map);
	SetInt(mouse, "spellMenu", controlSettings.m_Mouse.m_SpellMenu);
	SetInt(mouse, "spellMenuMark", controlSettings.m_Mouse.m_SpellMenuMark);

	if (!controls.contains("keyboard"))
		controls["keyboard"] = json::array({ json::object() });
	auto& keyboard = controls["keyboard"][0];
	SetBool(keyboard, "isActive", true);
	SetString(keyboard, "forward", m_ConfigToSdlScancode.GetName(controlSettings.m_Keyboard.m_Forward));
	SetString(keyboard, "backwards", m_ConfigToSdlScancode.GetName(controlSettings.m_Keyboard.m_Backwards));
	SetString(keyboard, "left", m_ConfigToSdlScancode.GetName(controlSettings.m_Keyboard.m_Left));
	SetString(keyboard, "right", m_ConfigToSdlScancode.GetName(controlSettings.m_Keyboard.m_Right));
	SetString(keyboard, "map", m_ConfigToSdlScancode.GetName(controlSettings.m_Keyboard.m_Map));
	SetString(keyboard, "spellMenu", m_ConfigToSdlScancode.GetName(controlSettings.m_Keyboard.m_SpellMenu));
	SetString(keyboard, "spellMenuMark", m_ConfigToSdlScancode.GetName(controlSettings.m_Keyboard.m_SpellMenuMark));
	SetBool(keyboard, "disableLRKeyRoll", controlSettings.m_Keyboard.m_DisableLRKeyRoll);

	if (!controls.contains("gamePad"))
		controls["gamePad"] = json::array({ json::object() });
	auto& gamePad = controls["gamePad"][0];
	SetBool(gamePad, "isActive", true);
	SetInt(gamePad, "controllerId", controlSettings.m_GamePad.m_ControllerId);
	SetInt(gamePad, "buttonMiniMap", controlSettings.m_GamePad.m_ButtonMiniMap);
	SetInt(gamePad, "buttonSpell", controlSettings.m_GamePad.m_ButtonSpell);
	SetInt(gamePad, "buttonPauseMenu", controlSettings.m_GamePad.m_ButtonPauseMenu);
	SetInt(gamePad, "buttonEsc", controlSettings.m_GamePad.m_ButtonEsc);
	SetInt(gamePad, "buttonFireL", controlSettings.m_GamePad.m_ButtonFireL);
	SetInt(gamePad, "buttonFireR", controlSettings.m_GamePad.m_ButtonFireR);
	SetInt(gamePad, "buttonMenuSelect", controlSettings.m_GamePad.m_ButtonMenuSelect);
	SetInt(gamePad, "triggerDeadZone", controlSettings.m_GamePad.m_TriggerDeadZone);
	SetBool(gamePad, "hapticEnabled", controlSettings.m_GamePad.m_HapticEnabled);
	SetInt(gamePad, "hapticMaxGain", controlSettings.m_GamePad.m_HapticMaxGain);
	SetInt(gamePad, "hatNav", controlSettings.m_GamePad.m_HatNav);
	SetInt(gamePad, "hatMov", controlSettings.m_GamePad.m_HatMov);
	SetBool(gamePad, "hatNavInv", controlSettings.m_GamePad.m_HatNavInv);
	SetBool(gamePad, "hatMovInv", controlSettings.m_GamePad.m_HatMovInv);
	SetInt(gamePad, "axisLong", controlSettings.m_GamePad.m_AxisLong);
	SetInt(gamePad, "axisLongDeadZone", controlSettings.m_GamePad.m_AxisLongDeadZone);
	SetInt(gamePad, "axisLongNavDeadZone", controlSettings.m_GamePad.m_AxisLongNavDeadZone);
	SetBool(gamePad, "axisLongInv", controlSettings.m_GamePad.m_AxisLongInv);
	SetInt(gamePad, "axisTrans", controlSettings.m_GamePad.m_AxisTrans);
	SetBool(gamePad, "axisTransInv", controlSettings.m_GamePad.m_AxisTransInv);
	SetInt(gamePad, "axisTransDeadZone", controlSettings.m_GamePad.m_AxisTransDeadZone);
	SetInt(gamePad, "axisTransNavDeadZone", controlSettings.m_GamePad.m_AxisTransNavDeadZone);
	SetInt(gamePad, "axisNavNs", controlSettings.m_GamePad.m_AxisNavNs);
	SetBool(gamePad, "axisNavNsInv", controlSettings.m_GamePad.m_AxisNavNsInv);
	SetInt(gamePad, "axisNavEw", controlSettings.m_GamePad.m_AxisNavEw);
	SetBool(gamePad, "axisNavEwInv", controlSettings.m_GamePad.m_AxisNavEwInv);
	SetInt(gamePad, "axisFireR", controlSettings.m_GamePad.m_AxisFireR);
	SetInt(gamePad, "axisFireL", controlSettings.m_GamePad.m_AxisFireL);
	SetInt(gamePad, "axisYaw", controlSettings.m_GamePad.m_AxisYaw);
	SetBool(gamePad, "axisYawInv", controlSettings.m_GamePad.m_AxisYawInv);
	SetInt(gamePad, "axisYawDeadZone", controlSettings.m_GamePad.m_AxisYawDeadZone);
	SetInt(gamePad, "axisPitch", controlSettings.m_GamePad.m_AxisPitch);
	SetBool(gamePad, "axisPitchInv", controlSettings.m_GamePad.m_AxisPitchInv);
	SetInt(gamePad, "axisPitchDeadZone", controlSettings.m_GamePad.m_AxisPitchDeadZone);
	SetZones(gamePad, "axisYawSensitivity", controlSettings.m_GamePad.m_AxisYawSensitivity);
	SetZones(gamePad, "axisPitchSensitivity", controlSettings.m_GamePad.m_AxisPitchSensitivity);
}

json& Config::GetOrCreateActiveSettingsEntry()
{
	auto& settingsArray = m_Document["settings"];

	for (auto& entry : settingsArray)
	{
		if (entry.contains("name") && entry.contains("isActive") && entry["isActive"].get<bool>() == true)
		{
			return entry;
		}
	}
}

bool Config::SaveToFile()
{
	return SaveToFile(m_FileName);
}

bool Config::SaveToFile(std::string fileName)
{
	std::ofstream f(fileName);
	if (!f.is_open()) return false;

	f << m_Document.dump(1, '\t');
	return true;
}
