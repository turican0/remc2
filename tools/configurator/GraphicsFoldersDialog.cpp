#include "GraphicsFoldersDialog.h"
#include "PathHelpers.h"

GraphicsFoldersDialog::GraphicsFoldersDialog(wxWindow* parent, const Config::Settings::GameDetail& d)
	: wxDialog(parent, wxID_ANY, "Select Graphics Files Folders", wxDefaultPosition, wxSize(600, 200), 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX), m_initialDetail(d)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	wxBoxSizer* row1 = MakeFolderRow(panel,
		"High Resolution Graphics Folder:",
		m_chkHighRes, ID_CHK_GRAPHICS,
		m_graphicsFolderCtrl,
		m_btnGraphicsBrowse, ID_BTN_GRAPHICS_BROWSE);

	wxBoxSizer* row2 = MakeFolderRow(panel,
		"Fixed Menu Graphics Folder:",
		m_chkFixedMenu, ID_CHK_FIX_MENU,
		m_menuFolderCtrl,
		m_btnMenuBrowse, ID_BTN_FIX_MENU_BROWSE);

	wxBoxSizer* row3 = MakeFolderRow(panel,
		"Extended Fonts Folder:",
		m_chkFonts, ID_CHK_FONTS,
		m_fontFolderCtrl,
		m_btnFontsBrowse, ID_BTN_FONTS_BROWSE);

	// ── Populate m_rows for unified handlers ─────────────────────────────────
	m_rows[0] = { m_chkHighRes,   m_graphicsFolderCtrl, m_btnGraphicsBrowse, "Select folder with the High Resolution graphics files" };
	m_rows[1] = { m_chkFixedMenu, m_menuFolderCtrl,     m_btnMenuBrowse,     "Select folder for fixed menu files" };
	m_rows[2] = { m_chkFonts,     m_fontFolderCtrl,     m_btnFontsBrowse,    "Select folder with the extended font files" };

	// ── Initial values ────────────────────────────────────────────────────────
	m_chkHighRes->SetValue(d.m_UseHighResGraphics);
	m_graphicsFolderCtrl->SetValue(d.m_HighResGraphicsFolder);
	m_graphicsFolderCtrl->SetToolTip("Path to High Resolution Graphics");

	m_chkFixedMenu->SetValue(d.m_UseFixedMenuGraphics);
	m_menuFolderCtrl->SetValue(d.m_FixedMenuGraphicsFolder);
	m_menuFolderCtrl->SetToolTip("Path to the fixed menu Graphics");

	m_chkFonts->SetValue(d.m_UseExtendedFonts);
	m_fontFolderCtrl->SetValue(d.m_ExtendedFontsFolder);
	m_fontFolderCtrl->SetToolTip("Path to the extended Fonts");

	// ── Sync enabled state with checkbox values ───────────────────────────────
	for (auto& row : m_rows)
	{
		row.ctrl->Enable(row.chk->GetValue());
		row.btn->Enable(row.chk->GetValue());
	}

	// ── OK / Cancel ───────────────────────────────────────────────────────────
	wxStdDialogButtonSizer* btnSizer = new wxStdDialogButtonSizer();
	btnSizer->AddButton(new wxButton(panel, wxID_OK));
	btnSizer->AddButton(new wxButton(panel, wxID_CANCEL));
	btnSizer->Realize();

	// ── Main sizer ────────────────────────────────────────────────────────────
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(row1, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->Add(row2, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->Add(row3, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12));
	mainSizer->AddStretchSpacer(1);
	mainSizer->Add(btnSizer, wxSizerFlags(0).Expand().Border(wxALL, 8));

	panel->SetSizer(mainSizer);

	wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
	dialogSizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(dialogSizer);

	// ── Event bindings ────────────────────────────────────────────────────────
	Bind(wxEVT_BUTTON, &GraphicsFoldersDialog::OnBrowse, this, ID_BTN_GRAPHICS_BROWSE, ID_BTN_FONTS_BROWSE);
	Bind(wxEVT_CHECKBOX, &GraphicsFoldersDialog::OnCheckChanged, this, ID_CHK_GRAPHICS, ID_CHK_FONTS);

	Centre();
}

wxBoxSizer* GraphicsFoldersDialog::MakeFolderRow(wxPanel* panel, const wxString& labelText,
	wxCheckBox*& outChk, int chkId,
	wxTextCtrl*& outCtrl,
	wxButton*& outBtn, int browseId)
{
	outChk = new wxCheckBox(panel, chkId, wxEmptyString);
	auto* label = new wxStaticText(panel, wxID_ANY, labelText);
	outCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize);
	outBtn = new wxButton(panel, browseId, "Browse...");

	wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(outChk, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	row->Add(label, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	row->Add(outCtrl, wxSizerFlags(1).Expand().Border(wxRIGHT, 6));
	row->Add(outBtn, wxSizerFlags(0).CentreVertical());
	return row;
}

void GraphicsFoldersDialog::OnBrowse(wxCommandEvent& evt)
{
	FolderRow& row = m_rows[evt.GetId() - ID_BTN_GRAPHICS_BROWSE];
	wxDirDialog dlg(this, row.prompt, row.ctrl->GetValue(),
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
		row.ctrl->SetValue(ToRelative(dlg.GetPath()));
}

void GraphicsFoldersDialog::OnCheckChanged(wxCommandEvent& evt)
{
	FolderRow& row = m_rows[evt.GetId() - ID_CHK_GRAPHICS];
	const bool ena = evt.IsChecked();
	row.ctrl->Enable(ena);
	row.btn->Enable(ena);
}

Config::Settings::GameDetail GraphicsFoldersDialog::GetFolders() const
{
	Config::Settings::GameDetail d = m_initialDetail;
	d.m_UseHighResGraphics = m_chkHighRes->GetValue();
	d.m_UseFixedMenuGraphics = m_chkFixedMenu->GetValue();
	d.m_UseExtendedFonts = m_chkFonts->GetValue();
	d.m_HighResGraphicsFolder = m_graphicsFolderCtrl->GetValue();
	d.m_FixedMenuGraphicsFolder = m_menuFolderCtrl->GetValue();
	d.m_ExtendedFontsFolder = m_fontFolderCtrl->GetValue();
	return d;
}