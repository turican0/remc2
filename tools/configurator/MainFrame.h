#pragma once
#include "Globals.h"
#include "GameFilesDialog.h"
#include "PathHelpers.h"
#include "MultiplayerDialog.h"
#include "LaunchOptionsDialog.h"
#include "SettingsDialog.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const std::string fileName);

private:

	Config* m_ptrConfig = nullptr;

	void OnPlay(wxCommandEvent& evt);
	void OnLaunchOptions(wxCommandEvent& evt);
	void OnMultiplayer(wxCommandEvent& evt);
	void OnFile(wxCommandEvent& evt);
	void OnSettings(wxCommandEvent&);
	void OnExit(wxCommandEvent& evt);
};