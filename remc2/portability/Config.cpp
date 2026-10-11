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
			settings.m_Paths = GetPaths(entry);
			settings.m_Multiplayer = GetMultiplayer(entry);
			settings.m_Sound = GetSound(entry);
			settings.m_Graphics = GetGraphics(entry);
			settings.m_Game = GetGame(entry);
			settings.m_Controls = GetControls(entry);
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

		graphicsValues.m_GameDetail = GetGameDetail(graphics);
		graphicsValues.m_Threading = GetThreading(graphics);
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
		gameDetailValues.m_UseHighResGraphics = ReadBoolValue(gameDetail, "useHighResGraphics");
		gameDetailValues.m_HighResGraphicsFolder = ReadStringValue(gameDetail, "highResGraphicsFolder");
		gameDetailValues.m_UseFixedMenuGraphics = ReadBoolValue(gameDetail, "useFixedMenuGraphics");
		gameDetailValues.m_FixedMenuGraphicsFolder = ReadStringValue(gameDetail, "fixedMenuGraphicsFolder");
		gameDetailValues.m_UseExtendedFonts = ReadBoolValue(gameDetail, "useExtendedFonts");
		gameDetailValues.m_ExtendedFontsFolder = ReadStringValue(gameDetail, "extendedFontsFolder");
		gameDetailValues.m_Sky = ReadBoolValue(gameDetail, "sky");
		gameDetailValues.m_Reflections = ReadBoolValue(gameDetail, "reflections");
		gameDetailValues.m_DynamicLighting = ReadBoolValue(gameDetail, "dynamicLighting");
		gameDetailValues.m_ViewDistanceScale = ReadIntValue(gameDetail, "viewDistanceScale");
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
		threadingValues.m_NumberOfRenderThreads = (uint8_t)ReadIntValue(threading, "numberOfRenderThreads");
		threadingValues.m_AssignToSpecificCores = ReadBoolValue(threading, "assignToSpecificCores");
	}
	return threadingValues;
}

Config::Settings::Sound Config::GetSound(const json& settings)
{
	Config::Settings::Sound soundValues;
	if (settings.contains("sound"))
	{
		const auto& sound = settings["sound"];
		soundValues.m_HqSound = ReadBoolValue(sound, "hqSound");
		soundValues.m_OggMusic = ReadBoolValue(sound, "oggMusic");
		soundValues.m_OggFolder = ReadStringValue(sound, "oggFolder");
		soundValues.m_OggMusicAlternative = ReadBoolValue(sound, "oggMusicAlternative");
		soundValues.m_FixSpeedSound = ReadBoolValue(sound, "fixSpeedSound");
		soundValues.m_AutoShowObjectivesForForeignLanguages = ReadBoolValue(sound, "autoShowObjectivesForForeignLanguages");
		soundValues.m_MaxSimultaniousSounds = ReadIntValue(sound, "maxSimultaniousSounds");
		soundValues.m_SpeechFolder = ReadStringValue(sound, "speechFolder");
	}
	return soundValues;
}

Config::Settings::Paths Config::GetPaths(const json& settings)
{
	Config::Settings::Paths pathValues;
	if (settings.contains("paths"))
	{
		const auto& paths = settings["paths"];
		pathValues.m_GameFolder = ReadStringValue(paths, "gameFolder");
		pathValues.m_CdFolder = ReadStringValue(paths, "cdFolder");
	}
	return pathValues;
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

void Config::SavePathsToDoc(Config::Settings::Paths pathSettings)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& paths = GetOrCreate(settingsEntry, "paths");
	SetString(paths, "gameFolder", pathSettings.m_GameFolder);
	SetString(paths, "cdFolder", pathSettings.m_CdFolder);
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
	SetBool(sound, "hqSound", soundSettings.m_HqSound);
	SetBool(sound, "oggMusic", soundSettings.m_OggMusic);
	SetString(sound, "oggFolder", soundSettings.m_OggFolder);
	SetBool(sound, "oggMusicAlternative", soundSettings.m_OggMusicAlternative);
	SetBool(sound, "fixSpeedSound", soundSettings.m_FixSpeedSound);
	SetBool(sound, "autoShowObjectivesForForeignLanguages", soundSettings.m_AutoShowObjectivesForForeignLanguages);
	SetInt(sound, "maxSimultaniousSounds", soundSettings.m_MaxSimultaniousSounds);
	SetString(sound, "speechFolder", soundSettings.m_SpeechFolder);
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
	SaveGameDetailToDoc(graphics.m_GameDetail);
	SaveThreadingToDoc(graphics.m_Threading);
}

void Config::SaveGameDetailToDoc(Config::Settings::GameDetail gameDetail)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& gfx = GetOrCreate(settingsEntry, "graphics");
	auto& gd = GetOrCreate(gfx, "gameDetail");
	SetInt(gd, "gameResWidth", gameDetail.m_GameResWidth);
	SetInt(gd, "gameResHeight", gameDetail.m_GameResHeight);
	SetInt(gd, "gameUiScale", gameDetail.m_GameUiScale);
	SetBool(gd, "useHighResGraphics", gameDetail.m_UseHighResGraphics);
	SetString(gd, "highResGraphicsFolder", gameDetail.m_HighResGraphicsFolder);
	SetBool(gd, "useFixedMenuGraphics", gameDetail.m_UseFixedMenuGraphics);
	SetString(gd, "fixedMenuGraphicsFolder", gameDetail.m_FixedMenuGraphicsFolder);
	SetBool(gd, "useExtendedFonts", gameDetail.m_UseExtendedFonts);
	SetString(gd, "extendedFontsFolder", gameDetail.m_ExtendedFontsFolder);
	SetBool(gd, "sky", gameDetail.m_Sky);
	SetBool(gd, "reflections", gameDetail.m_Reflections);
	SetBool(gd, "dynamicLighting", gameDetail.m_DynamicLighting);
	SetInt(gd, "viewDistanceScale", gameDetail.m_ViewDistanceScale);
}

void Config::SaveThreadingToDoc(Config::Settings::Threading threading)
{
	auto& settingsEntry = GetOrCreateActiveSettingsEntry();
	auto& gfx = GetOrCreate(settingsEntry, "graphics");
	auto& t = GetOrCreate(gfx, "threading");
	SetBool(t, "isActive", threading.m_IsActive);
	SetFloat(t, "sizePercentToThreadRender", threading.m_SizePercentToThreadRender);
	SetInt(t, "numberOfRenderThreads", threading.m_NumberOfRenderThreads);
	SetBool(t, "assignToSpecificCores", threading.m_AssignToSpecificCores);
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
