#include "ControlsDialog.h"

ConfigToSdlScancode ControlsDialog::m_configToSdlScancode;

ControlsDialog::ControlsDialog(wxWindow* parent, const Config::Settings::Controls& cfg)
	: wxDialog(parent, wxID_ANY, "Controls Settings",
		wxDefaultPosition, wxSize(666, 950), 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	m_notebook = new wxNotebook(this, wxID_ANY);

	m_mousePage = new wxPanel(m_notebook, wxID_ANY);
	m_keyboardPage = new wxPanel(m_notebook, wxID_ANY);
	m_joystickPage = new wxScrolledWindow(m_notebook, wxID_ANY);

	m_joystickPage->SetScrollRate(0, 10);

	m_notebook->AddPage(m_mousePage, "Mouse");
	m_notebook->AddPage(m_keyboardPage, "Keyboard");
	m_notebook->AddPage(m_joystickPage, "Joystick");

	CreateMousePage();
	CreateKeyboardPage();
	CreateJoystickPage();

	mainSizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(mainSizer);
	Centre(wxBOTH);

	// ── OK / Cancel ───────────────────────────────────────────────────────────
	auto* btnSizer = new wxStdDialogButtonSizer();
	auto* btnOK = new wxButton(this, wxID_OK, "OK");
	auto* btnCancel = new wxButton(this, wxID_CANCEL, "Cancel");
	btnOK->SetDefault();
	btnSizer->AddButton(btnOK);
	btnSizer->AddButton(btnCancel);
	btnSizer->Realize();
	mainSizer->Add(btnSizer, 0, wxEXPAND | wxALL, 10);

	SetSizer(mainSizer);
	btnOK->Bind(wxEVT_BUTTON, &ControlsDialog::OnOK, this);

	SetSizerAndFit(mainSizer);
	Centre(wxBOTH);
}

