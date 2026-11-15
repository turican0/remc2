#include "LookingGlassDialog.h"

// Looking Glass's own quilt settings for each device. A cell (one view, drawn at the game resolution) is the quilt
// divided by the grid.
const LookingGlassDialog::Preset LookingGlassDialog::s_presets[] = {
	{ "Looking Glass Go",                                 11, 6, 4092, 4092 },
	{ "Looking Glass Portrait",                            8, 6, 3360, 3360 },
	{ "Looking Glass 16\" Light Field Display (Landscape)", 7, 7, 5999, 5999 },
	{ "Looking Glass 16\" Light Field Display (Portrait)", 11, 6, 5995, 6000 },
	{ "Looking Glass 27\" Light Field Display (Landscape)", 8, 6, 7680, 4320 },
	{ "Looking Glass 27\" Light Field Display (Portrait)", 12, 4, 7680, 4320 },
	{ "Looking Glass 32\" Light Field Display (Landscape)", 7, 7, 8190, 8190 },
	{ "Looking Glass 32\" Light Field Display (Portrait)", 11, 6, 8184, 8184 },
	{ "Looking Glass 65\" Light Field Display",            8, 9, 8192, 8192 },
};
const int LookingGlassDialog::s_presetCount = sizeof(s_presets) / sizeof(s_presets[0]);

// The game's limits: a grid of up to 16 x 16 views (MAX_VIEW_GRID), a cell of 320x200 up to 3840x2160 pixels
static const int kMaxGrid = 16;
static const int kMinCellWidth = 320;
static const int kMinCellHeight = 200;
static const int kMaxCellSize = 3840;

