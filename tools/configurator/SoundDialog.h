#pragma once
#include "Globals.h"
#include "portability/Config.h"

class SoundDialog : public wxDialog {
public:
	SoundDialog(wxWindow* parent, const Config::Settings::Sound& s);
	Config::Settings::Sound GetSettings() const;

private:
	wxTextCtrl* m_oggFolder = nullptr;
	wxCheckBox* m_oggMusicAlternative = nullptr;
	wxCheckBox* m_fixSpeedSound = nullptr;
	wxSpinCtrl* m_maxSimSounds = nullptr;
	wxTextCtrl* m_speechFolder = nullptr;
	wxCheckBox* m_autoShowObjectives = nullptr;
};
