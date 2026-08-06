#pragma once
#include "Globals.h"
#include <wx/statline.h>
#include <wx/display.h>
#include <wx/notebook.h>
#include "ZoneEditorDialog.h"

class ControlsDialog : public wxDialog
{
public:
	ControlsDialog(wxWindow* parent, const Config::Settings::Controls& cfg);
	Config::Settings::Controls GetSettings() const;

private:

	static ConfigToSdlScancode m_configToSdlScancode;

	// Mouse tab widgets
	wxCheckBox* m_invertYAxis;
	wxCheckBox* m_invertXAxis;
	wxCheckBox* m_disableLRButtonsMenuOpen;

	wxSpinCtrlDouble* m_mouseScaleX;
	wxSpinCtrlDouble* m_mouseScaleY;

	wxChoice* m_spellLeft;
	wxChoice* m_spellRight;
	wxChoice* m_map;
	wxChoice* m_spellMenu;
	wxChoice* m_spellMenuMark;

	// Keyboard tab widgets
	wxButton* m_modernButton = nullptr;
	wxButton* m_classicButton = nullptr;

	wxChoice* m_forwardKey = nullptr;
	wxChoice* m_backwardsKey = nullptr;
	wxChoice* m_leftKey = nullptr;
	wxChoice* m_rightKey = nullptr;
	wxChoice* m_mapKey = nullptr;
	wxChoice* m_spellMenuKey = nullptr;
	wxChoice* m_spellMenuMarkKey = nullptr;

	wxButton* m_xboxPresetButton = nullptr;
	wxSpinCtrl* m_controllerId = nullptr;
	wxSpinCtrl* m_buttonMiniMap = nullptr;
	wxSpinCtrl* m_buttonSpell = nullptr;
	wxSpinCtrl* m_buttonPauseMenu = nullptr;
	wxSpinCtrl* m_buttonEsc = nullptr;
	wxSpinCtrl* m_buttonFireL = nullptr;
	wxSpinCtrl* m_buttonFireR = nullptr;
	wxSpinCtrl* m_buttonMenuSelect = nullptr;
	wxSpinCtrl* m_triggerDeadZone = nullptr;
	wxSpinCtrl* m_axisFireR = nullptr;
	wxSpinCtrl* m_axisFireL = nullptr;
	wxCheckBox* m_hapticEnabled = nullptr;
	wxSpinCtrl* m_hapticMaxGain = nullptr;
	wxSpinCtrl* m_hatNav = nullptr;
	wxSpinCtrl* m_hatMov = nullptr;
	wxCheckBox* m_hatNavInv = nullptr;
	wxCheckBox* m_hatMovInv = nullptr;
	wxSpinCtrl* m_axisLong = nullptr;
	wxSpinCtrl* m_axisLongDeadZone = nullptr;
	wxSpinCtrl* m_axisLongNavDeadZone = nullptr;
	wxCheckBox* m_axisLongInv = nullptr;
	wxSpinCtrl* m_axisTrans = nullptr;
	wxSpinCtrl* m_axisTransDeadZone = nullptr;
	wxSpinCtrl* m_axisTransNavDeadZone = nullptr;
	wxCheckBox* m_axisTransInv = nullptr;
	wxSpinCtrl* m_axisYaw = nullptr;
	wxSpinCtrl* m_axisYawDeadZone = nullptr;
	wxCheckBox* m_axisYawInv = nullptr;
	std::vector<Maths::Zone> m_axisYawSensitivity;
	wxButton* m_axisYawSensitivityButton = nullptr;
	wxSpinCtrl* m_axisPitch = nullptr;
	wxSpinCtrl* m_axisPitchDeadZone = nullptr;
	wxCheckBox* m_axisPitchInv = nullptr;
	std::vector<Maths::Zone> m_axisPitchSensitivity;
	wxButton* m_axisPitchSensitivityButton = nullptr;
	wxSpinCtrl* m_axisNavNs = nullptr;
	wxCheckBox* m_axisNavNsInv = nullptr;
	wxSpinCtrl* m_axisNavEw = nullptr;
	wxCheckBox* m_axisNavEwInv = nullptr;

	wxNotebook* m_notebook;

	wxPanel* m_mousePage;
	wxPanel* m_keyboardPage;
	wxScrolledWindow* m_joystickPage;

	Config::Settings::Controls m_cfg;
	void SetChoiceValue(wxChoice* choice, const wxString& value);
	void OnModernLayout(wxCommandEvent&);
	void OnClassicLayout(wxCommandEvent&);
	void OnXBoxPreset(wxCommandEvent& evt);
	void LoadKeyboardLayout(const wxString& layoutName);

	void CreateMousePage();
	void CreateKeyboardPage();
	void CreateJoystickPage();
	wxArrayString GetAvailableKeys() const;
	SDL_Scancode GetSelectedScancode(wxChoice* choice) const;
	void OnOK(wxCommandEvent&);
};