void ControlsDialog::CreateMousePage()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// -- Invert Axes --
	wxStaticBoxSizer* axesSizer = new wxStaticBoxSizer(wxVERTICAL, m_mousePage, "Axes");
	m_invertYAxis = new wxCheckBox(m_mousePage, wxID_ANY, "Invert Y Axis");
	m_invertYAxis->SetToolTip("True By default, the Y axis is inverted in the original game");
	m_invertXAxis = new wxCheckBox(m_mousePage, wxID_ANY, "Invert X Axis");
	m_invertXAxis->SetToolTip("By default, the X axis is not inverted");
	m_invertYAxis->SetValue(m_cfg.m_Mouse.m_InvertYAxis);
	m_invertXAxis->SetValue(m_cfg.m_Mouse.m_InvertXAxis);
	axesSizer->Add(m_invertYAxis, 0, wxALL, 5);
	axesSizer->Add(m_invertXAxis, 0, wxALL, 5);
	sizer->Add(axesSizer, 0, wxEXPAND | wxALL, 5);

	// -- Mouse Scale --
	wxStaticBoxSizer* scaleSizer = new wxStaticBoxSizer(wxVERTICAL, m_mousePage, "Sensitivity (1.0 = normal, higher = slower)");

	wxBoxSizer* scaleXRow = new wxBoxSizer(wxHORIZONTAL);
	scaleXRow->Add(new wxStaticText(m_mousePage, wxID_ANY, "Scale X:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	m_mouseScaleX = new wxSpinCtrlDouble(m_mousePage, wxID_ANY);
	m_mouseScaleX->SetRange(0.1, 5.0);
	m_mouseScaleX->SetIncrement(0.1);
	m_mouseScaleX->SetValue(m_cfg.m_Mouse.m_MouseScaleX);
	m_mouseScaleX->SetToolTip("1.0 = normal, 1.5 = slower, 0.5 faster etc... (Note: Effects joystick steering too)");
	scaleXRow->Add(m_mouseScaleX, 1, wxEXPAND);

	wxBoxSizer* scaleYRow = new wxBoxSizer(wxHORIZONTAL);
	scaleYRow->Add(new wxStaticText(m_mousePage, wxID_ANY, "Scale Y:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	m_mouseScaleY = new wxSpinCtrlDouble(m_mousePage, wxID_ANY);
	m_mouseScaleY->SetRange(0.1, 5.0);
	m_mouseScaleY->SetIncrement(0.1);
	m_mouseScaleY->SetValue(m_cfg.m_Mouse.m_MouseScaleY);
	m_mouseScaleY->SetToolTip("1.0 = normal, 1.5 = slower, 0.5 faster etc... (Note: Effects joystick steering too)");
	scaleYRow->Add(m_mouseScaleY, 1, wxEXPAND);

	scaleSizer->Add(scaleXRow, 0, wxEXPAND | wxALL, 5);
	scaleSizer->Add(scaleYRow, 0, wxEXPAND | wxALL, 5);
	sizer->Add(scaleSizer, 0, wxEXPAND | wxALL, 5);

	// -- Mouse Buttons --
	wxStaticBoxSizer* btnSizer = new wxStaticBoxSizer(wxVERTICAL, m_mousePage, "Mouse Buttons");

	const wxString buttonChoices[] = { "0 - Undefined", "1 - Left", "2 - Middle", "3 - Right", "4 - Mouse 4", "5 - Mouse 5" };
	const int numChoices = WXSIZEOF(buttonChoices);

	auto addButtonRow = [&](const wxString& label, wxChoice*& ctrl, int value)
		{
			wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
			row->Add(new wxStaticText(m_mousePage, wxID_ANY, label), 1, wxALIGN_CENTER_VERTICAL);
			ctrl = new wxChoice(m_mousePage, wxID_ANY, wxDefaultPosition, wxDefaultSize, numChoices, buttonChoices);
			ctrl->SetSelection(value); // value maps directly to index (0=Undefined, 1=Left, etc.)
			row->Add(ctrl, 1, wxEXPAND);
			btnSizer->Add(row, 0, wxEXPAND | wxALL, 5);
		};

	addButtonRow("Fire Spell Left:", m_spellLeft, m_cfg.m_Mouse.m_SpellLeft);
	addButtonRow("Fire Spell Right:", m_spellRight, m_cfg.m_Mouse.m_SpellRight);
	addButtonRow("Open Map:", m_map, m_cfg.m_Mouse.m_Map);
	addButtonRow("Open Spell Menu:", m_spellMenu, m_cfg.m_Mouse.m_SpellMenu);
	addButtonRow("Favorite Spell:", m_spellMenuMark, m_cfg.m_Mouse.m_SpellMenuMark);

	sizer->Add(btnSizer, 0, wxEXPAND | wxALL, 5);

	// -- Misc --
	wxStaticBoxSizer* miscSizer = new wxStaticBoxSizer(wxVERTICAL, m_mousePage, "Misc");
	m_disableLRButtonsMenuOpen = new wxCheckBox(m_mousePage, wxID_ANY, "Disable L+R Buttons Opening Menu");
	m_disableLRButtonsMenuOpen->SetValue(m_cfg.m_Mouse.m_DisableLRButtonsMenuOpen);
	m_disableLRButtonsMenuOpen->SetToolTip("When enabled, stops the two fire buttons opening the map screen");
	miscSizer->Add(m_disableLRButtonsMenuOpen, 0, wxALL, 5);
	sizer->Add(miscSizer, 0, wxEXPAND | wxALL, 5);

	m_mousePage->SetSizer(sizer);
}

void ControlsDialog::CreateKeyboardPage()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	//
	// Layout presets
	//
	wxStaticBoxSizer* presetSizer =
		new wxStaticBoxSizer(
			wxVERTICAL,
			m_keyboardPage,
			"Keyboard Layout");

	wxBoxSizer* buttonRow = new wxBoxSizer(wxHORIZONTAL);

	m_modernButton =
		new wxButton(
			m_keyboardPage,
			wxID_ANY,
			"Modern");

	m_classicButton =
		new wxButton(
			m_keyboardPage,
			wxID_ANY,
			"Classic");

	buttonRow->Add(m_modernButton, 1, wxEXPAND | wxRIGHT,5);
	buttonRow->Add(m_classicButton, 1, wxEXPAND);

	presetSizer->Add(buttonRow, 0, wxEXPAND | wxALL, 5);

	m_modernButton->Bind(wxEVT_BUTTON, &ControlsDialog::OnModernLayout, this);
	m_classicButton->Bind(wxEVT_BUTTON, &ControlsDialog::OnClassicLayout, this);

	sizer->Add(presetSizer, 0, wxEXPAND | wxALL, 5);

	//
	// Key bindings
	//
	wxStaticBoxSizer* bindingsSizer = new wxStaticBoxSizer(wxVERTICAL, m_keyboardPage, "Key Bindings");

	const wxArrayString keyChoices = GetAvailableKeys();

	// Forward
	wxBoxSizer* forwardRow = new wxBoxSizer(wxHORIZONTAL);
	forwardRow->Add(new wxStaticText(m_keyboardPage, wxID_ANY, "Forward:"), 1, wxALIGN_CENTER_VERTICAL);
	m_forwardKey = new wxChoice(m_keyboardPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, keyChoices);
	SetChoiceValue(m_forwardKey, m_configToSdlScancode.GetName(m_cfg.m_Keyboard.m_Forward));
	forwardRow->Add(m_forwardKey, 1, wxEXPAND);
	bindingsSizer->Add(forwardRow, 0, wxEXPAND | wxALL, 5);

	// Backwards
	wxBoxSizer* backwardsRow = new wxBoxSizer(wxHORIZONTAL);
	backwardsRow->Add(new wxStaticText(m_keyboardPage, wxID_ANY, "Backwards:"), 1, wxALIGN_CENTER_VERTICAL);
	m_backwardsKey = new wxChoice(m_keyboardPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, keyChoices);
	SetChoiceValue(m_backwardsKey, m_configToSdlScancode.GetName(m_cfg.m_Keyboard.m_Backwards));
	backwardsRow->Add(m_backwardsKey, 1, wxEXPAND);
	bindingsSizer->Add(backwardsRow, 0, wxEXPAND | wxALL, 5);

	// Left
	wxBoxSizer* leftRow = new wxBoxSizer(wxHORIZONTAL);
	leftRow->Add(new wxStaticText(m_keyboardPage, wxID_ANY, "Move Left:"), 1, wxALIGN_CENTER_VERTICAL);
	m_leftKey = new wxChoice(m_keyboardPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, keyChoices);
	SetChoiceValue(m_leftKey, m_configToSdlScancode.GetName(m_cfg.m_Keyboard.m_Left));
	leftRow->Add(m_leftKey, 1, wxEXPAND);
	bindingsSizer->Add(leftRow, 0, wxEXPAND | wxALL, 5);

	// Right
	wxBoxSizer* rightRow = new wxBoxSizer(wxHORIZONTAL);
	rightRow->Add(new wxStaticText(m_keyboardPage, wxID_ANY, "Move Right:"), 1, wxALIGN_CENTER_VERTICAL);
	m_rightKey = new wxChoice(m_keyboardPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, keyChoices);
	SetChoiceValue(m_rightKey, m_configToSdlScancode.GetName(m_cfg.m_Keyboard.m_Right));
	rightRow->Add(m_rightKey, 1, wxEXPAND);
	bindingsSizer->Add(rightRow, 0, wxEXPAND | wxALL, 5);

	// Map
	wxBoxSizer* mapRow = new wxBoxSizer(wxHORIZONTAL);
	mapRow->Add(new wxStaticText(m_keyboardPage, wxID_ANY, "Open Map:"), 1, wxALIGN_CENTER_VERTICAL);
	m_mapKey = new wxChoice(m_keyboardPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, keyChoices);
	SetChoiceValue(m_mapKey, m_configToSdlScancode.GetName(m_cfg.m_Keyboard.m_Map));
	mapRow->Add(m_mapKey, 1, wxEXPAND);
	bindingsSizer->Add(mapRow, 0, wxEXPAND | wxALL, 5);

	// Spell Menu
	wxBoxSizer* spellMenuRow = new wxBoxSizer(wxHORIZONTAL);
	spellMenuRow->Add(new wxStaticText(m_keyboardPage, wxID_ANY, "Open Spell Menu:"), 1, wxALIGN_CENTER_VERTICAL);
	m_spellMenuKey = new wxChoice(m_keyboardPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, keyChoices);
	SetChoiceValue(m_spellMenuKey, m_configToSdlScancode.GetName(m_cfg.m_Keyboard.m_SpellMenu));
	spellMenuRow->Add(m_spellMenuKey, 1, wxEXPAND);
	bindingsSizer->Add(spellMenuRow, 0, wxEXPAND | wxALL, 5);

	// Mark
	wxBoxSizer* markRow = new wxBoxSizer(wxHORIZONTAL);
	markRow->Add(new wxStaticText(m_keyboardPage, wxID_ANY, "Favourite Spell:"), 1, wxALIGN_CENTER_VERTICAL);
	m_spellMenuMarkKey = new wxChoice(m_keyboardPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, keyChoices);
	SetChoiceValue(m_spellMenuMarkKey, m_configToSdlScancode.GetName(m_cfg.m_Keyboard.m_SpellMenuMark));
	markRow->Add(m_spellMenuMarkKey, 1, wxEXPAND);

	bindingsSizer->Add(markRow, 0, wxEXPAND | wxALL, 5);

	sizer->Add(bindingsSizer, 1, wxEXPAND | wxALL, 5);

	m_keyboardPage->SetSizer(sizer);
}

void ControlsDialog::CreateJoystickPage()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	// ── Left column ───────────────────────────────────────────────────────────
	wxBoxSizer* leftCol = new wxBoxSizer(wxVERTICAL);

	// -- Preset --
	wxStaticBoxSizer* presetSizer =
		new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "Preset");

	m_xboxPresetButton = new wxButton(m_joystickPage, wxID_ANY, "Xbox Controller");
	presetSizer->Add(m_xboxPresetButton, 0, wxEXPAND | wxALL, 5);
	m_xboxPresetButton->Bind(wxEVT_BUTTON, &ControlsDialog::OnXBoxPreset, this);

	leftCol->Add(presetSizer, 0, wxEXPAND | wxALL, 5);

	// -- Controller --
	wxStaticBoxSizer* controllerSizer =
		new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "Controller");

	wxBoxSizer* controllerIdRow = new wxBoxSizer(wxHORIZONTAL);
	controllerIdRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Controller ID:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_controllerId = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_controllerId->SetRange(0, 7);
	m_controllerId->SetValue(m_cfg.m_GamePad.m_ControllerId);
	controllerIdRow->Add(m_controllerId, 1, wxEXPAND);
	controllerSizer->Add(controllerIdRow, 0, wxEXPAND | wxALL, 5);

	leftCol->Add(controllerSizer, 0, wxEXPAND | wxALL, 5);

	// -- Buttons --
	wxStaticBoxSizer* btnSizer =
		new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "Button Bindings");

	auto addButtonRow = [&](const wxString& label, wxSpinCtrl*& ctrl, int value)
		{
			wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
			row->Add(new wxStaticText(m_joystickPage, wxID_ANY, label),
				1, wxALIGN_CENTER_VERTICAL);
			ctrl = new wxSpinCtrl(m_joystickPage, wxID_ANY);
			ctrl->SetRange(0, 128);
			ctrl->SetValue(value);
			row->Add(ctrl, 1, wxEXPAND);
			btnSizer->Add(row, 0, wxEXPAND | wxALL, 5);
		};

	addButtonRow("Mini Map:", m_buttonMiniMap, m_cfg.m_GamePad.m_ButtonMiniMap);
	addButtonRow("Spell:", m_buttonSpell, m_cfg.m_GamePad.m_ButtonSpell);
	addButtonRow("Pause Menu:", m_buttonPauseMenu, m_cfg.m_GamePad.m_ButtonPauseMenu);
	addButtonRow("Esc:", m_buttonEsc, m_cfg.m_GamePad.m_ButtonEsc);
	addButtonRow("Fire Left Spell:", m_buttonFireL, m_cfg.m_GamePad.m_ButtonFireL);
	addButtonRow("Fire Right Spell:", m_buttonFireR, m_cfg.m_GamePad.m_ButtonFireR);
	addButtonRow("Menu Select:", m_buttonMenuSelect, m_cfg.m_GamePad.m_ButtonMenuSelect);

	wxBoxSizer* deadZoneRow = new wxBoxSizer(wxHORIZONTAL);
	deadZoneRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Trigger Dead Zone:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_triggerDeadZone = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_triggerDeadZone->SetRange(0, 32767);
	m_triggerDeadZone->SetValue(m_cfg.m_GamePad.m_TriggerDeadZone);
	deadZoneRow->Add(m_triggerDeadZone, 1, wxEXPAND);
	btnSizer->Add(deadZoneRow, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* axisFireLRow = new wxBoxSizer(wxHORIZONTAL);
	axisFireLRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Fire Left Spell Trigger Axis:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_axisFireL = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_axisFireL->SetRange(0, 7);
	m_axisFireL->SetValue(m_cfg.m_GamePad.m_AxisFireL);
	axisFireLRow->Add(m_axisFireL, 1, wxEXPAND);
	btnSizer->Add(axisFireLRow, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* axisFireRRow = new wxBoxSizer(wxHORIZONTAL);
	axisFireRRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Fire Right Spell Trigger Axis:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_axisFireR = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_axisFireR->SetRange(0, 7);
	m_axisFireR->SetValue(m_cfg.m_GamePad.m_AxisFireR);
	axisFireRRow->Add(m_axisFireR, 1, wxEXPAND);
	btnSizer->Add(axisFireRRow, 0, wxEXPAND | wxALL, 5);

	leftCol->Add(btnSizer, 0, wxEXPAND | wxALL, 5);

	// -- Haptic --
	wxStaticBoxSizer* hapticSizer =
		new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "Haptic");

	m_hapticEnabled = new wxCheckBox(m_joystickPage, wxID_ANY, "Enable Haptic Feedback");
	m_hapticEnabled->SetValue(m_cfg.m_GamePad.m_HapticEnabled);
	hapticSizer->Add(m_hapticEnabled, 0, wxALL, 5);

	wxBoxSizer* gainRow = new wxBoxSizer(wxHORIZONTAL);
	gainRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Max Gain:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_hapticMaxGain = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_hapticMaxGain->SetRange(0, 100);
	m_hapticMaxGain->SetValue(m_cfg.m_GamePad.m_HapticMaxGain);
	gainRow->Add(m_hapticMaxGain, 1, wxEXPAND);
	hapticSizer->Add(gainRow, 0, wxEXPAND | wxALL, 5);

	leftCol->Add(hapticSizer, 0, wxEXPAND | wxALL, 5);

	// -- POV Hat --
	wxStaticBoxSizer* hatSizer =
		new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "POV Hat");

	auto addHatRow = [&](const wxString& label, wxSpinCtrl*& ctrl, wxCheckBox*& inv, int value, bool invValue)
		{
			wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
			row->Add(new wxStaticText(m_joystickPage, wxID_ANY, label),
				1, wxALIGN_CENTER_VERTICAL);
			ctrl = new wxSpinCtrl(m_joystickPage, wxID_ANY);
			ctrl->SetRange(0, 128);
			ctrl->SetValue(value);
			row->Add(ctrl, 1, wxEXPAND | wxRIGHT, 5);
			inv = new wxCheckBox(m_joystickPage, wxID_ANY, "Invert");
			inv->SetValue(invValue);
			row->Add(inv, 0, wxALIGN_CENTER_VERTICAL);
			hatSizer->Add(row, 0, wxEXPAND | wxALL, 5);
		};

	addHatRow("Navigation:", m_hatNav, m_hatNavInv, m_cfg.m_GamePad.m_HatNav, m_cfg.m_GamePad.m_HatNavInv);
	addHatRow("Movement:", m_hatMov, m_hatMovInv, m_cfg.m_GamePad.m_HatMov, m_cfg.m_GamePad.m_HatMovInv);

	leftCol->Add(hatSizer, 0, wxEXPAND | wxALL, 5);

	sizer->Add(leftCol, 1, wxEXPAND);

	// ── Right column ──────────────────────────────────────────────────────────
	wxBoxSizer* rightCol = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* axisSizer =
		new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "Axes");

	// Yaw
	wxStaticBoxSizer* yawGroup = new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "Yaw");

	wxBoxSizer* yawAxisRow = new wxBoxSizer(wxHORIZONTAL);
	yawAxisRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Axis:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_axisYaw = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_axisYaw->SetRange(0, 7);
	m_axisYaw->SetValue(m_cfg.m_GamePad.m_AxisYaw);
	yawAxisRow->Add(m_axisYaw, 1, wxEXPAND);
	yawGroup->Add(yawAxisRow, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* yawDeadZoneRow = new wxBoxSizer(wxHORIZONTAL);
	yawDeadZoneRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Dead Zone:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_axisYawDeadZone = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_axisYawDeadZone->SetRange(0, 32767);
	m_axisYawDeadZone->SetValue(m_cfg.m_GamePad.m_AxisYawDeadZone);
	yawDeadZoneRow->Add(m_axisYawDeadZone, 1, wxEXPAND);
	yawGroup->Add(yawDeadZoneRow, 0, wxEXPAND | wxALL, 5);

	m_axisYawSensitivity = m_cfg.m_GamePad.m_AxisYawSensitivity;
	m_axisYawSensitivityButton = new wxButton(m_joystickPage, wxID_ANY, "Edit Yaw Sensitivity...");
	m_axisYawSensitivityButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
		{
			ZoneEditorDialog dlg(this, m_axisYawSensitivity);
			if (dlg.ShowModal() == wxID_OK)
				m_axisYawSensitivity = dlg.GetZones();
		});
	yawGroup->Add(m_axisYawSensitivityButton, 0, wxEXPAND | wxALL, 5);

	m_axisYawInv = new wxCheckBox(m_joystickPage, wxID_ANY, "Invert");
	m_axisYawInv->SetValue(m_cfg.m_GamePad.m_AxisYawInv);
	yawGroup->Add(m_axisYawInv, 0, wxALL, 5);

	axisSizer->Add(yawGroup, 0, wxEXPAND | wxALL, 3);

	// Pitch
	wxStaticBoxSizer* pitchGroup = new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, "Pitch");

	wxBoxSizer* pitchAxisRow = new wxBoxSizer(wxHORIZONTAL);
	pitchAxisRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Axis:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_axisPitch = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_axisPitch->SetRange(0, 7);
	m_axisPitch->SetValue(m_cfg.m_GamePad.m_AxisPitch);
	pitchAxisRow->Add(m_axisPitch, 1, wxEXPAND);
	pitchGroup->Add(pitchAxisRow, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* pitchDeadZoneRow = new wxBoxSizer(wxHORIZONTAL);
	pitchDeadZoneRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Dead Zone:"),
		1, wxALIGN_CENTER_VERTICAL);
	m_axisPitchDeadZone = new wxSpinCtrl(m_joystickPage, wxID_ANY);
	m_axisPitchDeadZone->SetRange(0, 32767);
	m_axisPitchDeadZone->SetValue(m_cfg.m_GamePad.m_AxisPitchDeadZone);
	pitchDeadZoneRow->Add(m_axisPitchDeadZone, 1, wxEXPAND);
	pitchGroup->Add(pitchDeadZoneRow, 0, wxEXPAND | wxALL, 5);

	m_axisPitchSensitivity = m_cfg.m_GamePad.m_AxisPitchSensitivity;
	m_axisPitchSensitivityButton = new wxButton(m_joystickPage, wxID_ANY, "Edit Pitch Sensitivity...");
	m_axisPitchSensitivityButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
		{
			ZoneEditorDialog dlg(this, m_axisPitchSensitivity);
			if (dlg.ShowModal() == wxID_OK)
				m_axisPitchSensitivity = dlg.GetZones();
		});
	pitchGroup->Add(m_axisPitchSensitivityButton, 0, wxEXPAND | wxALL, 5);

	m_axisPitchInv = new wxCheckBox(m_joystickPage, wxID_ANY, "Invert");
	m_axisPitchInv->SetValue(m_cfg.m_GamePad.m_AxisPitchInv);
	pitchGroup->Add(m_axisPitchInv, 0, wxALL, 5);

	axisSizer->Add(pitchGroup, 0, wxEXPAND | wxALL, 3);

	auto addAxisRowWithNavDeadZone = [&](const wxString& label, wxSpinCtrl*& axis,
		wxSpinCtrl*& deadZone, wxSpinCtrl*& navDeadZone, wxCheckBox*& inv,
		int axisVal, int deadZoneVal, int navDeadZoneVal, bool invVal)
		{
			wxStaticBoxSizer* group = new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, label);

			wxBoxSizer* axisRow = new wxBoxSizer(wxHORIZONTAL);
			axisRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Axis:"),
				1, wxALIGN_CENTER_VERTICAL);
			axis = new wxSpinCtrl(m_joystickPage, wxID_ANY);
			axis->SetRange(0, 7);
			axis->SetValue(axisVal);
			axisRow->Add(axis, 1, wxEXPAND);
			group->Add(axisRow, 0, wxEXPAND | wxALL, 5);

			wxBoxSizer* deadZoneRow = new wxBoxSizer(wxHORIZONTAL);
			deadZoneRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Dead Zone:"),
				1, wxALIGN_CENTER_VERTICAL);
			deadZone = new wxSpinCtrl(m_joystickPage, wxID_ANY);
			deadZone->SetRange(0, 32767);
			deadZone->SetValue(deadZoneVal);
			deadZoneRow->Add(deadZone, 1, wxEXPAND);
			group->Add(deadZoneRow, 0, wxEXPAND | wxALL, 5);

			wxBoxSizer* navDeadZoneRow = new wxBoxSizer(wxHORIZONTAL);
			navDeadZoneRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Nav Dead Zone:"),
				1, wxALIGN_CENTER_VERTICAL);
			navDeadZone = new wxSpinCtrl(m_joystickPage, wxID_ANY);
			navDeadZone->SetRange(0, 32767);
			navDeadZone->SetValue(navDeadZoneVal);
			navDeadZoneRow->Add(navDeadZone, 1, wxEXPAND);
			group->Add(navDeadZoneRow, 0, wxEXPAND | wxALL, 5);

			inv = new wxCheckBox(m_joystickPage, wxID_ANY, "Invert");
			inv->SetValue(invVal);
			group->Add(inv, 0, wxALL, 5);

			axisSizer->Add(group, 0, wxEXPAND | wxALL, 3);
		};

	auto addAxisRowNoDeadZone = [&](const wxString& label, wxSpinCtrl*& axis, wxCheckBox*& inv,
		int axisVal, bool invVal)
		{
			wxStaticBoxSizer* group = new wxStaticBoxSizer(wxVERTICAL, m_joystickPage, label);

			wxBoxSizer* axisRow = new wxBoxSizer(wxHORIZONTAL);
			axisRow->Add(new wxStaticText(m_joystickPage, wxID_ANY, "Axis:"),
				1, wxALIGN_CENTER_VERTICAL);
			axis = new wxSpinCtrl(m_joystickPage, wxID_ANY);
			axis->SetRange(0, 7);
			axis->SetValue(axisVal);
			axisRow->Add(axis, 1, wxEXPAND);
			group->Add(axisRow, 0, wxEXPAND | wxALL, 5);

			inv = new wxCheckBox(m_joystickPage, wxID_ANY, "Invert");
			inv->SetValue(invVal);
			group->Add(inv, 0, wxALL, 5);

			axisSizer->Add(group, 0, wxEXPAND | wxALL, 3);
		};

	addAxisRowWithNavDeadZone("Longitudinal",
		m_axisLong, m_axisLongDeadZone, m_axisLongNavDeadZone, m_axisLongInv,
		m_cfg.m_GamePad.m_AxisLong, m_cfg.m_GamePad.m_AxisLongDeadZone,
		m_cfg.m_GamePad.m_AxisLongNavDeadZone, m_cfg.m_GamePad.m_AxisLongInv);

	addAxisRowWithNavDeadZone("Transverse",
		m_axisTrans, m_axisTransDeadZone, m_axisTransNavDeadZone, m_axisTransInv,
		m_cfg.m_GamePad.m_AxisTrans, m_cfg.m_GamePad.m_AxisTransDeadZone,
		m_cfg.m_GamePad.m_AxisTransNavDeadZone, m_cfg.m_GamePad.m_AxisTransInv);

	addAxisRowNoDeadZone("Nav North South", m_axisNavNs, m_axisNavNsInv,
		m_cfg.m_GamePad.m_AxisNavNs, m_cfg.m_GamePad.m_AxisNavNsInv);

	addAxisRowNoDeadZone("Nav East West", m_axisNavEw, m_axisNavEwInv,
		m_cfg.m_GamePad.m_AxisNavEw, m_cfg.m_GamePad.m_AxisNavEwInv);

	rightCol->Add(axisSizer, 0, wxEXPAND | wxALL, 5);

	sizer->Add(rightCol, 1, wxEXPAND);

	m_joystickPage->SetSizer(sizer);
	m_joystickPage->FitInside();
}

