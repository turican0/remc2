#pragma once
#include "Globals.h"

class GameFilesDialog : public wxDialog {
public:
	GameFilesDialog(wxWindow* parent, const Config::Settings::Paths& p);
	Config::Settings::Paths GetPaths() const;

private:
	// Helper that builds one label + text + browse row and appends it to mainSizer
	wxBoxSizer* MakeFolderRow(wxPanel* panel, const wxString& label,
		wxTextCtrl*& outCtrl, int browseId);

	void OnBrowseGameFiles(wxCommandEvent& evt);
	void OnBrowseCdFiles(wxCommandEvent& evt);

	wxTextCtrl* m_gameFolderCtrl = nullptr;
	wxTextCtrl* m_cdFolderCtrl = nullptr;
};
