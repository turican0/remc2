#pragma once
#include "Globals.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>

class ThreadingDialog : public wxDialog
{
public:
	ThreadingDialog(wxWindow* parent, const Config::Settings::Threading& cfg);

	Config::Settings::Threading GetSettings() const;

private:
	void OnOK(wxCommandEvent&);
	void OnActiveToggled(wxCommandEvent&);

	wxCheckBox* m_chkActive;
	wxSpinCtrl* m_spinNumThreads;
	wxCheckBox* m_chkAssignCores;

	Config::Settings::Threading m_cfg;
};