void ControlsDialog::OnModernLayout(wxCommandEvent&)
{
	LoadKeyboardLayout("Modern");
}

void ControlsDialog::OnClassicLayout(wxCommandEvent&)
{
	LoadKeyboardLayout("Classic");
}

void ControlsDialog::OnXBoxPreset(wxCommandEvent&)
{
	m_controllerId->SetValue(0);
	m_buttonMiniMap->SetValue(3);
	m_buttonSpell->SetValue(1);
	m_buttonPauseMenu->SetValue(7);
	m_buttonEsc->SetValue(8);
	m_buttonFireL->SetValue(5);
	m_buttonFireR->SetValue(6);
	m_buttonMenuSelect->SetValue(1);
	m_triggerDeadZone->SetValue(3000);
	m_hapticEnabled->SetValue(true);
	m_hapticMaxGain->SetValue(75);
	m_hatNav->SetValue(0);
	m_hatMov->SetValue(0);
	m_hatNavInv->SetValue(false);
	m_hatMovInv->SetValue(false);
	m_axisLong->SetValue(2);
	m_axisLongDeadZone->SetValue(12000);
	m_axisLongNavDeadZone->SetValue(6000);
	m_axisLongInv->SetValue(true);
	m_axisTrans->SetValue(1);
	m_axisTransInv->SetValue(false);
	m_axisTransDeadZone->SetValue(12000);
	m_axisTransNavDeadZone->SetValue(6000);
	m_axisNavNs->SetValue(2);
	m_axisNavNsInv->SetValue(false);
	m_axisNavEw->SetValue(1);
	m_axisNavEwInv->SetValue(false);
	m_axisFireR->SetValue(6);
	m_axisFireL->SetValue(5);
	m_axisYaw->SetValue(3);
	m_axisYawInv->SetValue(false);
	m_axisYawDeadZone->SetValue(3000);
	m_axisPitch->SetValue(4);
	m_axisPitchInv->SetValue(false);
	m_axisPitchDeadZone->SetValue(3000);

	m_axisYawSensitivity.clear();
	m_axisYawSensitivity = {
		{    0,  8000, 0.1 },
		{ 8000, 15000, 0.2 },
		{15000, 20000, 0.3 },
		{20000, 25000, 0.35},
		{25000, 26000, 0.4 },
		{26000, 27000, 0.5 },
		{27000, 28000, 0.6 },
		{28000, 29000, 0.7 },
		{29000, 30000, 0.8 },
		{30000, 31000, 0.9 },
		{31000, 32767, 1.0 }
	};

	m_axisPitchSensitivity.clear();
	m_axisPitchSensitivity = {
		{    0,  8000, 0.1 },
		{ 8000, 15000, 0.2 },
		{15000, 20000, 0.3 },
		{20000, 25000, 0.35},
		{25000, 26000, 0.4 },
		{26000, 27000, 0.5 },
		{27000, 28000, 0.6 },
		{28000, 29000, 0.7 },
		{29000, 30000, 0.8 },
		{30000, 31000, 0.9 },
		{31000, 32767, 1.0 }
	};
}