// ── LookingGlassDialog ────────────────────────────────────────────────────────
LookingGlassDialog::LookingGlassDialog(wxWindow* parent, const Config::Settings::LookingGlass& cfg)
	: wxDialog(parent, wxID_ANY, "Looking Glass Settings",
		wxDefaultPosition, wxSize(455, 460),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;

	auto* main = new wxBoxSizer(wxVERTICAL);

	// ── Enabled ───────────────────────────────────────────────────────────────
	m_chkEnabled = new wxCheckBox(this, wxID_ANY, "Render for a Looking Glass");
	m_chkEnabled->SetValue(cfg.m_Enabled);
	m_chkEnabled->SetToolTip("Draws a quilt of views for a Looking Glass light field display (hardware renderer only). "
		"The game runs at the cell size: the menus, the UI and each view.");
	main->Add(m_chkEnabled, 0, wxLEFT | wxRIGHT | wxTOP, 14);

	auto* grid = new wxFlexGridSizer(/*cols*/2, /*vgap*/10, /*hgap*/12);
	grid->AddGrowableCol(1, 1);

	// ── Device preset ─────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Device:"), 0, wxALIGN_CENTER_VERTICAL);
	m_choicePreset = new wxChoice(this, wxID_ANY);
	for (int i = 0; i < s_presetCount; i++)
		m_choicePreset->Append(s_presets[i].name);
	m_choicePreset->Append("Custom");
	m_choicePreset->SetToolTip("Fills in the quilt Looking Glass uses for the device.");
	grid->Add(m_choicePreset, 0, wxEXPAND);

	// ── Quilt ─────────────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Columns:"), 0, wxALIGN_CENTER_VERTICAL);
	m_spinColumns = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 1, kMaxGrid, cfg.m_Columns);
	m_spinColumns->SetToolTip("Views across the quilt.");
	grid->Add(m_spinColumns, 0, wxEXPAND);

	grid->Add(new wxStaticText(this, wxID_ANY, "Rows:"), 0, wxALIGN_CENTER_VERTICAL);
	m_spinRows = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 1, kMaxGrid, cfg.m_Rows);
	m_spinRows->SetToolTip("Views down the quilt. The leftmost view is the bottom left cell.");
	grid->Add(m_spinRows, 0, wxEXPAND);

	grid->Add(new wxStaticText(this, wxID_ANY, "Cell size:"), 0, wxALIGN_CENTER_VERTICAL);
	auto* cellRow = new wxBoxSizer(wxHORIZONTAL);
	m_spinCellWidth = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1),
		wxSP_ARROW_KEYS, kMinCellWidth, kMaxCellSize, cfg.m_CellWidth);
	m_spinCellHeight = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1),
		wxSP_ARROW_KEYS, kMinCellHeight, kMaxCellSize, cfg.m_CellHeight);
	m_spinCellWidth->SetToolTip("Width of one view: the game's resolution.");
	m_spinCellHeight->SetToolTip("Height of one view: the game's resolution.");
	cellRow->Add(m_spinCellWidth, 1, wxEXPAND);
	cellRow->Add(new wxStaticText(this, wxID_ANY, " x "), 0, wxALIGN_CENTER_VERTICAL);
	cellRow->Add(m_spinCellHeight, 1, wxEXPAND);
	grid->Add(cellRow, 0, wxEXPAND);

	grid->Add(new wxStaticText(this, wxID_ANY, "Quilt size:"), 0, wxALIGN_CENTER_VERTICAL);
	m_lblQuiltSize = new wxStaticText(this, wxID_ANY, wxEmptyString);
	grid->Add(m_lblQuiltSize, 0, wxALIGN_CENTER_VERTICAL);

	// ── Depth ─────────────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "View spacing:"), 0, wxALIGN_CENTER_VERTICAL);
	m_spinViewSpacing = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0, 256, cfg.m_ViewSpacing);
	m_spinViewSpacing->SetToolTip("Sideways camera step between neighbouring views, in world units (a tile is 256). "
		"More gives more depth.");
	grid->Add(m_spinViewSpacing, 0, wxEXPAND);

	grid->Add(new wxStaticText(this, wxID_ANY, "Convergence:"), 0, wxALIGN_CENTER_VERTICAL);
	m_spinConvergence = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0, 65536, cfg.m_Convergence);
	m_spinConvergence->SetToolTip("Distance in world units that sits at the display surface: nearer pops out, "
		"further sinks in. 0 = parallel views (everything in front of the display).");
	grid->Add(m_spinConvergence, 0, wxEXPAND);

	main->Add(grid, 0, wxEXPAND | wxALL, 14);
	main->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 14);

	// ── Output ────────────────────────────────────────────────────────────────
	m_chkDisplay = new wxCheckBox(this, wxID_ANY, "Show on the Looking Glass");
	m_chkDisplay->SetValue(cfg.m_Display);
	m_chkDisplay->SetToolTip("Shows the hologram fullscreen on the Looking Glass, using its calibration from Looking "
		"Glass Bridge (Windows, Bridge running). Off: the quilt's views side by side in a normal window.");
	main->Add(m_chkDisplay, 0, wxLEFT | wxRIGHT | wxTOP, 14);

	auto* bridgeRow = new wxBoxSizer(wxHORIZONTAL);
	bridgeRow->Add(new wxStaticText(this, wxID_ANY, "Bridge folder:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 12);
	m_txtBridgePath = new wxTextCtrl(this, wxID_ANY, wxString::FromUTF8(cfg.m_BridgePath), wxDefaultPosition, wxSize(220, -1));
	m_txtBridgePath->SetHint("Automatic");
	m_txtBridgePath->SetToolTip("Looking Glass Bridge's install folder. Empty = the newest in C:\\Program Files\\Looking Glass.");
	bridgeRow->Add(m_txtBridgePath, 1, wxEXPAND | wxRIGHT, 6);
	m_btnBrowseBridge = new wxButton(this, wxID_ANY, "...", wxDefaultPosition, wxSize(30, -1));
	bridgeRow->Add(m_btnBrowseBridge, 0);
	main->Add(bridgeRow, 0, wxEXPAND | wxALL, 14);

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

	m_chkEnabled->Bind(wxEVT_CHECKBOX, &LookingGlassDialog::OnEnabledChanged, this);
	m_chkDisplay->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent&) { UpdateEnabled(); });
	m_choicePreset->Bind(wxEVT_CHOICE, &LookingGlassDialog::OnPreset, this);
	for (wxSpinCtrl* spin : { m_spinColumns, m_spinRows, m_spinCellWidth, m_spinCellHeight })
		spin->Bind(wxEVT_SPINCTRL, &LookingGlassDialog::OnQuiltChanged, this);
	m_btnBrowseBridge->Bind(wxEVT_BUTTON, &LookingGlassDialog::OnBrowseBridge, this);
	btnOK->Bind(wxEVT_BUTTON, &LookingGlassDialog::OnOK, this);

	SelectMatchingPreset();
	UpdateQuiltSize();
	UpdateEnabled();
	Fit();
	Centre();
}

