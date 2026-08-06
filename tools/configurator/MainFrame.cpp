#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const std::string fileName) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(416, 520), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	wxInitAllImageHandlers();

	m_ptrConfig = new Config(ToAbsolute(fileName).ToStdString());


	// ── Panel (gives a native background) ───────────────────────────────────
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	wxString path = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPathWithSep() + "mc2_hd_logo.png";

	wxBitmap logo(path, wxBITMAP_TYPE_PNG);

	if (!logo.IsOk())
	{
		wxMessageBox("Failed to load mc2_hd_logo.png");
	}

	wxStaticBitmap* image = new wxStaticBitmap(panel, wxID_ANY, logo);

	// ── Buttons ─────────────────────────────────────────────────────────────
	wxButton* btnPlay = new wxButton(panel, ID_BTN_PLAY, "Play");
	btnPlay->SetMinSize(wxSize(-1, 40));
	wxButton* btnLaunchOptions = new wxButton(panel, ID_BTN_LAUNCH_OPTIONS, "Launch Options");
	btnLaunchOptions->SetMinSize(wxSize(-1, 40));
	wxButton* btnMultiplayer = new wxButton(panel, ID_BTN_MULTIPLAYER, "Multiplayer");
	btnMultiplayer->SetMinSize(wxSize(-1, 40));
	wxButton* btnFile = new wxButton(panel, ID_BTN_FILE, "Game Files");
	btnFile->SetMinSize(wxSize(-1, 40));
	wxButton* btnSettings = new wxButton(panel, ID_BTN_SETTINGS, "Settings");
	btnSettings->SetMinSize(wxSize(-1, 40));
	wxButton* btnExit = new wxButton(panel, ID_BTN_EXIT, "Exit");
	btnExit->SetMinSize(wxSize(-1, 40));

	// ── Vertical sizer ──────────────────────────────────────────────────────

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
	const wxSizerFlags flags = wxSizerFlags(0)
		.Expand()
		.Border(wxTOP | wxBOTTOM, 4);

	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);

	buttonSizer->Add(image, flags);
	buttonSizer->Add(btnPlay, flags);
	buttonSizer->Add(btnLaunchOptions, flags);
	buttonSizer->Add(btnMultiplayer, flags);
	buttonSizer->Add(btnFile, flags);
	buttonSizer->Add(btnSettings, flags);
	buttonSizer->Add(btnExit, flags);

	vSizer->Add(
		buttonSizer,
		wxSizerFlags()
		.Expand()
		.Border(wxLEFT | wxRIGHT, 8)
	);

	// ── Wrap in an outer sizer to centre the column ─────────────────────────
	wxBoxSizer* outerSizer = new wxBoxSizer(wxVERTICAL);
	outerSizer->AddStretchSpacer(1);
	outerSizer->Add(vSizer, wxSizerFlags(0).Centre());
	outerSizer->AddStretchSpacer(1);

	panel->SetSizer(outerSizer);

	// ── Event bindings ───────────────────────────────────────────────────────
	Bind(wxEVT_BUTTON, &MainFrame::OnPlay, this, ID_BTN_PLAY);
	Bind(wxEVT_BUTTON, &MainFrame::OnLaunchOptions, this, ID_BTN_LAUNCH_OPTIONS);
	Bind(wxEVT_BUTTON, &MainFrame::OnMultiplayer, this, ID_BTN_MULTIPLAYER);
	Bind(wxEVT_BUTTON, &MainFrame::OnFile, this, ID_BTN_FILE);
	Bind(wxEVT_BUTTON, &MainFrame::OnSettings, this, ID_BTN_SETTINGS);
	Bind(wxEVT_BUTTON, &MainFrame::OnExit, this, ID_BTN_EXIT);

	Centre(); // centre on screen
}

// ── Button handlers ──────────────────────────────────────────────────────────
void MainFrame::OnPlay(wxCommandEvent&)
{
	if (LaunchGame(""))
		Close(true); // close the launcher
}

void MainFrame::OnLaunchOptions(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	LaunchOptionsDialog dlg(this, settings.m_LaunchArguments);
	auto dialogResult = dlg.ShowModal();
	if (dialogResult == wxID_OK || dialogResult == wxID_SAVE)
	{
		settings.m_LaunchArguments = dlg.GetLaunchArguments();
		m_ptrConfig->SaveLaunchArgumentsToDoc(settings);
		m_ptrConfig->SaveToFile();

		if (dialogResult == wxID_OK)
		{
			LaunchGame(settings.m_LaunchArguments);
			Close(true);
		}
	}
}

void MainFrame::OnMultiplayer(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	MultiplayerDialog dlg(this, settings.m_Multiplayer);
	auto dialogResult = dlg.ShowModal();
	if (dialogResult == wxID_NETWORK || dialogResult == wxID_ADD || dialogResult == wxID_OK)
	{
		auto multiplayerSettings = dlg.GetMultiplayer();
		m_ptrConfig->SaveMultiplayerToDoc(multiplayerSettings);
		m_ptrConfig->SaveToFile();
		m_ptrConfig->GetSettingsFromDoc();

		std::string additional_args = "";

		if (multiplayerSettings.m_RecordPlayFile.length() > 0)
			additional_args += "--record_file \"" + multiplayerSettings.m_RecordPlayFile + "\" ";

		if (multiplayerSettings.m_Debug)
			additional_args += "--network_debug ";

		if (dialogResult == wxID_NETWORK && LaunchGame(additional_args + "--network server " + std::to_string(multiplayerSettings.m_ServerPort) +
			" client 127.0.0.1 " + std::to_string(multiplayerSettings.m_ServerPort) +
			" " + std::to_string(multiplayerSettings.m_ServerClientPort)))
			this->Close();

		if (dialogResult == wxID_ADD && LaunchGame(additional_args +  "--network client " + multiplayerSettings.m_ClientServerIp +
			" " + std::to_string(multiplayerSettings.m_ClientServerPort) +
			" " + std::to_string(multiplayerSettings.m_ClientPort)))
			this->Close();
	}
}

void MainFrame::OnFile(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	GameFilesDialog dlg(this, settings.m_Paths);
	dlg.SetMinSize(wxSize(430, 195));
	if (dlg.ShowModal() == wxID_OK) {
		auto pathSettings = dlg.GetPaths();
		m_ptrConfig->SavePathsToDoc(pathSettings);
		m_ptrConfig->SaveToFile();
		m_ptrConfig->GetSettingsFromDoc();
	}
}

void MainFrame::OnSettings(wxCommandEvent&)
{
	SettingsDialog dlg(this, m_ptrConfig);
	dlg.SetMinSize(wxSize(300, 290));
	dlg.ShowModal();
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}