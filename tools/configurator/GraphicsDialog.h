#pragma once
#include "Globals.h"
#include <wx/statline.h>
#include <wx/display.h>
#include "GraphicsFoldersDialog.h"
#include "ThreadingDialog.h"

class GraphicsDialog : public wxDialog
{
public:
	GraphicsDialog(wxWindow* parent, const Config::Settings::Graphics& cfg);
	Config::Settings::Graphics GetSettings() const;

private:
	// Controls
	wxCheckBox* m_chkSky;
	wxCheckBox* m_chkReflections;
	wxCheckBox* m_chkDynamicLighting;
	wxSpinCtrl* m_spinViewDistanceScale;

	Config::Settings::Graphics m_cfg;

	// Helpers
	void OnOK(wxCommandEvent& evt);

	// Common resolutions list  { label, width, height }
	struct ResEntry { const char* label; int w; int h; };
	static const ResEntry s_resolutions[];
	static constexpr int CUSTOM_INDEX = -1;
	static int FindResIndex(int w, int h);
};