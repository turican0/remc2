#include "ZoneEditorDialog.h"

#include <wx/sizer.h>
#include <wx/statline.h>

// ---------------------------------------------------------------------------
// Event table
// ---------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(ZoneEditorDialog, wxDialog)
    EVT_BUTTON(wxID_OK,     ZoneEditorDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, ZoneEditorDialog::OnCancel)
wxEND_EVENT_TABLE()

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
ZoneEditorDialog::ZoneEditorDialog(wxWindow*    parent,
                       const std::vector<Maths::Zone>& zones,
                       wxWindowID               id,
                       const wxString&          title,
                       const wxPoint&           pos,
                       const wxSize&            size)
    : wxDialog(parent, id, title, pos, size, 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
    SetMinSize(wxSize(420, 380));

    // ---- Zone editor control -----------------------------------------------
    m_editor = new ZoneEditorCtrl(this);
    m_editor->SetZones(zones);
    m_editor->Bind(wxEVT_ZONES_CHANGED, &ZoneEditorDialog::OnZonesChanged, this);

    // ---- Status label -------------------------------------------------------
    m_statusLabel = new wxStaticText(this, wxID_ANY, wxEmptyString,
                                     wxDefaultPosition, wxDefaultSize,
                                     wxST_ELLIPSIZE_END);

    // ---- Buttons ------------------------------------------------------------
    m_btnOK            = new wxButton(this, wxID_OK,     wxT("OK"));
    wxButton* btnCancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    m_btnOK->SetDefault();

    // ---- Button row ---------------------------------------------------------
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(m_statusLabel, 1, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 12);
    btnSizer->Add(m_btnOK,       0, wxRIGHT, 6);
    btnSizer->Add(btnCancel,     0);

    // ---- Root sizer ---------------------------------------------------------
    wxBoxSizer* root = new wxBoxSizer(wxVERTICAL);
    root->Add(m_editor,
              1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    root->Add(new wxStaticLine(this),
              0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    root->Add(btnSizer,
              0, wxEXPAND | wxALL, 10);

    SetSizer(root);
    Layout();
    Centre(wxBOTH);

    // Initialise status for the pre-loaded zones
    UpdateStatus();
}

// ---------------------------------------------------------------------------
// GetZones
// ---------------------------------------------------------------------------
std::vector<Maths::Zone> ZoneEditorDialog::GetZones() const
{
    return m_editor->GetZones();
}

// ---------------------------------------------------------------------------
// OnZonesChanged
// ---------------------------------------------------------------------------
void ZoneEditorDialog::OnZonesChanged(wxCommandEvent& evt)
{
    UpdateStatus();
    evt.Skip();
}

// ---------------------------------------------------------------------------
// UpdateStatus  –  validate and update the status label + OK button
// ---------------------------------------------------------------------------
void ZoneEditorDialog::UpdateStatus()
{
    wxString err = m_editor->Validate();
    const bool ok = err.empty();

    if (ok)
    {
        std::size_t n = m_editor->GetZones().size();
        m_statusLabel->SetLabel(
            wxString::Format(wxT("\u2713  %zu zone%s valid"),
                             n, n == 1 ? wxT("") : wxT("s")));
        m_statusLabel->SetForegroundColour(wxColour(0, 130, 0));
    }
    else
    {
        m_statusLabel->SetLabel(wxT("\u26A0  ") + err);
        m_statusLabel->SetForegroundColour(wxColour(190, 0, 0));
    }

    m_btnOK->Enable(ok);
    m_statusLabel->GetParent()->Layout();
}

// ---------------------------------------------------------------------------
// OnOK / OnCancel
// ---------------------------------------------------------------------------
void ZoneEditorDialog::OnOK(wxCommandEvent&)
{
    // Force any in-progress cell edit to commit before we accept
    if (m_editor)
    {
        // wxGrid might have an open editor; save it
        // (ZoneEditorDialogCtrl exposes the grid indirectly via events, so we rely
        //  on the grid having already fired wxEVT_GRID_CELL_CHANGED.
        //  A belt-and-suspenders approach: re-validate once more.)
    }

    if (m_editor->Validate().empty())
        EndModal(wxID_OK);
}

void ZoneEditorDialog::OnCancel(wxCommandEvent&)
{
    EndModal(wxID_CANCEL);
}
