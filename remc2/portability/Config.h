#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../utilities/Maths.h"
#include "ConfigToSdlScancodeMapping.h"

using json = nlohmann::ordered_json;

class Config
{
private:
	ConfigToSdlScancode m_ConfigToSdlScancode;
	std::string m_FileName = "";
	json m_Document;

	std::string ReadStringValue(const json& settings, const char* name);
	int ReadIntValue(const json& settings, const char* name);
	float ReadFloatValue(const json& settings, const char* name);
	bool ReadBoolValue(const json& settings, const char* name);
	SDL_Scancode ReadKeyScancode(const json& settings, const char* name);
	std::string ReadFileToString(std::string fileName);
	void SetString(json& obj, const char* key, const std::string& value);
	void SetInt(json& obj, const char* key, int value);
	void SetFloat(json& obj, const char* key, float value);
	void SetBool(json& obj, const char* key, bool value);
	void SetZones(json& obj, const char* key,const std::vector<Maths::Zone>& zones);
	json& GetOrCreateActiveSettingsEntry();
	json& GetOrCreate(json& parent, const char* key);

public:

	struct Settings
	{
		struct Paths
		{
			std::string m_GameFolder = "";
			std::string m_CdFolder = "";
		};

		struct Multiplayer
		{
			int m_ServerPort = 3030;
			int m_ClientPort = 3030;
			int m_ClientServerPort = 3030;
			std::string m_ClientServerIp = "127.0.0.1";
			std::string m_RecordPlayFile = "";
			bool m_Debug = false;
		};

		struct Sound
		{
			bool m_HqSound = true;
			bool m_OggMusic = true;
			std::string m_OggFolder;
			bool m_OggMusicAlternative = false;
			bool m_FixSpeedSound = true;
			bool m_AutoShowObjectivesForForeignLanguages = false;
			int m_MaxSimultaniousSounds = 10;
			std::string m_SpeechFolder;
		};

		struct GameDetail
		{
			int m_GameResWidth = 640;
			int m_GameResHeight = 480;
			int m_GameUiScale = 1;
			bool m_UseHighResGraphics = false;
			std::string m_HighResGraphicsFolder = "";
			bool m_UseFixedMenuGraphics = false;
			std::string m_FixedMenuGraphicsFolder = "";
			bool m_UseExtendedFonts = false;
			std::string m_ExtendedFontsFolder = "";
			bool m_Sky = true;
			bool m_Reflections = true;
			bool m_DynamicLighting = true;
			int m_ViewDistanceScale = 2;
		};

		struct Threading
		{
			bool m_IsActive = false;
			float m_SizePercentToThreadRender = 10.0;
			int m_NumberOfRenderThreads = 0;
			bool m_AssignToSpecificCores = false;
		};

		struct Graphics
		{
			int m_DisplayIndex = 0;
			int m_WindowResWidth = 640;
			int m_WindowResHeight = 480;
			bool m_MaintainAspectRatio = true;
			bool m_StartWindowed = false;
			GameDetail m_GameDetail;
			Threading m_Threading;
		};

		struct Game
		{
			int m_MaxGameFps = 0;
			bool m_SkipIntro = false;
		};

		struct Mouse
		{
			std::string m_Name = "";
			bool m_InvertYAxis = true;
			bool m_InvertXAxis = false;
			float m_MouseScaleX = 1.0f;
			float m_MouseScaleY = 1.0f;
			bool m_DisableLRButtonsMenuOpen = false;
			uint16_t m_SpellLeft = 1;
			uint16_t m_SpellRight = 2;
			uint16_t m_Map = 0;
			uint16_t m_SpellMenu = 0;
			uint16_t m_SpellMenuMark = 0;
		};

		struct Keyboard
		{
			std::string m_Name = "";
			uint16_t m_Forward = 0;
			uint16_t m_Backwards = 0;
			uint16_t m_Left = 0;
			uint16_t m_Right = 0;
			uint16_t m_Map = 0;
			uint16_t m_SpellMenu = 0;
			uint16_t m_SpellMenuMark = 0;
		};

