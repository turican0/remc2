#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <vector>
#include "ZoneEditorDialog.h"
#include "ZoneEditorCtrl.h"

class ZoneEditorDialog : public wxDialog
{
public:
    ZoneEditorDialog(wxWindow*          parent,
               const std::vector<Maths::Zone>& zones,
               wxWindowID               id    = wxID_ANY,
               const wxString&          title = wxT("Zone Editor"),
               const wxPoint&           pos   = wxDefaultPosition,
               const wxSize&            size  = wxSize(560, 540));

    std::vector<Maths::Zone> GetZones() const;

private:
    ZoneEditorCtrl* m_editor      = nullptr;
    wxStaticText*   m_statusLabel = nullptr;
    wxButton*       m_btnOK       = nullptr;

    void OnZonesChanged(wxCommandEvent& evt);
    void OnOK          (wxCommandEvent& evt);
    void OnCancel      (wxCommandEvent& evt);

    void UpdateStatus();

    wxDECLARE_EVENT_TABLE();
};
