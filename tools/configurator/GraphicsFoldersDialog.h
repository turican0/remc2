#pragma once
#include "Globals.h"
#include <wx/dirdlg.h>

class GraphicsFoldersDialog : public wxDialog
{
public:
	GraphicsFoldersDialog(wxWindow* parent, const Config::Settings::GameDetail& d);
	Config::Settings::GameDetail GetFolders() const;

private:
	wxCheckBox* m_chkHighRes;
	wxTextCtrl* m_graphicsFolderCtrl;
	wxButton* m_btnGraphicsBrowse;

	wxCheckBox* m_chkFixedMenu;
	wxTextCtrl* m_menuFolderCtrl;
	wxButton* m_btnMenuBrowse;

	wxCheckBox* m_chkFonts;
	wxTextCtrl* m_fontFolderCtrl;
	wxButton* m_btnFontsBrowse;

	Config::Settings::GameDetail m_initialDetail;

	struct FolderRow
	{
		wxCheckBox* chk;
		wxTextCtrl* ctrl;
		wxButton* btn;
		wxString    prompt;
	};
	FolderRow m_rows[3];

	wxBoxSizer* MakeFolderRow(wxPanel* panel, const wxString& labelText,
		wxCheckBox*& outChk, int chkId,
		wxTextCtrl*& outCtrl,
		wxButton*& outBtn, int browseId);

	void OnBrowse(wxCommandEvent& evt);
	void OnCheckChanged(wxCommandEvent& evt);
};