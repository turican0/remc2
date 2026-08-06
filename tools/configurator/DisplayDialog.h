#pragma once
#include "Globals.h"
#include <wx/statline.h>
#include <wx/display.h>

class DisplayDialog : public wxDialog
{
public:
	DisplayDialog(wxWindow* parent, const Config::Settings::Graphics& cfg);
	Config::Settings::Graphics GetSettings() const;

private:
	// Controls
	wxSpinCtrl* m_spinDisplay;
	wxSpinCtrl* m_spinUiScale;
	wxChoice* m_choiceRes;
	wxChoice* m_choiceInGame;
	wxCheckBox* m_chkAspect;
	wxCheckBox* m_chkWindowed;

	Config::Settings::Graphics m_cfg;

	// Tracks custom resolutions entered by the user
	int m_customWidth = 1920, m_customHeight = 1080;
	int m_customIGWidth = 1920, m_customIGHeight = 1080;

	// Helpers
	void OnOK(wxCommandEvent& evt);
	void OnResolutionChanged(wxCommandEvent& evt);
	void OnInGameResolutionChanged(wxCommandEvent& evt);

	// Common resolutions list  { label, width, height }
	struct ResEntry { const char* label; int w; int h; };
	static const ResEntry s_resolutions[];
	static constexpr int CUSTOM_INDEX = -1;
	static int FindResIndex(int w, int h);
};