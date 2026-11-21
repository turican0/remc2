#pragma once
#include "Globals.h"
#include "../remc2/portability/Config.h"

class GameDialog : public wxDialog {
public:
	GameDialog(wxWindow* parent, const Config::Settings::Game& g);
	Config::Settings::Game GetSettings() const;

private:
	wxSpinCtrl* m_maxGameFps = nullptr;
	wxCheckBox* m_skipIntro = nullptr;
	wxSpinCtrl* m_mc1EntityLimit = nullptr;
};