#include "GraphicsDialog.h"

// ── GraphicsDialog ────────────────────────────────────────────────────────────
GraphicsDialog::GraphicsDialog(wxWindow* parent, const Config::Settings::Graphics& cfg)
	: wxDialog(parent, wxID_ANY, "Graphics Settings",
		wxDefaultPosition, wxSize(245, 440),
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

	// ── Renderer ──────────────────────────────────────────────────────────────
	auto* renderRow = new wxBoxSizer(wxHORIZONTAL);
	renderRow->Add(new wxStaticText(this, wxID_ANY, "Renderer:"),
		0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 12);
	m_choiceRender = new wxChoice(this, wxID_ANY);
	m_choiceRender->Append("Hardware");
	m_choiceRender->Append("Software");
	m_choiceRender->SetSelection(cfg.m_Render == "HW" ? RENDER_HARDWARE : RENDER_SOFTWARE);
	m_choiceRender->SetToolTip("Hardware renders with the GPU (Vulkan). Software renders with the CPU.");
	renderRow->Add(m_choiceRender, 1, wxEXPAND);
	main->Add(renderRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 14);

	// ── Hardware Settings button (hardware renderer only) ─────────────────────
	m_btnHardwareSettings = new wxButton(this, wxID_ANY, "Hardware Settings...");
	main->Add(m_btnHardwareSettings, 0, wxALIGN_CENTER | wxLEFT | wxTOP | wxBOTTOM, 10);

	// ── Looking Glass button (hardware renderer only) ─────────────────────────
	m_btnLookingGlass = new wxButton(this, wxID_ANY, "Looking Glass...");
	m_btnLookingGlass->SetToolTip("Settings for Looking Glass light field displays.");
	main->Add(m_btnLookingGlass, 0, wxALIGN_CENTER | wxLEFT | wxBOTTOM, 10);
	m_btnLookingGlass->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		LookingGlassDialog dlg(this, m_cfg.m_Hardware.m_LookingGlass);
		SetDialogMinSize(dlg, wxSize(455, 460));
		if (dlg.ShowModal() == wxID_OK)
		{
			m_cfg.m_Hardware.m_LookingGlass = dlg.GetSettings();
			// Turned on: the view distance and UI margins it needs (the dialog told the user when it was ticked)
			if (dlg.TurnedOn())
			{
				m_spinViewDistanceScale->SetValue(LookingGlassDialog::kViewDistanceScale);
				auto& ui = m_cfg.m_InGameUi;
				for (Config::Settings::UiElement* element : { &ui.m_TopBar, &ui.m_SpellIcons, &ui.m_SpellMenu })
				{
					// a centred element's X margin is its offset from the middle: it stays centred
					if (element->m_Alignment != "center")
						element->m_MarginX = LookingGlassDialog::kUiMargin;
					element->m_MarginY = LookingGlassDialog::kUiMargin;
				}
			}
		}
		});

	// ── Anaglyph 3D button (hardware renderer only) ───────────────────────────
	m_btnAnaglyph = new wxButton(this, wxID_ANY, "Anaglyph 3D...");
	m_btnAnaglyph->SetToolTip("Lens colours for anaglyph (red / cyan glasses) 3D. F11 in game turns it on and off.");
	main->Add(m_btnAnaglyph, 0, wxALIGN_CENTER | wxLEFT | wxBOTTOM, 10);
	m_btnAnaglyph->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		AnaglyphDialog dlg(this, m_cfg.m_Hardware.m_Anaglyph);
		SetDialogMinSize(dlg, wxSize(675, 650));
		if (dlg.ShowModal() == wxID_OK)
			m_cfg.m_Hardware.m_Anaglyph = dlg.GetSettings();
		});

	// ── Render Threads button (software renderer only) ────────────────────────
	m_btnRenderThreads = new wxButton(this, wxID_ANY, "Render Threads...");
	main->Add(m_btnRenderThreads, 0, wxALIGN_CENTER | wxLEFT | wxTOP | wxBOTTOM, 10);
	m_choiceRender->Bind(wxEVT_CHOICE, [this](wxCommandEvent&) { UpdateRenderButtons(); });
	m_btnHardwareSettings->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		HardwareDialog dlg(this, m_cfg.m_Hardware);
		SetDialogMinSize(dlg, wxSize(286, 170));
		if (dlg.ShowModal() == wxID_OK)
			m_cfg.m_Hardware = dlg.GetSettings();
		});
	m_btnRenderThreads->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		ThreadingDialog dlg(this, m_cfg.m_Threading);
		SetDialogMinSize(dlg, wxSize(286, 220));
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
		SetDialogMinSize(dlg, wxSize(600, 260));
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

	UpdateRenderButtons();
	Fit();
	Centre();
}

void GraphicsDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

void GraphicsDialog::UpdateRenderButtons()
{
	bool hardware = m_choiceRender->GetSelection() == RENDER_HARDWARE;
	m_btnHardwareSettings->Show(hardware);
	m_btnLookingGlass->Show(hardware);
	m_btnAnaglyph->Show(hardware);
	m_btnRenderThreads->Show(!hardware);
	Layout();
}

Config::Settings::Graphics GraphicsDialog::GetSettings() const
{
	Config::Settings::Graphics cfg = m_cfg;

	cfg.m_GameDetail.m_Sky = m_chkSky->GetValue();
	cfg.m_GameDetail.m_Reflections = m_chkReflections->GetValue();
	cfg.m_GameDetail.m_DynamicLighting = m_chkDynamicLighting->GetValue();
	cfg.m_GameDetail.m_ViewDistanceScale = m_spinViewDistanceScale->GetValue();
	cfg.m_GameDetail.m_FogScale = static_cast<float>(m_spinFogScale->GetValue());
	cfg.m_Render = m_choiceRender->GetSelection() == RENDER_HARDWARE ? "HW" : "NG";

	return cfg;
}
