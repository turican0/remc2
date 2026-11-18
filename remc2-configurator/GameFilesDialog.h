#pragma once
#include "Globals.h"
#include <wx/choice.h>

class GameFilesDialog : public wxDialog {
public:
	GameFilesDialog(wxWindow* parent, const Config::Settings::Paths& p);
	Config::Settings::Paths GetPaths() const;

private:
	// the games of the drop down, in its order
	struct GameInfo
	{
		const char* name;          // the "name" of the paths entry
		const char* label;         // shown in the drop down
		const char* gameFolderHint; // what the game folder is
	};
	static const GameInfo s_games[];

	// Helper that builds one label + text + browse row and appends it to mainSizer
	wxBoxSizer* MakeFolderRow(wxPanel* panel, const wxString& label,
		wxTextCtrl*& outCtrl, int browseId);

	void OnBrowseGameFiles(wxCommandEvent& evt);
	void OnBrowseCdFiles(wxCommandEvent& evt);
	void OnGameChanged(wxCommandEvent& evt);
	void OnOK(wxCommandEvent& evt); // checks the games whose folders changed before it saves

	void ShowGame(int index);  // the fields from the paths of the game
	void StoreGame(int index); // the fields to the paths of the game
	Config::Settings::Paths WithFields(int index) const;

	wxChoice* m_gameChoice = nullptr;
	wxTextCtrl* m_gameFolderCtrl = nullptr;
	wxTextCtrl* m_cdFolderCtrl = nullptr;
	Config::Settings::Paths m_paths; // all games, the shown one as it was when it was selected
	Config::Settings::Paths m_originalPaths; // as the dialog opened
	int m_shownGame = 0;
};
