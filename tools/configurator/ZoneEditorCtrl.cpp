#include "ZoneEditorCtrl.h"

#include <wx/sizer.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <wx/numdlg.h>
#include <algorithm>
#include <cmath>

// ============================================================================
//  Event table IDs
// ============================================================================
namespace {
    const int ID_BTN_ADD    = wxNewId();
    const int ID_BTN_REMOVE = wxNewId();

    // Grid columns
    const int COL_START  = 0;
    const int COL_END    = 1;
    const int COL_FACTOR = 2;

    // Visual bar height
    const int BAR_HEIGHT = 40;

    // Colour palette for zone segments (cycles)
    const wxColour kPalette[] = {
        wxColour(0x4E, 0x79, 0xA7),
        wxColour(0xF2, 0x8E, 0x2B),
        wxColour(0x59, 0xA1, 0x4F),
        wxColour(0xE1, 0x57, 0x59),
        wxColour(0x76, 0xB7, 0xB2),
        wxColour(0xFF, 0x9D, 0xA7),
        wxColour(0x9C, 0x75, 0x5F),
        wxColour(0xBA, 0xB0, 0xAC),
    };
    const int kPaletteSize = (int)(sizeof(kPalette) / sizeof(kPalette[0]));
}

// ============================================================================
//  wxEVT_ZONES_CHANGED
// ============================================================================
wxDEFINE_EVENT(wxEVT_ZONES_CHANGED, wxCommandEvent);

// ============================================================================
//  ZoneBarPanel
// ============================================================================
wxBEGIN_EVENT_TABLE(ZoneBarPanel, wxPanel)
    EVT_PAINT(ZoneBarPanel::OnPaint)
    EVT_SIZE (ZoneBarPanel::OnSize)
wxEND_EVENT_TABLE()

ZoneBarPanel::ZoneBarPanel(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id, wxDefaultPosition, wxSize(-1, BAR_HEIGHT))
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetMinSize(wxSize(-1, BAR_HEIGHT));
}

void ZoneBarPanel::SetZones(const std::vector<Maths::Zone>& zones)
{
    m_zones = zones;
    m_totalRange = 1;
    if (!zones.empty())
        m_totalRange = std::max(1, zones.back().m_xEnd - zones.front().m_xStart);
    Refresh();
}

