#include "GameDialog.h"
#include "PathHelpers.h"
#include <wx/checkbox.h>
#include <wx/dirdlg.h>

GameDialog::GameDialog(wxWindow* parent, const Config::Settings::Game& g)
	: wxDialog(parent, wxID_ANY, "Speed Settings",
		wxDefaultPosition, wxSize(286, 194), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// ── Max Game FPS row ─────────────────────────────────────────────────────
	auto* maxGameLabel = new wxStaticText(panel, wxID_ANY, "Game Frames per Second (24 default):");
	m_maxGameFps = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0, 340, g.m_MaxGameFps);
	m_maxGameFps->SetToolTip("Frames Per Second for game. Locks the frame rate. Set to 0 to unlock.");
	wxBoxSizer* maxRow = new wxBoxSizer(wxHORIZONTAL);
	maxRow->Add(maxGameLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	maxRow->Add(m_maxGameFps, wxSizerFlags(0));

	// ── Skip Intro checkbox ──────────────────────────────────────────────────
	m_skipIntro = new wxCheckBox(panel, wxID_ANY, "Skip Intro");
	m_skipIntro->SetValue(g.m_SkipIntro);
	m_skipIntro->SetToolTip("Skip the intro sequence on startup.");

	// ── OK / Cancel ──────────────────────────────────────────────────────────
	wxStdDialogButtonSizer* btnSizer = new wxStdDialogButtonSizer();
	btnSizer->AddButton(new wxButton(panel, wxID_OK));
	btnSizer->AddButton(new wxButton(panel, wxID_CANCEL));
	btnSizer->Realize();

	// ── Main sizer ───────────────────────────────────────────────────────────
	const wxSizerFlags rowFlags = wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(maxRow, rowFlags);
	mainSizer->Add(m_skipIntro, wxSizerFlags(0).Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->AddStretchSpacer(1);
	mainSizer->Add(btnSizer, wxSizerFlags(0).Expand().Border(wxALL, 8));
	panel->SetSizer(mainSizer);

	wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
	dialogSizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(dialogSizer);
	Centre();
}

Config::Settings::Game GameDialog::GetSettings() const
{
	Config::Settings::Game g;
	g.m_MaxGameFps = m_maxGameFps->GetValue();
	g.m_SkipIntro = m_skipIntro->GetValue();
	return g;
}