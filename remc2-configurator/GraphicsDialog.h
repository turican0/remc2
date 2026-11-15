#pragma once
#include "Globals.h"
#include <wx/statline.h>
#include <wx/display.h>
#include "GraphicsFoldersDialog.h"
#include "ThreadingDialog.h"
#include "HardwareDialog.h"
#include "LookingGlassDialog.h"
#include "AnaglyphDialog.h"

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
	wxSpinCtrlDouble* m_spinFogScale = nullptr;
	wxChoice* m_choiceRender = nullptr;
	wxButton* m_btnHardwareSettings = nullptr;
	wxButton* m_btnLookingGlass = nullptr;
	wxButton* m_btnAnaglyph = nullptr;
	wxButton* m_btnRenderThreads = nullptr;

	Config::Settings::Graphics m_cfg;

	// Renderer drop down entries
	enum { RENDER_HARDWARE = 0, RENDER_SOFTWARE = 1 };

	// Helpers
	void OnOK(wxCommandEvent& evt);
	void UpdateRenderButtons();	// shows the settings button for the chosen renderer

	// Common resolutions list  { label, width, height }
	struct ResEntry { const char* label; int w; int h; };
	static const ResEntry s_resolutions[];
	static constexpr int CUSTOM_INDEX = -1;
	static int FindResIndex(int w, int h);
};