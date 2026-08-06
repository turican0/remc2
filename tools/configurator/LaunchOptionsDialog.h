#pragma once
#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/filedlg.h>

class LaunchOptionsDialog : public wxDialog
{
public:
	LaunchOptionsDialog(wxWindow* parent, std::string launchArguments);
	std::string GetLaunchArguments() const;

private:
	wxTextCtrl* m_extraArgs;
	wxCheckBox* m_setLevel;
	wxSpinCtrl* m_levelSpin;
	wxTextCtrl* m_customMap;
	wxButton* m_browseMapBtn;
	wxRadioButton* m_radioRecord;
	wxRadioButton* m_radioPlay;
	wxTextCtrl* m_recordPlayFile;
	wxChoice* m_logLevel;
	wxCheckBox* m_enableInGameDebug;
	wxStaticText* m_commandLineLabel;

	void BuildCommandLine();
	void ParseAndApplyArguments(const std::string& args);

	void OnControlChanged(wxCommandEvent&);
	void OnSetLevel(wxCommandEvent&);
	void OnBrowseMap(wxCommandEvent&);
	void OnBrowseRecordPlay(wxCommandEvent&);
	void OnSave(wxCommandEvent&);
	void OnOK(wxCommandEvent&);
};