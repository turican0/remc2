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

	wxButton* m_btnPlayMagicCarpet = nullptr;
	wxButton* m_btnPlayHiddenWorlds = nullptr;
	wxButton* m_btnPlayMagicCarpet2 = nullptr;

	// enables each play button only when its game has the folders and files it needs (the tooltip lists what is missing)
	void UpdatePlayButtons();
	static void UpdatePlayButton(wxButton* button, const std::vector<wxString>& missing);

	void OnPlayMagicCarpet(wxCommandEvent& evt);
	void OnPlayHiddenWorlds(wxCommandEvent& evt);
	void OnPlayMagicCarpet2(wxCommandEvent& evt);
	void Play(const wxString& args);
	void OnLaunchOptions(wxCommandEvent& evt);
	void OnMultiplayer(wxCommandEvent& evt);
	void OnFile(wxCommandEvent& evt);
	void OnSettings(wxCommandEvent&);
	void OnExit(wxCommandEvent& evt);
};