void LookingGlassDialog::OnPreset(wxCommandEvent&)
{
	const int selection = m_choicePreset->GetSelection();
	if (selection < 0 || selection >= s_presetCount)
		return;   // Custom: keep the values to edit
	const Preset& preset = s_presets[selection];
	m_spinColumns->SetValue(preset.columns);
	m_spinRows->SetValue(preset.rows);
	m_spinCellWidth->SetValue(preset.quiltWidth / preset.columns);
	m_spinCellHeight->SetValue(preset.quiltHeight / preset.rows);
	UpdateQuiltSize();
}

void LookingGlassDialog::OnQuiltChanged(wxSpinEvent&)
{
	SelectMatchingPreset();
	UpdateQuiltSize();
}

void LookingGlassDialog::SelectMatchingPreset()
{
	const int columns = m_spinColumns->GetValue();
	const int rows = m_spinRows->GetValue();
	const int cellWidth = m_spinCellWidth->GetValue();
	const int cellHeight = m_spinCellHeight->GetValue();
	for (int i = 0; i < s_presetCount; i++)
	{
		const Preset& preset = s_presets[i];
		if (preset.columns == columns && preset.rows == rows
			&& preset.quiltWidth / preset.columns == cellWidth && preset.quiltHeight / preset.rows == cellHeight)
		{
			m_choicePreset->SetSelection(i);
			return;
		}
	}
	m_choicePreset->SetSelection(CustomIndex());
}

void LookingGlassDialog::UpdateQuiltSize()
{
	const int columns = m_spinColumns->GetValue();
	const int rows = m_spinRows->GetValue();
	m_lblQuiltSize->SetLabel(wxString::Format("%d x %d (%d views)",
		m_spinCellWidth->GetValue() * columns, m_spinCellHeight->GetValue() * rows, columns * rows));
	Layout();
}

// Ticked when it was off: says what turning it on also sets (applied when this dialog is OK'd)
void LookingGlassDialog::OnEnabledChanged(wxCommandEvent&)
{
	UpdateEnabled();
	if (!TurnedOn())
		return;

	wxMessageBox(wxString::Format(
		"For the Looking Glass, these graphics settings have been auto-changed:\n"
		"-View Distance: 1x\n"
		"-In-game UI margins: 25 pixels\n"
		"They are applied when you press OK in the Looking Glass Settings Dialog",
		kViewDistanceScale, kUiMargin),
		"Game Detail Settings auto changed!", wxOK | wxICON_INFORMATION, this);
}

void LookingGlassDialog::UpdateEnabled()
{
	const bool enabled = m_chkEnabled->GetValue();
	for (wxWindow* control : std::initializer_list<wxWindow*>{ m_choicePreset, m_spinColumns, m_spinRows, m_spinCellWidth,
		m_spinCellHeight, m_spinViewSpacing, m_spinConvergence, m_chkDisplay })
		control->Enable(enabled);
	const bool bridge = enabled && m_chkDisplay->GetValue();
	m_txtBridgePath->Enable(bridge);
	m_btnBrowseBridge->Enable(bridge);
}

void LookingGlassDialog::OnBrowseBridge(wxCommandEvent&)
{
	const wxString start = m_txtBridgePath->GetValue().IsEmpty() ? wxString("C:\\Program Files\\Looking Glass") : m_txtBridgePath->GetValue();
	wxDirDialog dlg(this, "Looking Glass Bridge's folder (with bridge_inproc.dll)", start, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
		m_txtBridgePath->SetValue(dlg.GetPath());
}

void LookingGlassDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

Config::Settings::LookingGlass LookingGlassDialog::GetSettings() const
{
	Config::Settings::LookingGlass cfg = m_cfg;

	cfg.m_Enabled = m_chkEnabled->GetValue();
	cfg.m_Columns = m_spinColumns->GetValue();
	cfg.m_Rows = m_spinRows->GetValue();
	cfg.m_CellWidth = m_spinCellWidth->GetValue();
	cfg.m_CellHeight = m_spinCellHeight->GetValue();
	cfg.m_ViewSpacing = m_spinViewSpacing->GetValue();
	cfg.m_Convergence = m_spinConvergence->GetValue();
	cfg.m_Display = m_chkDisplay->GetValue();
	cfg.m_BridgePath = m_txtBridgePath->GetValue().ToUTF8().data();

	return cfg;
}
