#pragma once
#include <map>
#include <string>
#include <SDL2/SDL_scancode.h>

class ConfigToSdlScancode
{
private:
	std::map<SDL_Scancode, std::string> m_SdlScancodeToConfigMap;

public:
	ConfigToSdlScancode()
	{
		m_SdlScancodeToConfigMap[SDL_SCANCODE_UNKNOWN] = "";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_A] = "A";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_B] = "B";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_C] = "C";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_D] = "D";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_E] = "E";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F] = "F";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_G] = "G";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_H] = "H";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_I] = "I";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_J] = "J";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_K] = "K";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_L] = "L";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_M] = "M";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_N] = "N";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_O] = "O";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_P] = "P";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_Q] = "Q";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_R] = "R";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_S] = "S";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_T] = "T";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_U] = "U";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_V] = "V";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_W] = "W";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_X] = "X";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_Y] = "Y";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_Z] = "Z";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_1] = "1";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_2] = "2";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_3] = "3";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_4] = "4";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_5] = "5";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_6] = "6";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_7] = "7";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_8] = "8";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_9] = "9";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_0] = "0";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_RETURN] = "RETURN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_ESCAPE] = "ESCAPE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_BACKSPACE] = "BACKSPACE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_TAB] = "TAB";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_SPACE] = "SPACE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_MINUS] = "MINUS";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_EQUALS] = "EQUALS";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LEFTBRACKET] = "LEFTBRACKET";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_RIGHTBRACKET] = "RIGHTBRACKET";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_BACKSLASH] = "BACKSLASH";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_NONUSHASH] = "NONUSHASH";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_SEMICOLON] = "SEMICOLON";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_APOSTROPHE] = "APOSTROPHE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_GRAVE] = "GRAVE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_COMMA] = "COMMA";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_PERIOD] = "PERIOD";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_SLASH] = "SLASH";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CAPSLOCK] = "CAPSLOCK";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_F1] = "F1";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F2] = "F2";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F3] = "F3";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F4] = "F4";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F5] = "F5";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F6] = "F6";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F7] = "F7";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F8] = "F8";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F9] = "F9";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F10] = "F10";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F11] = "F11";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F12] = "F12";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_PRINTSCREEN] = "PRINTSCREEN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_SCROLLLOCK] = "SCROLLLOCK";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_PAUSE] = "PAUSE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INSERT] = "INSERT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_HOME] = "HOME";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_PAGEUP] = "PAGEUP";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_DELETE] = "DELETE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_END] = "END";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_PAGEDOWN] = "PAGEDOWN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_RIGHT] = "RIGHT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LEFT] = "LEFT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_DOWN] = "DOWN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_UP] = "UP";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_NUMLOCKCLEAR] = "NUMLOCKCLEAR";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_DIVIDE] = "KP_DIVIDE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MULTIPLY] = "KP_MULTIPLY";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MINUS] = "KP_MINUS";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_PLUS] = "KP_PLUS";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_ENTER] = "KP_ENTER";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_1] = "KP_1";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_2] = "KP_2";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_3] = "KP_3";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_4] = "KP_4";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_5] = "KP_5";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_6] = "KP_6";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_7] = "KP_7";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_8] = "KP_8";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_9] = "KP_9";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_0] = "KP_0";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_PERIOD] = "KP_PERIOD";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_NONUSBACKSLASH] = "NONUSBACKSLASH";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_APPLICATION] = "APPLICATION";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_POWER] = "POWER";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_EQUALS] = "KP_EQUALS";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F13] = "F13";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F14] = "F14";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F15] = "F15";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F16] = "F16";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F17] = "F17";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F18] = "F18";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F19] = "F19";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F20] = "F20";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F21] = "F21";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F22] = "F22";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F23] = "F23";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_F24] = "F24";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_EXECUTE] = "EXECUTE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_HELP] = "HELP";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_MENU] = "MENU";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_SELECT] = "SELECT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_STOP] = "STOP";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_AGAIN] = "AGAIN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_UNDO] = "UNDO";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CUT] = "CUT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_COPY] = "COPY";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_PASTE] = "PASTE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_FIND] = "FIND";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_MUTE] = "MUTE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_VOLUMEUP] = "VOLUMEUP";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_VOLUMEDOWN] = "VOLUMEDOWN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_COMMA] = "KP_COMMA";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_EQUALSAS400] = "KP_EQUALSAS400";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL1] = "INTERNATIONAL1";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL2] = "INTERNATIONAL2";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL3] = "INTERNATIONAL3";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL4] = "INTERNATIONAL4";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL5] = "INTERNATIONAL5";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL6] = "INTERNATIONAL6";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL7] = "INTERNATIONAL7";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL8] = "INTERNATIONAL8";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_INTERNATIONAL9] = "INTERNATIONAL9";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG1] = "LANG1";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG2] = "LANG2";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG3] = "LANG3";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG4] = "LANG4";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG5] = "LANG5";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG6] = "LANG6";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG7] = "LANG7";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG8] = "LANG8";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LANG9] = "LANG9";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_ALTERASE] = "ALTERASE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_SYSREQ] = "SYSREQ";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CANCEL] = "CANCEL";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CLEAR] = "CLEAR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_PRIOR] = "PRIOR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_RETURN2] = "RETURN2";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_SEPARATOR] = "SEPARATOR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_OUT] = "OUT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_OPER] = "OPER";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CLEARAGAIN] = "CLEARAGAIN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CRSEL] = "CRSEL";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_EXSEL] = "EXSEL";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_00] = "KP_00";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_000] = "KP_000";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_THOUSANDSSEPARATOR] = "THOUSANDSSEPARATOR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_DECIMALSEPARATOR] = "DECIMALSEPARATOR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CURRENCYUNIT] = "CURRENCYUNIT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_CURRENCYSUBUNIT] = "CURRENCYSUBUNIT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_LEFTPAREN] = "KP_LEFTPAREN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_RIGHTPAREN] = "KP_RIGHTPAREN";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_LEFTBRACE] = "KP_LEFTBRACE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_RIGHTBRACE] = "KP_RIGHTBRACE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_TAB] = "KP_TAB";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_BACKSPACE] = "KP_BACKSPACE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_A] = "KP_A";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_B] = "KP_B";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_C] = "KP_C";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_D] = "KP_D";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_E] = "KP_E";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_F] = "KP_F";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_XOR] = "KP_XOR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_POWER] = "KP_POWER";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_PERCENT] = "KP_PERCENT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_LESS] = "KP_LESS";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_GREATER] = "KP_GREATER";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_AMPERSAND] = "KP_AMPERSAND";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_DBLAMPERSAND] = "KP_DBLAMPERSAND";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_VERTICALBAR] = "KP_VERTICALBAR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_DBLVERTICALBAR] = "KP_DBLVERTICALBAR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_COLON] = "KP_COLON";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_HASH] = "KP_HASH";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_SPACE] = "KP_SPACE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_AT] = "KP_AT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_EXCLAM] = "KP_EXCLAM";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MEMSTORE] = "KP_MEMSTORE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MEMRECALL] = "KP_MEMRECALL";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MEMCLEAR] = "KP_MEMCLEAR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MEMADD] = "KP_MEMADD";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MEMSUBTRACT] = "KP_MEMSUBTRACT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MEMMULTIPLY] = "KP_MEMMULTIPLY";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_MEMDIVIDE] = "KP_MEMDIVIDE";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_PLUSMINUS] = "KP_PLUSMINUS";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_CLEAR] = "KP_CLEAR";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_CLEARENTRY] = "KP_CLEARENTRY";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_BINARY] = "KP_BINARY";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_OCTAL] = "KP_OCTAL";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_DECIMAL] = "KP_DECIMAL";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_KP_HEXADECIMAL] = "KP_HEXADECIMAL";

		m_SdlScancodeToConfigMap[SDL_SCANCODE_LCTRL] = "LCTRL";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LSHIFT] = "LSHIFT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LALT] = "LALT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_LGUI] = "LGUI";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_RCTRL] = "RCTRL";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_RSHIFT] = "RSHIFT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_RALT] = "RALT";
		m_SdlScancodeToConfigMap[SDL_SCANCODE_RGUI] = "RGUI";
	}

	std::map<SDL_Scancode, std::string> GetScancodeMap()
	{
		return m_SdlScancodeToConfigMap;
	}

	SDL_Scancode GetScancode(std::string text)
	{
		for (auto const& [key, val] : m_SdlScancodeToConfigMap)
		{
			if (val == text)
				return key;
		}
		return SDL_SCANCODE_UNKNOWN;
	}

	std::string GetName(uint16_t scanCode)
	{
		return GetName((SDL_Scancode)scanCode);
	}

	std::string GetName(SDL_Scancode scanCode)
	{
		if (m_SdlScancodeToConfigMap.count(scanCode) > 0)
			return m_SdlScancodeToConfigMap[scanCode];
		return "";
	}
};