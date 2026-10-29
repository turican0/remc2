#include "GraphicsDialog.h"

// ── GraphicsDialog ────────────────────────────────────────────────────────────
GraphicsDialog::GraphicsDialog(wxWindow* parent, const Config::Settings::Graphics& cfg)
	: wxDialog(parent, wxID_ANY, "Graphics Settings",
		wxDefaultPosition, wxSize(210, 340),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;

	auto* main = new wxBoxSizer(wxVERTICAL);
	auto* grid = new wxFlexGridSizer(/*rows*/5, /*cols*/2, /*vgap*/10, /*hgap*/12);
	grid->AddGrowableCol(1, 1);

	// ── Sky ───────────────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Sky On:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkSky = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkSky->SetValue(cfg.m_GameDetail.m_Sky);
	m_chkSky->SetToolTip("Turns the sky on or off. Can be changed in-game.");
	grid->Add(m_chkSky, 0);

	// ── Reflections ───────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Reflections On:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkReflections = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkReflections->SetValue(cfg.m_GameDetail.m_Reflections);
	m_chkReflections->SetToolTip("Turns the reflections on or off. Can be changed in-game.");
	grid->Add(m_chkReflections, 0);

	// ── Dynamic Lighting ──────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Dynamic lighting:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkDynamicLighting = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkDynamicLighting->SetValue(cfg.m_GameDetail.m_DynamicLighting);
	m_chkDynamicLighting->SetToolTip("Turns the dynamic terrain lighting on or off. Can be changed in-game");
	grid->Add(m_chkDynamicLighting, 0);

	// ── View Distance Scale ───────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Increase View Distance"),
		0, wxALIGN_CENTER_VERTICAL);
	m_spinViewDistanceScale = new wxSpinCtrl(this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 1, 6, cfg.m_GameDetail.m_ViewDistanceScale);
	m_spinViewDistanceScale->SetToolTip("Increases the view distance (will effect performance). '1' = Original distance, '2' = 2x, up to '6' = 6x (Max)");
	grid->Add(m_spinViewDistanceScale, 0, wxEXPAND);

	// ── Fog Scale ─────────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Fog Scale:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_spinFogScale = new wxSpinCtrlDouble(this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0.1, 2.0, cfg.m_GameDetail.m_FogScale, 0.1);
	m_spinFogScale->SetDigits(1);
	m_spinFogScale->SetToolTip("Moves the fog (NG renderer). '1.0' = Original, '2.0' = twice as far (up to the view distance), '0.5' = closer");
	grid->Add(m_spinFogScale, 0, wxEXPAND);

	main->Add(grid, 1, wxEXPAND | wxALL, 14);
	main->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 14);

	// ── Render Threads button ─────────────────────────────────────────────────
	auto* btnRenderThreads = new wxButton(this, wxID_ANY, "Render Threads...");
	main->Add(btnRenderThreads, 0, wxALIGN_CENTER | wxLEFT | wxTOP | wxBOTTOM, 10);
	btnRenderThreads->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		ThreadingDialog dlg(this, m_cfg.m_Threading);
		dlg.SetMinSize(wxSize(286, 220));
		if (dlg.ShowModal() == wxID_OK)
		{
			auto threadingSettings = dlg.GetSettings();
			m_cfg.m_Threading.m_IsActive = threadingSettings.m_IsActive;
			m_cfg.m_Threading.m_NumberOfRenderThreads = threadingSettings.m_NumberOfRenderThreads;
			m_cfg.m_Threading.m_AssignToSpecificCores = threadingSettings.m_AssignToSpecificCores;
		}
		});

	main->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 14);

	// ── Enhanced Graphics button ──────────────────────────────────────────────
	auto* btnEnhanced = new wxButton(this, wxID_ANY, "Enhanced Graphics...");
	main->Add(btnEnhanced, 0, wxALIGN_CENTER | wxLEFT | wxTOP | wxBOTTOM, 10);
	btnEnhanced->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		GraphicsFoldersDialog dlg(this, m_cfg.m_GameDetail);
		dlg.SetMinSize(wxSize(600, 260));
		if (dlg.ShowModal() == wxID_OK)
		{
			auto folderSettings = dlg.GetFolders();
			m_cfg.m_GameDetail.m_UseHighResGraphics = folderSettings.m_UseHighResGraphics;
			m_cfg.m_GameDetail.m_HighResGraphicsFolder = folderSettings.m_HighResGraphicsFolder;
			m_cfg.m_GameDetail.m_UseFixedMenuGraphics = folderSettings.m_UseFixedMenuGraphics;
			m_cfg.m_GameDetail.m_FixedMenuGraphicsFolder = folderSettings.m_FixedMenuGraphicsFolder;
			m_cfg.m_GameDetail.m_UseFixedTmaps = folderSettings.m_UseFixedTmaps;
			m_cfg.m_GameDetail.m_FixedTmapsFolder = folderSettings.m_FixedTmapsFolder;
			m_cfg.m_GameDetail.m_UseExtendedFonts = folderSettings.m_UseExtendedFonts;
			m_cfg.m_GameDetail.m_ExtendedFontsFolder = folderSettings.m_ExtendedFontsFolder;
		}
		});

	main->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 14);

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
	btnOK->Bind(wxEVT_BUTTON, &GraphicsDialog::OnOK, this);

	Fit();
	Centre();
}

void GraphicsDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

Config::Settings::Graphics GraphicsDialog::GetSettings() const
{
	Config::Settings::Graphics cfg = m_cfg;

	cfg.m_GameDetail.m_Sky = m_chkSky->GetValue();
	cfg.m_GameDetail.m_Reflections = m_chkReflections->GetValue();
	cfg.m_GameDetail.m_DynamicLighting = m_chkDynamicLighting->GetValue();
	cfg.m_GameDetail.m_ViewDistanceScale = m_spinViewDistanceScale->GetValue();
	cfg.m_GameDetail.m_FogScale = static_cast<float>(m_spinFogScale->GetValue());

	return cfg;
}
