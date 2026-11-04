#pragma once
#include "Globals.h"
#include <wx/wx.h>
#include <wx/statline.h>

class HardwareDialog : public wxDialog
{
public:
	HardwareDialog(wxWindow* parent, const Config::Settings::Hardware& cfg);

	Config::Settings::Hardware GetSettings() const;

private:
	void OnOK(wxCommandEvent&);

	wxCheckBox* m_chkBilinear;
	wxChoice* m_choiceAntiAliasing;

	Config::Settings::Hardware m_cfg;

	// Anti-aliasing drop down entries, as MSAA sample counts (0 = off)
	static const int s_antiAliasingSamples[];
	static const int s_antiAliasingCount;
};
