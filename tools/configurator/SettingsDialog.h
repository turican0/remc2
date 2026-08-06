#pragma once
#include <wx/wx.h>
#include "Globals.h"
#include "GameDialog.h"
#include "ControlsDialog.h"
#include "SoundDialog.h"
#include "DisplayDialog.h"
#include "GraphicsDialog.h"

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(wxWindow* parent, Config* config);

private:
	Config* m_ptrConfig;

	void OnGame(wxCommandEvent&);
	void OnControls(wxCommandEvent&);
	void OnSound(wxCommandEvent&);
	void OnDisplay(wxCommandEvent&);
	void OnGraphics(wxCommandEvent&);
};