void ControlsDialog::LoadKeyboardLayout(const wxString& layoutName)
{
	if (layoutName.ToStdString() == "Modern")
	{
		SetChoiceValue(m_forwardKey, "W");
		SetChoiceValue(m_backwardsKey, "S");
		SetChoiceValue(m_leftKey, "A");
		SetChoiceValue(m_rightKey, "D");
		SetChoiceValue(m_mapKey, "TAB");
		SetChoiceValue(m_spellMenuKey, "LCTRL");
		SetChoiceValue(m_spellMenuMarkKey, "LSHIFT");
		return;
	}
	
	if (layoutName.ToStdString() == "Classic")
	{
		SetChoiceValue(m_forwardKey, "UP");
		SetChoiceValue(m_backwardsKey, "DOWN");
		SetChoiceValue(m_leftKey, "LEFT");
		SetChoiceValue(m_rightKey, "RIGHT");
		SetChoiceValue(m_mapKey, "RETURN");
		SetChoiceValue(m_spellMenuKey, "RCTRL");
		SetChoiceValue(m_spellMenuMarkKey, "RSHIFT");
		return;
	}
}

void ControlsDialog::SetChoiceValue(wxChoice* choice, const wxString& value)
{
	int idx = choice->FindString(value);

	if (idx != wxNOT_FOUND)
		choice->SetSelection(idx);
}


