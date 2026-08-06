#include "DisplayDialog.h"
#include "GraphicsFoldersDialog.h"

// ── Resolution table ─────────────────────────────────────────────────────────
const DisplayDialog::ResEntry DisplayDialog::s_resolutions[] =
{
	{ "640 x 480",   640,  480  },
	{ "800 x 600",   800,  600  },
	{ "1024 x 768",  1024, 768  },
	{ "1280 x 720",  1280, 720  },
	{ "1280 x 1024", 1280, 1024 },
	{ "1366 x 768",  1366, 768  },
	{ "1600 x 900",  1600, 900  },
	{ "1920 x 1080", 1920, 1080 },
	{ "2560 x 1440", 2560, 1440 },
	{ "3840 x 2160", 3840, 2160 },
};

int DisplayDialog::FindResIndex(int w, int h)
{
	for (int i = 0; i < (int)std::size(s_resolutions); ++i)
		if (s_resolutions[i].w == w && s_resolutions[i].h == h)
			return i;
	return (int)std::size(s_resolutions); // points to the "Custom..." entry
}

// ── Shared helper: show the custom-resolution sub-dialog ─────────────────────
// Returns true if the user confirmed; updates w/h and refreshes the choice label.
static bool ShowCustomResDlg(wxWindow* parent, wxChoice* choice, int customIdx,
	int& w, int& h)
{
	wxDialog dlg(parent, wxID_ANY, "Custom Resolution",
		wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

	auto* sizer = new wxBoxSizer(wxVERTICAL);
	auto* row = new wxBoxSizer(wxHORIZONTAL);

	auto* spinW = new wxSpinCtrl(&dlg, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(90, -1), wxSP_ARROW_KEYS, 320, 7680, w);
	auto* spinH = new wxSpinCtrl(&dlg, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(90, -1), wxSP_ARROW_KEYS, 240, 4320, h);

	row->Add(new wxStaticText(&dlg, wxID_ANY, "Width:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 6);
	row->Add(spinW, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 12);
	row->Add(new wxStaticText(&dlg, wxID_ANY, "Height:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 6);
	row->Add(spinH, 0, wxALIGN_CENTER_VERTICAL);

	auto* btnSizer = new wxStdDialogButtonSizer();
	auto* btnOK = new wxButton(&dlg, wxID_OK, "OK");
	btnOK->SetDefault();
	btnSizer->AddButton(btnOK);
	btnSizer->AddButton(new wxButton(&dlg, wxID_CANCEL, "Cancel"));
	btnSizer->Realize();

	sizer->Add(row, 0, wxEXPAND | wxALL, 14);
	sizer->Add(new wxStaticLine(&dlg), 0, wxEXPAND | wxLEFT | wxRIGHT, 14);
	sizer->Add(btnSizer, 0, wxEXPAND | wxALL, 10);
	dlg.SetSizerAndFit(sizer);
	dlg.Centre();

	if (dlg.ShowModal() == wxID_OK)
	{
		w = spinW->GetValue();
		h = spinH->GetValue();
		choice->SetString(customIdx,
			wxString::Format("%d x %d (custom)", w, h));
		return true;
	}
	return false;
}

// ── DisplayDialog ────────────────────────────────────────────────────────────
DisplayDialog::DisplayDialog(wxWindow* parent, const Config::Settings::Graphics& cfg)
	: wxDialog(parent, wxID_ANY, "Display Settings",
		wxDefaultPosition, wxSize(270, 300), 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;
	m_customWidth = cfg.m_WindowResWidth;
	m_customHeight = cfg.m_WindowResHeight;
	m_customIGWidth = cfg.m_GameDetail.m_GameResWidth;
	m_customIGHeight = cfg.m_GameDetail.m_GameResHeight;

	auto* main = new wxBoxSizer(wxVERTICAL);
	auto* grid = new wxFlexGridSizer(/*rows*/6, /*cols*/2, /*vgap*/10, /*hgap*/12);
	grid->AddGrowableCol(1, 1);

	// ── Display index ────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Display index:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_spinDisplay = new wxSpinCtrl(this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0, wxDisplay::GetCount() - 1, cfg.m_DisplayIndex);
	m_spinDisplay->SetToolTip(
		"Decides which display to use. If it cannot find a display at the "
		"index, it will use the first one big enough.");
	grid->Add(m_spinDisplay, 0, wxEXPAND);

	// ── Build the shared resolution choices array once ────────────────────────
	wxArrayString choices;
	for (const auto& r : s_resolutions)
		choices.Add(r.label);
	choices.Add("Custom...");
	const int customIdx = (int)std::size(s_resolutions);

	// ── Window resolution ─────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Window resolution:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_choiceRes = new wxChoice(this, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, choices);
	m_choiceRes->SetSelection(FindResIndex(cfg.m_WindowResWidth, cfg.m_WindowResHeight));
	m_choiceRes->Bind(wxEVT_CHOICE, &DisplayDialog::OnResolutionChanged, this);
	m_choiceRes->SetToolTip(
		"Window resolution. Cannot be greater than the resolution of the chosen display.");
	grid->Add(m_choiceRes, 0, wxEXPAND);

	// ── In-game resolution ────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "In-game resolution:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_choiceInGame = new wxChoice(this, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, choices);
	m_choiceInGame->SetSelection(FindResIndex(cfg.m_GameDetail.m_GameResWidth, cfg.m_GameDetail.m_GameResHeight));
	m_choiceInGame->Bind(wxEVT_CHOICE, &DisplayDialog::OnInGameResolutionChanged, this);
	m_choiceInGame->SetToolTip(
		"Resolution used during gameplay. Can be set lower than the window "
		"resolution for better performance.");
	grid->Add(m_choiceInGame, 0, wxEXPAND);

	// ── UI Scale ──────────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "UI Scale multiplier:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_spinUiScale = new wxSpinCtrl(this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 1, 8, cfg.m_GameDetail.m_GameUiScale);
	m_spinUiScale->SetToolTip(
		"Scale of the Game Mini/Map and UI '2' recommended for HD, 3 for higher. "
		"(Max 8) Will not allow spell menu to be wider than screen width.");
	grid->Add(m_spinUiScale, 0, wxEXPAND);

	// ── Maintain aspect ratio ─────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Maintain aspect ratio:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkAspect = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkAspect->SetValue(cfg.m_MaintainAspectRatio);
	m_chkAspect->SetToolTip(
		"If false, the whole window will be used for menu screens etc., stretching content.");
	grid->Add(m_chkAspect, 0);

	// ── Start windowed ────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Start windowed:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkWindowed = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkWindowed->SetValue(cfg.m_StartWindowed);
	m_chkWindowed->SetToolTip(
		"Start in windowed mode. Use Alt+Enter to toggle in-game.");
	grid->Add(m_chkWindowed, 0);

	main->Add(grid, 1, wxEXPAND | wxALL, 14);

	// ── OK / Cancel ───────────────────────────────────────────────────────────
	auto* btnSizer = new wxStdDialogButtonSizer();
	auto* btnOK = new wxButton(this, wxID_OK, "OK");
	auto* btnCancel = new wxButton(this, wxID_CANCEL, "Cancel");
	btnOK->SetDefault();
	btnSizer->AddButton(btnOK);
	btnSizer->AddButton(btnCancel);
	btnSizer->Realize();
	main->Add(btnSizer, 0, wxEXPAND | wxALL, 10);

	SetSizer(main);
	btnOK->Bind(wxEVT_BUTTON, &DisplayDialog::OnOK, this);

	Fit();
	Centre();
}

void DisplayDialog::OnResolutionChanged(wxCommandEvent&)
{
	const int customIdx = (int)std::size(s_resolutions);
	if (m_choiceRes->GetSelection() != customIdx)
		return;

	if (!ShowCustomResDlg(this, m_choiceRes, customIdx, m_customWidth, m_customHeight))
		m_choiceRes->SetSelection(FindResIndex(m_customWidth, m_customHeight));
}

void DisplayDialog::OnInGameResolutionChanged(wxCommandEvent&)
{
	const int customIdx = (int)std::size(s_resolutions);
	if (m_choiceInGame->GetSelection() != customIdx)
		return;

	if (!ShowCustomResDlg(this, m_choiceInGame, customIdx, m_customIGWidth, m_customIGHeight))
		m_choiceInGame->SetSelection(FindResIndex(m_customIGWidth, m_customIGHeight));
}

void DisplayDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

Config::Settings::Graphics DisplayDialog::GetSettings() const
{
	Config::Settings::Graphics cfg = m_cfg;

	cfg.m_DisplayIndex = m_spinDisplay->GetValue();

	const int customIdx = (int)std::size(s_resolutions);

	// Window resolution
	{
		int sel = m_choiceRes->GetSelection();
		if (sel == customIdx) {
			cfg.m_WindowResWidth = m_customWidth;
			cfg.m_WindowResHeight = m_customHeight;
		}
		else if (sel >= 0 && sel < customIdx) {
			cfg.m_WindowResWidth = s_resolutions[sel].w;
			cfg.m_WindowResHeight = s_resolutions[sel].h;
		}
	}

	// In-game resolution
	{
		int sel = m_choiceInGame->GetSelection();
		if (sel == customIdx) {
			cfg.m_GameDetail.m_GameResWidth = m_customIGWidth;
			cfg.m_GameDetail.m_GameResHeight = m_customIGHeight;
		}
		else if (sel >= 0 && sel < customIdx) {
			cfg.m_GameDetail.m_GameResWidth = s_resolutions[sel].w;
			cfg.m_GameDetail.m_GameResHeight = s_resolutions[sel].h;
		}
	}

	cfg.m_GameDetail.m_GameUiScale = m_spinUiScale->GetValue();
	cfg.m_MaintainAspectRatio = m_chkAspect->GetValue();
	cfg.m_StartWindowed = m_chkWindowed->GetValue();
	return cfg;
}