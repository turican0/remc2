#include "HardwareDialog.h"

// MSAA is the only anti-aliasing the hardware renderer supports: it draws without a depth buffer or
// motion vectors, which rules out TAA, and FXAA/SMAA would smear the palette-indexed pixels.
const int HardwareDialog::s_antiAliasingSamples[] = { 0, 2, 4, 8 };
const int HardwareDialog::s_antiAliasingCount = sizeof(s_antiAliasingSamples) / sizeof(s_antiAliasingSamples[0]);

// ── HardwareDialog ────────────────────────────────────────────────────────────
HardwareDialog::HardwareDialog(wxWindow* parent, const Config::Settings::Hardware& cfg)
	: wxDialog(parent, wxID_ANY, "Hardware Settings",
		wxDefaultPosition, wxSize(286, 170),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;

	auto* main = new wxBoxSizer(wxVERTICAL);
	auto* grid = new wxFlexGridSizer(/*rows*/2, /*cols*/2, /*vgap*/10, /*hgap*/12);
	grid->AddGrowableCol(1, 1);

	// ── Bilinear Filtering ────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Bilinear filtering:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkBilinear = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkBilinear->SetValue(cfg.m_BilinearFiltering);
	m_chkBilinear->SetToolTip("Smooths the terrain, sky and sprite textures instead of showing square texels.");
	grid->Add(m_chkBilinear, 0);

	// ── Anti-aliasing ─────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Anti-aliasing:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_choiceAntiAliasing = new wxChoice(this, wxID_ANY);
	int selection = 0;
	for (int i = 0; i < s_antiAliasingCount; i++)
	{
		const int samples = s_antiAliasingSamples[i];
		m_choiceAntiAliasing->Append(samples == 0 ? wxString("Off") : wxString::Format("%dx MSAA", samples));
		if (samples == cfg.m_AntiAliasing)
			selection = i;
	}
	m_choiceAntiAliasing->SetSelection(selection);
	m_choiceAntiAliasing->SetToolTip("Multisample anti-aliasing smooths polygon edges. Higher is smoother but slower; "
		"if the graphics card can't do the chosen level, the highest it supports is used.");
	grid->Add(m_choiceAntiAliasing, 0, wxEXPAND);

	main->Add(grid, 1, wxEXPAND | wxALL, 14);
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
	btnOK->Bind(wxEVT_BUTTON, &HardwareDialog::OnOK, this);

	Fit();
	Centre();
}

void HardwareDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

Config::Settings::Hardware HardwareDialog::GetSettings() const
{
	Config::Settings::Hardware cfg = m_cfg;

	cfg.m_BilinearFiltering = m_chkBilinear->GetValue();
	cfg.m_AntiAliasing = s_antiAliasingSamples[m_choiceAntiAliasing->GetSelection()];

	return cfg;
}
