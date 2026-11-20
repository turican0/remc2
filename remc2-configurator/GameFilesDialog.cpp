#include "GameFilesDialog.h"
#include "PathHelpers.h"
#include "GameFilesCheck.h"

using GameName = Config::Settings::GameName;
using PathKey = Config::Settings::PathKey;

const GameFilesDialog::GameInfo GameFilesDialog::s_games[] = {
	{ GameName::MagicCarpet, "Magic Carpet", "the CARPET.CD install folder" },
	{ GameName::HiddenWorlds, "Magic Carpet Hidden Worlds", "the CARPET.CD install folder" },
	{ GameName::MagicCarpet2, "Magic Carpet 2", "the NETHERW folder" },
};

GameFilesDialog::GameFilesDialog(wxWindow* parent, const Config::Settings::Paths& p)
	: wxDialog(parent, wxID_ANY, "Select Game Files Folders",
		wxDefaultPosition, wxSize(430, 230), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX), m_paths(p), m_originalPaths(p)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// ── Game drop down: the folders below are those of the chosen game ───────
	wxStaticText* gameLabel = new wxStaticText(panel, wxID_ANY, "Game:");
	m_gameChoice = new wxChoice(panel, wxID_ANY);
	for (const auto& game : s_games)
		m_gameChoice->Append(game.label);
	m_gameChoice->SetToolTip("The game whose folders are shown below");
	wxBoxSizer* gameRow = new wxBoxSizer(wxHORIZONTAL);
	gameRow->Add(gameLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	gameRow->Add(m_gameChoice, wxSizerFlags(1).Expand());

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
	mainSizer->Add(gameRow, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->Add(row1, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->Add(row2, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->AddStretchSpacer(1);
	mainSizer->Add(btnSizer, wxSizerFlags(0).Expand().Border(wxALL, 8));

	panel->SetSizer(mainSizer);

	wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
	dialogSizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(dialogSizer);

	// ── Event bindings ───────────────────────────────────────────────────────
	Bind(wxEVT_BUTTON, &GameFilesDialog::OnBrowseGameFiles, this, ID_BTN_GAME_BROWSE);
	Bind(wxEVT_BUTTON, &GameFilesDialog::OnBrowseCdFiles, this, ID_BTN_CD_BROWSE);
	m_gameChoice->Bind(wxEVT_CHOICE, &GameFilesDialog::OnGameChanged, this);
	Bind(wxEVT_BUTTON, &GameFilesDialog::OnOK, this, wxID_OK);

	m_cdFolderCtrl->SetToolTip("Path to the Contents of the CD");

	// Magic Carpet 2 first, as before the drop down
	m_shownGame = static_cast<int>(std::size(s_games)) - 1;
	m_gameChoice->SetSelection(m_shownGame);
	ShowGame(m_shownGame);

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

void GameFilesDialog::ShowGame(int index)
{
	const GameInfo& game = s_games[index];
	m_gameFolderCtrl->SetValue(m_paths.Get(game.name, PathKey::GameFolder));
	m_gameFolderCtrl->SetToolTip(wxString::Format("Path to %s", game.gameFolderHint));
	m_cdFolderCtrl->SetValue(m_paths.Get(game.name, PathKey::CdFolder));
}

void GameFilesDialog::StoreGame(int index)
{
	m_paths = WithFields(index);
}

Config::Settings::Paths GameFilesDialog::WithFields(int index) const
{
	Config::Settings::Paths p = m_paths;
	const char* name = s_games[index].name;
	p.Set(name, PathKey::GameFolder, m_gameFolderCtrl->GetValue().ToStdString());
	p.Set(name, PathKey::CdFolder, m_cdFolderCtrl->GetValue().ToStdString());
	return p;
}

void GameFilesDialog::OnGameChanged(wxCommandEvent& evt)
{
	const int selected = m_gameChoice->GetSelection();
	if (selected == wxNOT_FOUND || selected == m_shownGame)
		return;
	StoreGame(m_shownGame); // keeps the edits of the game left
	m_shownGame = selected;
	ShowGame(m_shownGame);
}

void GameFilesDialog::OnBrowseGameFiles(wxCommandEvent& evt)
{
	wxDirDialog dlg(this,
		wxString::Format("Select the Game file folder (%s)", s_games[m_shownGame].gameFolderHint),
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

void GameFilesDialog::OnOK(wxCommandEvent& evt)
{
	// the games whose folders changed: what each of them misses
	const Config::Settings::Paths paths = GetPaths();
	wxString problems;
	for (const auto& game : s_games)
	{
		const bool changed = paths.Get(game.name, PathKey::GameFolder) != m_originalPaths.Get(game.name, PathKey::GameFolder)
			|| paths.Get(game.name, PathKey::CdFolder) != m_originalPaths.Get(game.name, PathKey::CdFolder);
		if (!changed)
			continue;
		const auto missing = GameFilesCheck::Missing(paths, game.name);
		if (!missing.empty())
			problems += wxString::Format("%s%s:\n%s\n", problems.empty() ? "" : "\n", game.label, GameFilesCheck::ToText(missing));
	}

	if (!problems.empty())
	{
		const int answer = wxMessageBox(
			"These games will not start with the folders set:\n\n" + problems + "\nSave anyway?",
			"Missing game files", wxYES_NO | wxNO_DEFAULT | wxICON_WARNING, this);
		if (answer != wxYES)
			return; // stays open to correct the folders
	}
	EndModal(wxID_OK);
}

Config::Settings::Paths GameFilesDialog::GetPaths() const
{
	// all games: the edits of the games left and the fields of the shown one
	return WithFields(m_shownGame);
}
