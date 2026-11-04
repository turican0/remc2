#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "../remc2/portability/Config.h"
#include "../remc2/portability/ConfigToSdlScancodeMapping.h"

// SetMinSize only limits later resizing: a dialog its constructor has already Fit() smaller stays that size.
// This grows it to the minimum as well (never shrinks it) and re-centres it, since it was centred at the old size.
inline void SetDialogMinSize(wxDialog& dlg, const wxSize& minSize)
{
	dlg.SetMinSize(minSize);
	wxSize size = dlg.GetSize();
	size.IncTo(minSize);
	dlg.SetSize(size);
	dlg.Centre();
}

enum {
	ID_BTN_PLAY = wxID_HIGHEST + 1,
	ID_BTN_FILE,
	ID_BTN_GAME,
	ID_BTN_CONTROLS,
	ID_BTN_SOUND,
	ID_BTN_DISPLAY,
	ID_BTN_GRAPHICS,
	ID_BTN_EXIT,
	ID_BTN_GAME_BROWSE,
	ID_BTN_CD_BROWSE,
	ID_BTN_GRAPHICS_BROWSE,
	ID_BTN_FIX_MENU_BROWSE,
	ID_BTN_FIX_TMAPS_BROWSE,
	ID_BTN_FONTS_BROWSE,
	ID_CHK_GRAPHICS,
	ID_CHK_FIX_MENU,
	ID_CHK_FIX_TMAPS,
	ID_CHK_FONTS,
	ID_BTN_LAUNCH_OPTIONS,
	ID_BTN_MULTIPLAYER,
	ID_BTN_SETTINGS,
};