void ZoneBarPanel::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    const wxSize sz = GetClientSize();

    // Background
    dc.SetBackground(wxBrush(GetParent()->GetBackgroundColour()));
    dc.Clear();

    if (m_zones.empty()) return;

    const int originValue = m_zones.front().m_xStart;
    const double scale    = (double)sz.x / m_totalRange;
    const int    barY     = 4;
    const int    barH     = sz.y - 8;

    for (int i = 0; i < (int)m_zones.size(); ++i)
    {
        const Maths::Zone& z   = m_zones[i];
        int x0 = (int)std::round((z.m_xStart - originValue) * scale);
        int x1 = (int)std::round((z.m_xEnd   - originValue) * scale);
        int w  = std::max(1, x1 - x0);

        wxColour col = kPalette[i % kPaletteSize];

        // Fill proportional to factor (height)
        int fillH = (int)(barH * z.m_factor);
        int fillY = barY + (barH - fillH);

        // Background stripe (dim)
        dc.SetBrush(wxBrush(col.ChangeLightness(170)));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(x0, barY, w, barH);

        // Factor fill
        dc.SetBrush(wxBrush(col));
        dc.DrawRectangle(x0, fillY, w, fillH);

        // Border between zones
        dc.SetPen(wxPen(*wxWHITE, 1));
        dc.DrawLine(x0, barY, x0, barY + barH);
    }

    // Outer border
    dc.SetPen(wxPen(wxColour(180,180,180)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(0, barY, sz.x, barH);
}

void ZoneBarPanel::OnSize(wxSizeEvent& evt)
{
    Refresh();
    evt.Skip();
}

// ============================================================================
//  ZoneEditorCtrl – event table
// ============================================================================
wxBEGIN_EVENT_TABLE(ZoneEditorCtrl, wxPanel)
    EVT_BUTTON(ID_BTN_ADD,    ZoneEditorCtrl::OnAddZone)
    EVT_BUTTON(ID_BTN_REMOVE, ZoneEditorCtrl::OnRemoveZone)
wxEND_EVENT_TABLE()

// ============================================================================
//  ZoneEditorCtrl – construction
// ============================================================================
ZoneEditorCtrl::ZoneEditorCtrl(wxWindow*       parent,
                               wxWindowID      id,
                               const wxPoint&  pos,
                               const wxSize&   size,
                               long            style,
                               const wxString& name)
    : wxPanel(parent, id, pos, size, style, name)
{
    // ---- Visual bar --------------------------------------------------------
    m_bar = new ZoneBarPanel(this);

    // ---- Grid --------------------------------------------------------------
    BuildGrid();

    // ---- Buttons -----------------------------------------------------------
    m_btnAdd = new wxButton(this, ID_BTN_ADD,    wxT("+ Add Zone"));
    m_btnDel = new wxButton(this, ID_BTN_REMOVE, wxT("- Remove Selected"));

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(m_btnAdd, 0, wxRIGHT, 6);
    btnSizer->Add(m_btnDel, 0);

    // ---- Layout ------------------------------------------------------------
    wxBoxSizer* root = new wxBoxSizer(wxVERTICAL);
    root->Add(m_bar,    0, wxEXPAND | wxBOTTOM, 6);
    root->Add(m_grid,   1, wxEXPAND | wxBOTTOM, 6);
    root->Add(btnSizer, 0, wxALIGN_LEFT);
    SetSizer(root);
}

// ============================================================================
//  BuildGrid – create the wxGrid with column setup
// ============================================================================
void ZoneEditorCtrl::BuildGrid()
{
    m_grid = new wxGrid(this, wxID_ANY);
    m_grid->CreateGrid(0, 3);

    m_grid->SetColLabelValue(COL_START,  wxT("Start"));
    m_grid->SetColLabelValue(COL_END,    wxT("End"));
    m_grid->SetColLabelValue(COL_FACTOR, wxT("Factor"));

    m_grid->SetRowLabelSize(30);
    m_grid->SetColSize(COL_START,  120);
    m_grid->SetColSize(COL_END,    120);
    m_grid->SetColSize(COL_FACTOR, 100);

    m_grid->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_grid->EnableDragRowSize(false);

    // Connect grid cell-change events
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED,
                 &ZoneEditorCtrl::OnGridChange, this);
    m_grid->Bind(wxEVT_GRID_SELECT_CELL,
                 &ZoneEditorCtrl::OnGridSelect, this);
}

// ============================================================================
//  SetZones / GetZones
// ============================================================================
void ZoneEditorCtrl::SetZones(const std::vector<Maths::Zone>& zones)
{
    m_zones = zones;
    RefreshGrid();
    RefreshBar();
}

std::vector<Maths::Zone> ZoneEditorCtrl::GetZones() const
{
    return m_zones;
}

// ============================================================================
//  Validate
// ============================================================================
wxString ZoneEditorCtrl::Validate() const
{
    for (int i = 0; i < (int)m_zones.size(); ++i)
    {
        const Maths::Zone& z = m_zones[i];

        if (z.m_xStart >= z.m_xEnd)
            return wxString::Format(
                wxT("Zone %d: start (%d) must be less than end (%d)."),
                i + 1, z.m_xStart, z.m_xEnd);

        if (z.m_factor < 0.0 || z.m_factor > 1.0)
            return wxString::Format(
                wxT("Zone %d: factor (%.4f) must be in [0, 1]."),
                i + 1, z.m_factor);

        if (i > 0 && m_zones[i].m_xStart != m_zones[i - 1].m_xEnd)
            return wxString::Format(
                wxT("Gap between zone %d (end=%d) and zone %d (start=%d)."),
                i, m_zones[i - 1].m_xEnd, i + 1, m_zones[i].m_xStart);
    }
    return wxEmptyString;
}

// ============================================================================
//  RefreshGrid – sync m_zones → grid rows
// ============================================================================
void ZoneEditorCtrl::RefreshGrid()
{
    m_suppressEvents = true;

    const int needed  = (int)m_zones.size();
    const int current = m_grid->GetNumberRows();

    if (current < needed)
        m_grid->AppendRows(needed - current);
    else if (current > needed)
        m_grid->DeleteRows(0, current - needed);

    for (int r = 0; r < needed; ++r)
        AppendRowToGrid(r, m_zones[r]);

    // Colour alternating rows
    for (int r = 0; r < needed; ++r)
    {
        wxColour bg = (r % 2 == 0)
            ? wxColour(245, 248, 255)
            : wxColour(255, 255, 255);
        for (int c = 0; c < 3; ++c)
            m_grid->SetCellBackgroundColour(r, c, bg);
    }

    m_grid->ForceRefresh();
    m_suppressEvents = false;
}

