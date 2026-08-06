#include "MainFrame.h"

class Remc2ConfiguratorApp : public wxApp
{
public:
	bool OnInit() override;
};

bool Remc2ConfiguratorApp::OnInit()
{
	MainFrame* frame = new MainFrame("Magic Carpet HD Configurator", "Config.json");
	frame->SetMinSize(wxSize(416, 520));
	frame->Show();
	return true;
}

// This defines the equivalent of main() for the current platform.
wxIMPLEMENT_APP(Remc2ConfiguratorApp);