		struct GamePad
		{
			std::string m_Name = "";
			uint16_t m_ControllerId = 0;
			uint16_t m_ButtonMiniMap = 0;
			uint16_t m_ButtonSpell = 0;
			uint16_t m_ButtonPauseMenu = 0;
			uint16_t m_ButtonEsc = 0;
			uint16_t m_ButtonFireL = 0;
			uint16_t m_ButtonFireR = 0;
			uint16_t m_ButtonFwd = 0;
			uint16_t m_ButtonBack = 0;
			uint16_t m_ButtonMenuSelect = 0;
			uint16_t m_TriggerDeadZone = 3000;
			bool m_HapticEnabled = false;
			uint16_t m_HapticMaxGain = 75;
			uint16_t m_HatNav = 0;
			uint16_t m_HatMov = 0;
			bool m_HatNavInv = false;
			bool m_HatMovInv = false;
			uint16_t m_AxisLong = 0;
			uint16_t m_AxisLongDeadZone = 3000;
			uint16_t m_AxisLongNavDeadZone = 6000;
			bool m_AxisLongInv = false;
			uint16_t m_AxisTrans = 0;
			bool m_AxisTransInv = false;
			uint16_t m_AxisTransDeadZone = 3000;
			uint16_t m_AxisTransNavDeadZone = 6000;
			uint16_t m_AxisNavNs = 0;
			bool m_AxisNavNsInv = false;
			uint16_t m_AxisNavEw = 0;
			bool m_AxisNavEwInv = false;
			uint16_t m_AxisFireR = 0;
			uint16_t m_AxisFireL = 0;
			uint16_t m_AxisYaw = 0;
			bool m_AxisYawInv = false;
			uint16_t m_AxisYawDeadZone = 3000;
			std::vector<Maths::Zone> m_AxisYawSensitivity;
			uint16_t m_AxisPitch = 0;
			bool m_AxisPitchInv = false;
			uint16_t m_AxisPitchDeadZone = 3000;
			std::vector<Maths::Zone> m_AxisPitchSensitivity;
		};

		struct Controls
		{
			Mouse m_Mouse;
			Keyboard m_Keyboard;
			GamePad m_GamePad;
		};

		std::string m_Name;
		std::string m_Version;
		std::string m_LaunchArguments;
		Paths m_Paths;
		Multiplayer m_Multiplayer;
		Sound m_Sound;
		Graphics m_Graphics;
		Game m_Game;
		Controls m_Controls;
	};

	Config(std::string fileName);
	bool LoadFromFile();
	bool LoadFromFile(std::string fileName);
	bool SaveToFile();
	bool SaveToFile(std::string fileName);

    Settings GetSettingsFromDoc();
	Settings GetSettings(json& document);
	Settings::Graphics GetGraphics(const json& settings);
	Settings::GameDetail GetGameDetail(const json& graphics);
	Settings::Threading GetThreading(const json& graphics);
	Settings::Sound GetSound(const json& settings);
	Settings::Paths GetPaths(const json& settings);
	Settings::Controls GetControls(const json& settings);
	Settings::Multiplayer GetMultiplayer(const json& settings);
	Settings::Game GetGame(const json& settings);

	void SaveLaunchArgumentsToDoc(Config::Settings settings);
	void SaveSoundToDoc(Settings::Sound soundSettings);
	void SavePathsToDoc(Settings::Paths pathSettings);
	void SaveMultiplayerToDoc(Settings::Multiplayer multiplayerSettings);
	void SaveGraphicsToDoc(Settings::Graphics graphics);
	void SaveGameDetailToDoc(Settings::GameDetail gameDetail);
	void SaveThreadingToDoc(Settings::Threading threading);
	void SaveGameToDoc(Settings::Game gameSettings);
	void SaveControlsToDoc(Settings::Controls controlSettings);
};

#endif //CONFIG_H