#include "SoundDialog.h"
#include "PathHelpers.h"
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/choice.h>
#include <wx/statbox.h>

SoundDialog::SoundDialog(wxWindow* parent, const Config::Settings::Sound& s)
	: wxDialog(parent, wxID_ANY, "Sound Settings",
		wxDefaultPosition, wxSize(450, 420), 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// ── Music section ────────────────────────────────────────────────────────
	auto* musicBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Music");
	wxStaticBox* musicParent = musicBox->GetStaticBox();

	// Music Type row
	auto* musicTypeLabel = new wxStaticText(musicParent, wxID_ANY, "Music Type:");
	m_musicType = new wxChoice(musicParent, wxID_ANY);
	m_musicType->Append("AWE Enum");
	m_musicType->Append("OGG");
	m_musicType->SetSelection(s.m_MusicType == "ogg" ? 1 : 0);
	m_musicType->SetToolTip("Select how music is played. Awe32 (the original XMI music of the game through the Sound Blaster AWE32 emulation) or OGG recorded music (re-imagined from the midi tracks (uses less CPU))");

	wxBoxSizer* typeRow = new wxBoxSizer(wxHORIZONTAL);
	typeRow->Add(musicTypeLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	typeRow->Add(m_musicType, wxSizerFlags(0));

	// AWE controls
	auto* aweRomLabel = new wxStaticText(musicParent, wxID_ANY, "AWE 32 Rom Folder:");
	m_aweRomPath = new wxTextCtrl(musicParent, wxID_ANY, s.m_Awe32RomFolder);
	m_aweRomPath->SetToolTip("Relative path to the AWE 32 ROM file (.binz or .bin).");
	auto* btnAweBrowse = new wxButton(musicParent, wxID_ANY, "Browse...");

	wxBoxSizer* aweRow = new wxBoxSizer(wxHORIZONTAL);
	aweRow->Add(aweRomLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	aweRow->Add(m_aweRomPath, wxSizerFlags(1).Expand().Border(wxRIGHT, 4));
	aweRow->Add(btnAweBrowse, wxSizerFlags(0).CentreVertical());

	m_useMC1Music = new wxCheckBox(musicParent, wxID_ANY, "Use Magic Carpet 1 Music in Levels");
	m_useMC1Music->SetValue(s.m_UseMagicCarpet1Music);
	m_useMC1Music->SetToolTip("Use the Magic Carpet 1 music tracks in Levels.");

	m_aweGmIntroMusic = new wxCheckBox(musicParent, wxID_ANY, "General MIDI Music in Intro and Cutscenes");
	m_aweGmIntroMusic->SetValue(s.m_Awe32GmIntroMusic);
	m_aweGmIntroMusic->SetToolTip("The intro and the cutscenes play the General MIDI version of the music, the game keeps the AWE32 version.");

	auto* aweBufferLabel = new wxStaticText(musicParent, wxID_ANY, "AWE 32 Buffer (ms):");
	m_aweBufferMs = new wxSpinCtrl(musicParent, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 100, 2000, s.m_Awe32BufferMs);
	m_aweBufferMs->SetToolTip("How far ahead the AWE32 emulation renders. Raise it when the music crackles.");

	wxBoxSizer* aweBufferRow = new wxBoxSizer(wxHORIZONTAL);
	aweBufferRow->Add(aweBufferLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	aweBufferRow->Add(m_aweBufferMs, wxSizerFlags(0));

	wxBoxSizer* aweGroup = new wxBoxSizer(wxVERTICAL);
	aweGroup->Add(aweRow, wxSizerFlags(0).Expand());
	aweGroup->Add(m_useMC1Music, wxSizerFlags(0).Border(wxTOP, 8));
	aweGroup->Add(m_aweGmIntroMusic, wxSizerFlags(0).Border(wxTOP, 8));
	aweGroup->Add(aweBufferRow, wxSizerFlags(0).Border(wxTOP, 8));

	// OGG controls
	auto* oggFolderLabel = new wxStaticText(musicParent, wxID_ANY, "Music Tracks Folder:");
	m_oggFolder = new wxTextCtrl(musicParent, wxID_ANY, s.m_OggFolder);
	m_oggFolder->SetToolTip("Relative Path to the music tracks. Required for music to play.");
	auto* btnOggBrowse = new wxButton(musicParent, wxID_ANY, "Browse...");

	wxBoxSizer* oggRow = new wxBoxSizer(wxHORIZONTAL);
	oggRow->Add(oggFolderLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	oggRow->Add(m_oggFolder, wxSizerFlags(1).Expand().Border(wxRIGHT, 4));
	oggRow->Add(btnOggBrowse, wxSizerFlags(0).CentreVertical());

	m_oggMusicAlternative = new wxCheckBox(musicParent, wxID_ANY, "Use Alternative Music Tracks");
	m_oggMusicAlternative->SetValue(s.m_OggMusicAlternative);
	m_oggMusicAlternative->SetToolTip("Use alternative music tracks.");

	wxBoxSizer* oggGroup = new wxBoxSizer(wxVERTICAL);
	oggGroup->Add(oggRow, wxSizerFlags(0).Expand());
	oggGroup->Add(m_oggMusicAlternative, wxSizerFlags(0).Border(wxTOP, 8));

	// Line up the labels so the text fields start at the same x position
	int labelWidth = 0;
	for (wxStaticText* l : { musicTypeLabel, aweRomLabel, oggFolderLabel })
		labelWidth = std::max(labelWidth, l->GetBestSize().GetWidth());
	for (wxStaticText* l : { musicTypeLabel, aweRomLabel, oggFolderLabel })
		l->SetMinSize(wxSize(labelWidth, -1));

	const wxSizerFlags innerFlags = wxSizerFlags(0).Expand().Border(wxALL, 6);
	musicBox->Add(typeRow, innerFlags);
	musicBox->Add(aweGroup, innerFlags);
	musicBox->Add(oggGroup, innerFlags);

	// ── Sound FX section ─────────────────────────────────────────────────────
	auto* fxBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Sound FX");
	wxStaticBox* fxParent = fxBox->GetStaticBox();

	m_fixSpeedSound = new wxCheckBox(fxParent, wxID_ANY, "Fix Speed Sound");
	m_fixSpeedSound->SetValue(s.m_FixSpeedSound);
	m_fixSpeedSound->SetToolTip("Enable if sounds play at double speed.");

	auto* maxLabel = new wxStaticText(fxParent, wxID_ANY, "Max Simultaneous Sounds:");
	m_maxSimSounds = new wxSpinCtrl(fxParent, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 10, 128, s.m_MaxSimultaniousSounds);
	m_maxSimSounds->SetToolTip("Original game was 10. Increasing beyond 20 might impact performance.");

	wxBoxSizer* maxRow = new wxBoxSizer(wxHORIZONTAL);
	maxRow->Add(maxLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	maxRow->Add(m_maxSimSounds, wxSizerFlags(0));

	auto* speechLabel = new wxStaticText(fxParent, wxID_ANY, "Speech Track Folder:");
	m_speechFolder = new wxTextCtrl(fxParent, wxID_ANY, s.m_SpeechFolder);
	m_speechFolder->SetToolTip("Relative Path to the speech tracks. Required for in game briefings to play.");
	auto* btnSpeechBrowse = new wxButton(fxParent, wxID_ANY, "Browse...");

	wxBoxSizer* speechRow = new wxBoxSizer(wxHORIZONTAL);
	speechRow->Add(speechLabel, wxSizerFlags(0).CentreVertical().Border(wxRIGHT, 6));
	speechRow->Add(m_speechFolder, wxSizerFlags(1).Expand().Border(wxRIGHT, 4));
	speechRow->Add(btnSpeechBrowse, wxSizerFlags(0).CentreVertical());

	m_autoShowObjectives = new wxCheckBox(fxParent, wxID_ANY,
		"Auto Show Objectives for Foreign Languages");
	m_autoShowObjectives->SetValue(s.m_AutoShowObjectivesForForeignLanguages);
	m_autoShowObjectives->SetToolTip("Auto display objective and mission text when a different language to english is used.");

	fxBox->Add(m_fixSpeedSound, innerFlags);
	fxBox->Add(maxRow, innerFlags);
	fxBox->Add(speechRow, innerFlags);
	fxBox->Add(m_autoShowObjectives, innerFlags);

	// ── Show / hide music controls based on Music Type ──────────────────────
	auto updateMusicType = [this, panel, musicBox, aweGroup, oggGroup](bool relayout) {
		const bool isOgg = (m_musicType->GetSelection() == 1);
		musicBox->Show(aweGroup, !isOgg, true);
		musicBox->Show(oggGroup, isOgg, true);
		if (relayout)
		{
			panel->InvalidateBestSize();
			panel->Layout();
			GetSizer()->Fit(this);
			GetSizer()->SetSizeHints(this);
		}
		};
	updateMusicType(false);
	m_musicType->Bind(wxEVT_CHOICE, [updateMusicType](wxCommandEvent&) {
		updateMusicType(true);
		});

	// ── Browse button handlers ───────────────────────────────────────────────
	btnAweBrowse->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		wxDirDialog dlg(this, "Select AWE 32 ROM Folder",
			ToAbsolute(m_aweRomPath->GetValue()),
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if (dlg.ShowModal() == wxID_OK)
			m_aweRomPath->SetValue(ToRelative(dlg.GetPath()));
		});

	btnOggBrowse->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		wxDirDialog dlg(this, "Select Music Track Folder",
			ToAbsolute(m_oggFolder->GetValue()),
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if (dlg.ShowModal() == wxID_OK)
			m_oggFolder->SetValue(ToRelative(dlg.GetPath()));
		});

	btnSpeechBrowse->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
		wxDirDialog dlg(this, "Select Speech Track Folder",
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
	const wxSizerFlags sectionFlags = wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 12);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(musicBox, sectionFlags);
	mainSizer->Add(fxBox, sectionFlags);
	mainSizer->AddStretchSpacer(1);
	mainSizer->Add(btnSizer, wxSizerFlags(0).Expand().Border(wxALL, 8));

	panel->SetSizer(mainSizer);

	wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
	dialogSizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizerAndFit(dialogSizer);
	SetMinSize(GetSize());

	Centre();
}

Config::Settings::Sound SoundDialog::GetSettings() const
{
	Config::Settings::Sound s;
	s.m_MusicType = (m_musicType->GetSelection() == 1)
		? "ogg"
		: "awe32";

	// Music
	s.m_Awe32RomFolder = m_aweRomPath->GetValue();
	s.m_UseMagicCarpet1Music = m_useMC1Music->GetValue();
	s.m_Awe32GmIntroMusic = m_aweGmIntroMusic->GetValue();
	s.m_Awe32BufferMs = m_aweBufferMs->GetValue();
	s.m_OggFolder = m_oggFolder->GetValue();
	s.m_OggMusicAlternative = m_oggMusicAlternative->GetValue();

	// Sound FX
	s.m_FixSpeedSound = m_fixSpeedSound->GetValue();
	s.m_MaxSimultaniousSounds = m_maxSimSounds->GetValue();
	s.m_SpeechFolder = m_speechFolder->GetValue();
	s.m_AutoShowObjectivesForForeignLanguages = m_autoShowObjectives->GetValue();
	return s;
}