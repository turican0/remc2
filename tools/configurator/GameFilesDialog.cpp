#include "GameFilesDialog.h"
#include "PathHelpers.h"

GameFilesDialog::GameFilesDialog(wxWindow* parent, const Config::Settings::Paths& p)
	: wxDialog(parent, wxID_ANY, "Select Game Files Folders",
		wxDefaultPosition, wxSize(430, 195), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// ── Two folder rows ──────────────────────────────────────────────────────
	wxBoxSizer* row1 = MakeFolderRow(panel, "Game Folder:", m_gameFolderCtrl, ID_BTN_GAME_BROWSE);
	wxBoxSizer* row2 = MakeFolderRow(panel, "CD Folder:", m_cdFolderCtrl, ID_BTN_CD_BROWSE);

	// ── OK / Cancel ──────────────────────────────────────────────────────────
	wxStdDialogButtonSizer* btnSizer = new wxStdDialogButtonSizer();
	btnSizer->AddButton(new wxButton(panel, wxID_OK));
	btnSizer->AddButton(new wxButton(panel, wxID_CANCEL));
	btnSizer->Realize();

	// ── Main sizer ───────────────────────────────────────────────────────────
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(row1, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->Add(row2, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->AddStretchSpacer(1);
	mainSizer->Add(btnSizer, wxSizerFlags(0).Expand().Border(wxALL, 8));

	panel->SetSizer(mainSizer);

	wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
	dialogSizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(dialogSizer);

	// ── Event bindings (both Browse buttons share one handler) ───────────────
	Bind(wxEVT_BUTTON, &GameFilesDialog::OnBrowseGameFiles, this, ID_BTN_GAME_BROWSE);
	Bind(wxEVT_BUTTON, &GameFilesDialog::OnBrowseCdFiles, this, ID_BTN_CD_BROWSE);

	m_gameFolderCtrl->SetValue(p.m_GameFolder);
	m_gameFolderCtrl->SetToolTip("Path to the NETHERW Folder");
	m_cdFolderCtrl->SetValue(p.m_CdFolder);
	m_cdFolderCtrl->SetToolTip("Path to the Contents of the CD");

	Centre();
}

wxBoxSizer* GameFilesDialog::MakeFolderRow(wxPanel* panel, const wxString& labelText,
	wxTextCtrl*& outCtrl, int browseId)
{
	wxStaticText* label = new wxStaticText(panel, wxID_ANY, labelText);
	outCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize);
	wxButton* browse = new wxButton(panel, browseId, "Browse...");

	wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(label, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	row->Add(outCtrl, wxSizerFlags(1).Expand().Border(wxRIGHT, 6));
	row->Add(browse, wxSizerFlags(0).CentreVertical());
	return row;
}

void GameFilesDialog::OnBrowseGameFiles(wxCommandEvent& evt)
{
	wxDirDialog dlg(this,
		"Select the Game file folder (usually NETHERW)",
		m_gameFolderCtrl->GetValue(),
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
		m_gameFolderCtrl->SetValue(ToRelative(dlg.GetPath()));
}

void GameFilesDialog::OnBrowseCdFiles(wxCommandEvent& evt)
{
	wxDirDialog dlg(this,
		"Select CD files folder (usually CD_Files)",
		m_cdFolderCtrl->GetValue(),
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
		m_cdFolderCtrl->SetValue(ToRelative(dlg.GetPath()));
}

Config::Settings::Paths GameFilesDialog::GetPaths() const
{
	Config::Settings::Paths p;
	p.m_GameFolder = m_gameFolderCtrl->GetValue();
	p.m_CdFolder = m_cdFolderCtrl->GetValue();
	return p;
}

