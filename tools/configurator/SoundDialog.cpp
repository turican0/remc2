#include "SoundDialog.h"
#include "PathHelpers.h"
#include <wx/dirdlg.h>

SoundDialog::SoundDialog(wxWindow* parent, const Config::Settings::Sound& s)
	: wxDialog(parent, wxID_ANY, "Sound Settings",
		wxDefaultPosition, wxSize(404, 332), 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// ── Controls ────────────────────────────────────────────────────────────
	m_oggMusicAlternative = new wxCheckBox(panel, wxID_ANY, "Use Alternative Music Tracks");
	m_fixSpeedSound = new wxCheckBox(panel, wxID_ANY, "Fix Speed Sound");
	m_autoShowObjectives = new wxCheckBox(panel, wxID_ANY,
		"Auto Show Objectives for Foreign Languages");

	m_oggMusicAlternative->SetValue(s.m_OggMusicAlternative);
	m_oggMusicAlternative->SetToolTip("Use alternative music tracks.");
	m_fixSpeedSound->SetValue(s.m_FixSpeedSound);
	m_fixSpeedSound->SetToolTip("Set to true when sounds play double speed.");
	m_autoShowObjectives->SetValue(s.m_AutoShowObjectivesForForeignLanguages);
	m_autoShowObjectives->SetToolTip("Auto display objective and mission text when a different language to english is used.");

	// ── OGG Folder row ───────────────────────────────────────────────────────
	auto* oggFolderLabel = new wxStaticText(panel, wxID_ANY, "Music Tracks Folder:");
	m_oggFolder = new wxTextCtrl(panel, wxID_ANY, s.m_OggFolder);
	m_oggFolder->SetToolTip("Relative Path to the music tracks. Required for music to play.");
	auto* btnOggBrowse = new wxButton(panel, wxID_ANY, "Browse...");

	wxBoxSizer* oggRow = new wxBoxSizer(wxHORIZONTAL);
	oggRow->Add(oggFolderLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	oggRow->Add(m_oggFolder, wxSizerFlags(1).Expand().Border(wxRIGHT, 4));
	oggRow->Add(btnOggBrowse, wxSizerFlags(0).CentreVertical());

	// ── Max Simultaneous Sounds row ──────────────────────────────────────────
	auto* maxLabel = new wxStaticText(panel, wxID_ANY, "Max Simultaneous Sounds:");
	m_maxSimSounds = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 10, 128, s.m_MaxSimultaniousSounds);
	m_maxSimSounds->SetToolTip("Original game was 10. Increasing beyond 20 might impact performance.");

	wxBoxSizer* maxRow = new wxBoxSizer(wxHORIZONTAL);
	maxRow->Add(maxLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	maxRow->Add(m_maxSimSounds, wxSizerFlags(0));

	// ── Speech Folder row ────────────────────────────────────────────────────
	auto* speechLabel = new wxStaticText(panel, wxID_ANY, "Speech Track Folder:");
	m_speechFolder = new wxTextCtrl(panel, wxID_ANY, s.m_SpeechFolder);
	m_speechFolder->SetToolTip("Relative Path to the speech tracks. Required for in game briefings to play.");
	auto* btnSpeechBrowse = new wxButton(panel, wxID_ANY, "Browse...");

	wxBoxSizer* speechRow = new wxBoxSizer(wxHORIZONTAL);
	speechRow->Add(speechLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	speechRow->Add(m_speechFolder, wxSizerFlags(1).Expand().Border(wxRIGHT, 4));
	speechRow->Add(btnSpeechBrowse, wxSizerFlags(0).CentreVertical());

	// ── Browse button handlers ───────────────────────────────────────────────
	btnOggBrowse->Bind(wxEVT_BUTTON, [this, panel](wxCommandEvent&) {
		wxDirDialog dlg(panel, "Select Music Track Folder",
			ToAbsolute(m_oggFolder->GetValue()),
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if (dlg.ShowModal() == wxID_OK)
			m_oggFolder->SetValue(ToRelative(dlg.GetPath()));
		});

	btnSpeechBrowse->Bind(wxEVT_BUTTON, [this, panel](wxCommandEvent&) {
		wxDirDialog dlg(panel, "Select Speech Track Folder",
			ToAbsolute(m_speechFolder->GetValue()),
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if (dlg.ShowModal() == wxID_OK)
			m_speechFolder->SetValue(ToRelative(dlg.GetPath()));
		});

	// ── OK / Cancel ──────────────────────────────────────────────────────────
	wxStdDialogButtonSizer* btnSizer = new wxStdDialogButtonSizer();
	btnSizer->AddButton(new wxButton(panel, wxID_OK));
	btnSizer->AddButton(new wxButton(panel, wxID_CANCEL));
	btnSizer->Realize();

	// ── Main sizer ───────────────────────────────────────────────────────────
	const wxSizerFlags rowFlags = wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(oggRow, rowFlags);
	mainSizer->Add(m_oggMusicAlternative, rowFlags);
	mainSizer->Add(m_fixSpeedSound, rowFlags);
	mainSizer->Add(maxRow, rowFlags);
	mainSizer->Add(speechRow, rowFlags);
	mainSizer->Add(m_autoShowObjectives, rowFlags);
	mainSizer->AddStretchSpacer(1);
	mainSizer->Add(btnSizer, wxSizerFlags(0).Expand().Border(wxALL, 8));

	panel->SetSizer(mainSizer);

	wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
	dialogSizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(dialogSizer);

	Centre();
}

Config::Settings::Sound SoundDialog::GetSettings() const
{
	Config::Settings::Sound s;
	s.m_OggFolder = m_oggFolder->GetValue();
	s.m_OggMusicAlternative = m_oggMusicAlternative->GetValue();
	s.m_FixSpeedSound = m_fixSpeedSound->GetValue();
	s.m_MaxSimultaniousSounds = m_maxSimSounds->GetValue();
	s.m_SpeechFolder = m_speechFolder->GetValue();
	s.m_AutoShowObjectivesForForeignLanguages = m_autoShowObjectives->GetValue();
	return s;
}