// ============================================================================
//  AppendRowToGrid – write one zone into a grid row
// ============================================================================
void ZoneEditorCtrl::AppendRowToGrid(int row, const Maths::Zone& z)
{
    m_grid->SetCellValue(row, COL_START,
        wxString::Format(wxT("%d"), z.m_xStart));
    m_grid->SetCellValue(row, COL_END,
        wxString::Format(wxT("%d"), z.m_xEnd));
    m_grid->SetCellValue(row, COL_FACTOR,
        wxString::Format(wxT("%.4f"), z.m_factor));
}

// ============================================================================
//  RowToZone – read one grid row back into a Zone
// ============================================================================
Maths::Zone ZoneEditorCtrl::RowToZone(int row) const
{
	Maths::Zone z;
    long tmp = 0;

    if (m_grid->GetCellValue(row, COL_START).ToLong(&tmp))
        z.m_xStart = (int)tmp;
    if (m_grid->GetCellValue(row, COL_END).ToLong(&tmp))
        z.m_xEnd = (int)tmp;

    double d = 0.0;
    if (m_grid->GetCellValue(row, COL_FACTOR).ToDouble(&d))
        z.m_factor = d;

    return z;
}

// ============================================================================
//  RefreshBar
// ============================================================================
void ZoneEditorCtrl::RefreshBar()
{
    m_bar->SetZones(m_zones);
}

// ============================================================================
//  FireChangedEvent
// ============================================================================
void ZoneEditorCtrl::FireChangedEvent()
{
    wxCommandEvent evt(wxEVT_ZONES_CHANGED, GetId());
    evt.SetEventObject(this);
    GetEventHandler()->ProcessEvent(evt);
}

// ============================================================================
//  OnGridChange – user edited a cell
// ============================================================================
void ZoneEditorCtrl::OnGridChange(wxGridEvent& evt)
{
    if (m_suppressEvents) { evt.Skip(); return; }

    int row = evt.GetRow();
    if (row < 0 || row >= (int)m_zones.size()) { evt.Skip(); return; }

    m_zones[row] = RowToZone(row);

    RefreshBar();
    FireChangedEvent();
    evt.Skip();
}

// ============================================================================
//  OnGridSelect – keep Remove button state current
// ============================================================================
void ZoneEditorCtrl::OnGridSelect(wxGridEvent& evt)
{
    m_btnDel->Enable(m_grid->GetNumberRows() > 1);
    evt.Skip();
}

// ============================================================================
//  OnAddZone – duplicate the last zone and append
// ============================================================================
void ZoneEditorCtrl::OnAddZone(wxCommandEvent&)
{
	Maths::Zone newZone;

    if (!m_zones.empty())
    {
        const Maths::Zone& last = m_zones.back();
        int span = last.m_xEnd - last.m_xStart;
        newZone.m_xStart  = last.m_xEnd;
        newZone.m_xEnd    = last.m_xEnd + span;
        newZone.m_factor = std::min(1.0, last.m_factor + 0.05);
    }
    else
    {
        newZone = {0, 1000, 0.5};
    }

    m_zones.push_back(newZone);
    RefreshGrid();
    RefreshBar();
    FireChangedEvent();

    // Scroll to the new row
    int lastRow = m_grid->GetNumberRows() - 1;
    m_grid->GoToCell(lastRow, 0);
    m_grid->SetGridCursor(lastRow, 0);
    m_btnDel->Enable(true);
}

// ============================================================================
//  OnRemoveZone – remove currently selected row(s)
// ============================================================================
void ZoneEditorCtrl::OnRemoveZone(wxCommandEvent&)
{
    if (m_zones.size() <= 1) return;

    // Collect unique selected rows
    wxArrayInt selRows = m_grid->GetSelectedRows();

    if (selRows.empty())
    {
        // Fall back to cursor row
        int cur = m_grid->GetGridCursorRow();
        if (cur >= 0) selRows.push_back(cur);
    }

    if (selRows.empty()) return;

    // Sort descending so indices stay valid as we erase
    selRows.Sort([](int* a, int* b){ return (*b) - (*a); });

    for (int r : selRows)
    {
        if (r >= 0 && r < (int)m_zones.size())
            m_zones.erase(m_zones.begin() + r);
    }

    RefreshGrid();
    RefreshBar();
    FireChangedEvent();

    m_btnDel->Enable(m_zones.size() > 1);
}