wxArrayString ControlsDialog::GetAvailableKeys() const
{
	wxArrayString keys;

	for (const auto& [scancode, name] : m_configToSdlScancode.GetScancodeMap())
	{
		if (!name.empty())
			keys.Add(wxString(name));
	}

	return keys;
}

SDL_Scancode ControlsDialog::GetSelectedScancode(wxChoice* choice) const
{
	int sel = choice->GetSelection();
	wxString str = choice->GetString(choice->GetSelection());
	SDL_Scancode code = m_configToSdlScancode.GetScancode(choice->GetString(choice->GetSelection()).ToStdString());
	return code;
}

Config::Settings::Controls ControlsDialog::GetSettings() const
{
	Config::Settings::Controls cfg = m_cfg;

	cfg.m_Mouse.m_InvertXAxis = m_invertXAxis->GetValue();
	cfg.m_Mouse.m_InvertYAxis = m_invertYAxis->GetValue();
	cfg.m_Mouse.m_MouseScaleX = m_mouseScaleX->GetValue();
	cfg.m_Mouse.m_MouseScaleY = m_mouseScaleY->GetValue();
	cfg.m_Mouse.m_SpellLeft = m_spellLeft->GetSelection();
	cfg.m_Mouse.m_SpellRight = m_spellRight->GetSelection();
	cfg.m_Mouse.m_Map = m_map->GetSelection();
	cfg.m_Mouse.m_SpellMenu = m_spellMenu->GetSelection();
	cfg.m_Mouse.m_SpellMenuMark = m_spellMenuMark->GetSelection();
	cfg.m_Mouse.m_DisableLRButtonsMenuOpen = m_disableLRButtonsMenuOpen->GetValue();

	cfg.m_Keyboard.m_Forward = GetSelectedScancode(m_forwardKey);
	cfg.m_Keyboard.m_Backwards = GetSelectedScancode(m_backwardsKey);
	cfg.m_Keyboard.m_Left = GetSelectedScancode(m_leftKey);
	cfg.m_Keyboard.m_Right = GetSelectedScancode(m_rightKey);
	cfg.m_Keyboard.m_Map = GetSelectedScancode(m_mapKey);
	cfg.m_Keyboard.m_SpellMenu = GetSelectedScancode(m_spellMenuKey);
	cfg.m_Keyboard.m_SpellMenuMark = GetSelectedScancode(m_spellMenuMarkKey);

	cfg.m_GamePad.m_ControllerId = m_controllerId->GetValue();
	cfg.m_GamePad.m_ButtonMiniMap = m_buttonMiniMap->GetValue();
	cfg.m_GamePad.m_ButtonSpell = m_buttonSpell->GetValue();
	cfg.m_GamePad.m_ButtonPauseMenu = m_buttonPauseMenu->GetValue();
	cfg.m_GamePad.m_ButtonEsc = m_buttonEsc->GetValue();
	cfg.m_GamePad.m_ButtonFireL = m_buttonFireL->GetValue();
	cfg.m_GamePad.m_ButtonFireR = m_buttonFireR->GetValue();
	cfg.m_GamePad.m_ButtonMenuSelect = m_buttonMenuSelect->GetValue();
	cfg.m_GamePad.m_TriggerDeadZone = m_triggerDeadZone->GetValue();
	cfg.m_GamePad.m_AxisFireL = m_axisFireL->GetValue();
	cfg.m_GamePad.m_AxisFireR = m_axisFireR->GetValue();

	cfg.m_GamePad.m_HapticEnabled = m_hapticEnabled->GetValue();
	cfg.m_GamePad.m_HapticMaxGain = m_hapticMaxGain->GetValue();

	cfg.m_GamePad.m_HatNav = m_hatNav->GetValue();
	cfg.m_GamePad.m_HatNavInv = m_hatNavInv->GetValue();
	cfg.m_GamePad.m_HatMov = m_hatMov->GetValue();
	cfg.m_GamePad.m_HatMovInv = m_hatMovInv->GetValue();

	cfg.m_GamePad.m_AxisLong = m_axisLong->GetValue();
	cfg.m_GamePad.m_AxisLongDeadZone = m_axisLongDeadZone->GetValue();
	cfg.m_GamePad.m_AxisLongNavDeadZone = m_axisLongNavDeadZone->GetValue();
	cfg.m_GamePad.m_AxisLongInv = m_axisLongInv->GetValue();
	cfg.m_GamePad.m_AxisTrans = m_axisTrans->GetValue();
	cfg.m_GamePad.m_AxisTransDeadZone = m_axisTransDeadZone->GetValue();
	cfg.m_GamePad.m_AxisTransNavDeadZone = m_axisTransNavDeadZone->GetValue();
	cfg.m_GamePad.m_AxisTransInv = m_axisTransInv->GetValue();
	cfg.m_GamePad.m_AxisYaw = m_axisYaw->GetValue();
	cfg.m_GamePad.m_AxisYawDeadZone = m_axisYawDeadZone->GetValue();
	cfg.m_GamePad.m_AxisYawInv = m_axisYawInv->GetValue();
	cfg.m_GamePad.m_AxisPitch = m_axisPitch->GetValue();
	cfg.m_GamePad.m_AxisPitchDeadZone = m_axisPitchDeadZone->GetValue();
	cfg.m_GamePad.m_AxisPitchInv = m_axisPitchInv->GetValue();
	cfg.m_GamePad.m_AxisNavNs = m_axisNavNs->GetValue();
	cfg.m_GamePad.m_AxisNavNsInv = m_axisNavNsInv->GetValue();
	cfg.m_GamePad.m_AxisNavEw = m_axisNavEw->GetValue();
	cfg.m_GamePad.m_AxisNavEwInv = m_axisNavEwInv->GetValue();
	cfg.m_GamePad.m_AxisYawSensitivity = m_axisYawSensitivity;
	cfg.m_GamePad.m_AxisPitchSensitivity = m_axisPitchSensitivity;

	return cfg;
}

void ControlsDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}