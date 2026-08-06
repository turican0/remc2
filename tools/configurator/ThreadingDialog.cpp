#include "ThreadingDialog.h"

// ── ThreadingDialog ───────────────────────────────────────────────────────────
ThreadingDialog::ThreadingDialog(wxWindow* parent, const Config::Settings::Threading& cfg)
	: wxDialog(parent, wxID_ANY, "Threading Settings",
		wxDefaultPosition, wxSize(286, 220),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;

	auto* main = new wxBoxSizer(wxVERTICAL);
	auto* grid = new wxFlexGridSizer(/*rows*/4, /*cols*/2, /*vgap*/10, /*hgap*/12);
	grid->AddGrowableCol(1, 1);

	// ── Is Active ─────────────────────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Threading On:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkActive = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkActive->SetValue(cfg.m_IsActive);
	m_chkActive->SetToolTip("If set to false or deleted, multi-threading will not be used in render.");
	m_chkActive->Bind(wxEVT_CHECKBOX, &ThreadingDialog::OnActiveToggled, this);
	grid->Add(m_chkActive, 0);

	// ── Size Percent to Thread Render ─────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Size percentage to thread render:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_spinSizePercent = new wxSpinCtrlDouble(this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0.0, 100.0, cfg.m_SizePercentToThreadRender, 0.5);
	m_spinSizePercent->SetDigits(1);
	m_spinSizePercent->SetToolTip("0.0 - 100.0. Decides if a poly is big enough percentage on screen to render using threading.");
	grid->Add(m_spinSizePercent, 0, wxEXPAND);

	// ── Number of Render Threads ──────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Addtional render threads:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_spinNumThreads = new wxSpinCtrl(this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 1, 7, cfg.m_NumberOfRenderThreads);
	m_spinNumThreads->SetToolTip("Valid range 1-7. Depends on the number of available cores (Press 'T' to change in game).");
	grid->Add(m_spinNumThreads, 0, wxEXPAND);

	// ── Assign to Specific Cores ──────────────────────────────────────────────
	grid->Add(new wxStaticText(this, wxID_ANY, "Assign to specific cores:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_chkAssignCores = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_chkAssignCores->SetValue(cfg.m_AssignToSpecificCores);
	m_chkAssignCores->SetToolTip("When set to true, threads will be assigned to a specifc core (as many as supported).");
	grid->Add(m_chkAssignCores, 0);

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
	btnOK->Bind(wxEVT_BUTTON, &ThreadingDialog::OnOK, this);

	const bool active = m_chkActive->GetValue();
	m_spinSizePercent->Enable(active);
	m_spinNumThreads->Enable(active);
	m_chkAssignCores->Enable(active);

	Fit();
	Centre();
}

void ThreadingDialog::OnActiveToggled(wxCommandEvent&)
{
	const bool active = m_chkActive->GetValue();
	m_spinSizePercent->Enable(active);
	m_spinNumThreads->Enable(active);
	m_chkAssignCores->Enable(active);
}

void ThreadingDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

Config::Settings::Threading ThreadingDialog::GetSettings() const
{
	Config::Settings::Threading cfg = m_cfg;

	cfg.m_IsActive = m_chkActive->GetValue();
	cfg.m_SizePercentToThreadRender = m_spinSizePercent->GetValue();
	cfg.m_NumberOfRenderThreads = m_spinNumThreads->GetValue();
	cfg.m_AssignToSpecificCores = m_chkAssignCores->GetValue();

	return cfg;
}