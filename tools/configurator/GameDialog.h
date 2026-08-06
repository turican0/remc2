#pragma once
#include "Globals.h"
#include "portability/Config.h"

class GameDialog : public wxDialog {
public:
	GameDialog(wxWindow* parent, const Config::Settings::Game& g);
	Config::Settings::Game GetSettings() const;

private:
	wxSpinCtrl* m_maxGameFps = nullptr;
	wxCheckBox* m_skipIntro = nullptr;
};
