#pragma once
#include "Globals.h"
#include "../remc2/portability/Config.h"

class SoundDialog : public wxDialog {
public:
	SoundDialog(wxWindow* parent, const Config::Settings::Sound& s);
	Config::Settings::Sound GetSettings() const;

private:
#include <wx/choice.h>

	wxChoice* m_musicType = nullptr;
	wxTextCtrl* m_aweRomPath = nullptr;
	wxCheckBox* m_useMC1Music = nullptr;
	wxCheckBox* m_aweGmIntroMusic = nullptr;
	wxSpinCtrl* m_aweBufferMs = nullptr;
	wxTextCtrl* m_oggFolder = nullptr;
	wxCheckBox* m_oggMusicAlternative = nullptr;
	wxCheckBox* m_fixSpeedSound = nullptr;
	wxSpinCtrl* m_maxSimSounds = nullptr;
	wxTextCtrl* m_speechFolder = nullptr;
	wxCheckBox* m_autoShowObjectives = nullptr;
};