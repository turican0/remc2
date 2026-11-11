#include "UiAlignmentDialog.h"
#include <algorithm>
#include <cctype>

const char* const UiAlignmentDialog::s_alignments[] = { "left", "center", "right" };

// Largest margin the spin controls accept, in screen pixels
static constexpr int MAX_MARGIN = 4000;

int UiAlignmentDialog::FindAlignmentIndex(const std::string& alignment)
{
	std::string value = alignment;
	std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return (char)std::tolower(c); });
	if (value == "centre")   // also read by the game
		value = "center";

	for (int i = 0; i < (int)std::size(s_alignments); i++)
		if (value == s_alignments[i])
			return i;
	return -1;
}

// ── UiAlignmentDialog ─────────────────────────────────────────────────────────
UiAlignmentDialog::UiAlignmentDialog(wxWindow* parent, const Config::Settings::InGameUi& cfg)
	: wxDialog(parent, wxID_ANY, "UI Alignment",
		wxDefaultPosition, wxDefaultSize,
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;

	auto* main = new wxBoxSizer(wxVERTICAL);

	auto* info = new wxStaticText(this, wxID_ANY,
		"Position of the in-game UI elements on the screen (Not used at oringal resolutions)");
	info->Wrap(420);
	main->Add(info, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 14);

	auto* grid = new wxFlexGridSizer(/*cols*/4, /*vgap*/10, /*hgap*/12);
	grid->AddGrowableCol(1, 1);

	// ── Column headers ────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, wxEmptyString));
	grid->Add(new wxStaticText(this, wxID_ANY, "Alignment"), 0, wxALIGN_CENTER_VERTICAL);
	grid->Add(new wxStaticText(this, wxID_ANY, "Margin X"), 0, wxALIGN_CENTER_VERTICAL);
	grid->Add(new wxStaticText(this, wxID_ANY, "Margin Y"), 0, wxALIGN_CENTER_VERTICAL);

	// ── One row per element ───────────────────────────────────────────────────
	m_topBar = AddElementRow(grid, "Top bar:",
		"The mini-map and the health and mana bars.",
		"Pixels down from the top of the screen.",
		cfg.m_TopBar);
	m_spellIcons = AddElementRow(grid, "Spell icons:",
		"The selected left and right spell icons.",
		"Pixels down from the top of the screen.",
		cfg.m_SpellIcons);
	m_spellMenu = AddElementRow(grid, "Spell menu:",
		"The spell selection menu at the bottom of the screen.",
		"Pixels up from the bottom of the screen.",
		cfg.m_SpellMenu);

	main->Add(grid, 1, wxEXPAND | wxALL, 14);

	// ── Reset to defaults ─────────────────────────────────────────────────────
	auto* btnDefaults = new wxButton(this, wxID_ANY, "Reset to Defaults");
	btnDefaults->SetToolTip("Top bar on the left, spell icons on the right, spell menu in the center, all with no margins.");
	btnDefaults->Bind(wxEVT_BUTTON, &UiAlignmentDialog::OnResetDefaults, this);
	main->Add(btnDefaults, 0, wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM, 14);

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
	btnOK->Bind(wxEVT_BUTTON, &UiAlignmentDialog::OnOK, this);

	Fit();
	SetMinSize(GetSize());
	Centre();
}

UiAlignmentDialog::ElementControls UiAlignmentDialog::AddElementRow(wxFlexGridSizer* grid, const wxString& label,
	const wxString& tooltip, const wxString& marginYTooltip, const Config::Settings::UiElement& element)
{
	ElementControls controls;

	auto* text = new wxStaticText(this, wxID_ANY, label);
	text->SetToolTip(tooltip);
	grid->Add(text, 0, wxALIGN_CENTER_VERTICAL);

	controls.alignment = new wxChoice(this, wxID_ANY);
	controls.alignment->Append("Left");
	controls.alignment->Append("Center");
	controls.alignment->Append("Right");
	controls.alignment->SetToolTip(tooltip + " Where it sits across the screen.");
	grid->Add(controls.alignment, 0, wxEXPAND);

	controls.marginX = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1),
		wxSP_ARROW_KEYS, -MAX_MARGIN, MAX_MARGIN, 0);
	controls.marginX->SetToolTip("Pixels in from the left edge (Left), in from the right edge (Right), "
		"or moved from the middle (Center: + = right, - = left).");
	grid->Add(controls.marginX, 0);

	controls.marginY = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1),
		wxSP_ARROW_KEYS, 0, MAX_MARGIN, 0);
	controls.marginY->SetToolTip(marginYTooltip);
	grid->Add(controls.marginY, 0);

	SetElementControls(controls, element);
	return controls;
}

void UiAlignmentDialog::SetElementControls(const ElementControls& controls, const Config::Settings::UiElement& element)
{
	const int index = FindAlignmentIndex(element.m_Alignment);
	controls.alignment->SetSelection(index >= 0 ? index : 0);
	controls.marginX->SetValue((std::clamp)(element.m_MarginX, -MAX_MARGIN, MAX_MARGIN));
	controls.marginY->SetValue((std::clamp)(element.m_MarginY, 0, MAX_MARGIN));
}

Config::Settings::UiElement UiAlignmentDialog::GetElement(const ElementControls& controls, const Config::Settings::UiElement& element)
{
	Config::Settings::UiElement result = element;
	const int index = controls.alignment->GetSelection();
	if (index >= 0 && index < (int)std::size(s_alignments))
		result.m_Alignment = s_alignments[index];
	result.m_MarginX = controls.marginX->GetValue();
	result.m_MarginY = controls.marginY->GetValue();
	return result;
}

void UiAlignmentDialog::OnResetDefaults(wxCommandEvent&)
{
	const Config::Settings::InGameUi defaults;
	SetElementControls(m_topBar, defaults.m_TopBar);
	SetElementControls(m_spellIcons, defaults.m_SpellIcons);
	SetElementControls(m_spellMenu, defaults.m_SpellMenu);
}

void UiAlignmentDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

Config::Settings::InGameUi UiAlignmentDialog::GetSettings() const
{
	Config::Settings::InGameUi cfg = m_cfg;
	cfg.m_TopBar = GetElement(m_topBar, m_cfg.m_TopBar);
	cfg.m_SpellIcons = GetElement(m_spellIcons, m_cfg.m_SpellIcons);
	cfg.m_SpellMenu = GetElement(m_spellMenu, m_cfg.m_SpellMenu);
	return cfg;
}
