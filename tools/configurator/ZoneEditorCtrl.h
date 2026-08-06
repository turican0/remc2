#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/panel.h>
#include <vector>
#include "utilities/Maths.h"

// ---------------------------------------------------------------------------
// ZoneBarPanel – read-only visual bar drawn above the grid
// ---------------------------------------------------------------------------
class ZoneBarPanel : public wxPanel
{
public:
    ZoneBarPanel(wxWindow* parent, wxWindowID id = wxID_ANY);

    void SetZones(const std::vector<Maths::Zone>& zones);

private:
    std::vector<Maths::Zone> m_zones;
    int               m_totalRange = 1;

    void OnPaint(wxPaintEvent& evt);
    void OnSize (wxSizeEvent&  evt);

    wxDECLARE_EVENT_TABLE();
};

// ---------------------------------------------------------------------------
// wxEVT_ZONES_CHANGED – fired whenever the user edits the grid
// ---------------------------------------------------------------------------
wxDECLARE_EVENT(wxEVT_ZONES_CHANGED, wxCommandEvent);

// ---------------------------------------------------------------------------
// ZoneEditorCtrl
// ---------------------------------------------------------------------------
class ZoneEditorCtrl : public wxPanel
{
public:
    ZoneEditorCtrl(wxWindow*        parent,
                   wxWindowID       id     = wxID_ANY,
                   const wxPoint&   pos    = wxDefaultPosition,
                   const wxSize&    size   = wxDefaultSize,
                   long             style  = wxTAB_TRAVERSAL,
                   const wxString&  name   = wxPanelNameStr);

    // -----------------------------------------------------------------------
    // Public API
    // -----------------------------------------------------------------------
    void SetZones(const std::vector<Maths::Zone>& zones);
    std::vector<Maths::Zone> GetZones() const;

    // Validate that zones are contiguous (end[i] == start[i+1]) and
    // that factors are in [0,1].  Returns an empty string on success,
    // or a human-readable error message.
    wxString Validate() const;

private:
    // ---- helpers -----------------------------------------------------------
    void BuildGrid();
    void RefreshGrid();
    void RefreshBar();
    void AppendRowToGrid(int row, const Maths::Zone& z);
	Maths::Zone RowToZone(int row) const;
    void FireChangedEvent();

    // ---- event handlers ----------------------------------------------------
    void OnAddZone   (wxCommandEvent& evt);
    void OnRemoveZone(wxCommandEvent& evt);
    void OnGridChange(wxGridEvent&    evt);
    void OnGridSelect(wxGridEvent&    evt);

    // ---- members -----------------------------------------------------------
    std::vector<Maths::Zone> m_zones;

    ZoneBarPanel* m_bar    = nullptr;
    wxGrid*       m_grid   = nullptr;
    wxButton*     m_btnAdd = nullptr;
    wxButton*     m_btnDel = nullptr;

    bool m_suppressEvents = false;   // guard against re-entrant updates

    wxDECLARE_EVENT_TABLE();
};
