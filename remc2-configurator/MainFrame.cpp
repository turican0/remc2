#include "MainFrame.h"

#include "GameFilesCheck.h"

MainFrame::MainFrame(const wxString& title, const std::string fileName) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(430, 710), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	wxInitAllImageHandlers();

	m_ptrConfig = new Config(ToAbsolute(fileName).ToStdString());


	// ── Panel (gives a native background) ───────────────────────────────────
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	wxString path = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPathWithSep() + "images" + wxFileName::GetPathSeparator() + "mc2_hd_logo.png";

	wxBitmap logo(path, wxBITMAP_TYPE_PNG);

	if (!logo.IsOk())
	{
		wxMessageBox("Failed to load images/mc2_hd_logo.png");
	}

	wxStaticBitmap* image = new wxStaticBitmap(panel, wxID_ANY, logo);

	// ── Buttons ─────────────────────────────────────────────────────────────
	m_btnPlayMagicCarpet = new wxButton(panel, ID_BTN_PLAY_MAGIC_CARPET, "Magic Carpet");
	m_btnPlayMagicCarpet->SetMinSize(wxSize(-1, 40));
	m_btnPlayHiddenWorlds = new wxButton(panel, ID_BTN_PLAY_HIDDEN_WORLDS, "Magic Carpet Hidden Worlds");
	m_btnPlayHiddenWorlds->SetMinSize(wxSize(-1, 40));
	m_btnPlayMagicCarpet2 = new wxButton(panel, ID_BTN_PLAY_MAGIC_CARPET_2, "Magic Carpet 2");
	m_btnPlayMagicCarpet2->SetMinSize(wxSize(-1, 40));
	// the picture of each game (images next to the .exe), the text only without it
	SetButtonImage(m_btnPlayMagicCarpet, "magic_carpet.png");
	SetButtonImage(m_btnPlayHiddenWorlds, "hidden_worlds.png");
	SetButtonImage(m_btnPlayMagicCarpet2, "magic_carpet2.png");
	wxButton* btnLaunchOptions = new wxButton(panel, ID_BTN_LAUNCH_OPTIONS, "Launch Options");
	btnLaunchOptions->SetMinSize(wxSize(-1, 40));
	wxButton* btnFile = new wxButton(panel, ID_BTN_FILE, "Game Files");
	btnFile->SetMinSize(wxSize(-1, 40));
	wxButton* btnMultiplayer = new wxButton(panel, ID_BTN_MULTIPLAYER, "Multiplayer");
	btnMultiplayer->SetMinSize(wxSize(-1, 40));
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
	buttonSizer->Add(m_btnPlayMagicCarpet, flags);
	buttonSizer->Add(m_btnPlayHiddenWorlds, flags);
	buttonSizer->Add(m_btnPlayMagicCarpet2, flags);
	buttonSizer->Add(btnLaunchOptions, flags);
	buttonSizer->Add(btnFile, flags);
	buttonSizer->Add(btnMultiplayer, flags);
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
	Bind(wxEVT_BUTTON, &MainFrame::OnPlayMagicCarpet, this, ID_BTN_PLAY_MAGIC_CARPET);
	Bind(wxEVT_BUTTON, &MainFrame::OnPlayHiddenWorlds, this, ID_BTN_PLAY_HIDDEN_WORLDS);
	Bind(wxEVT_BUTTON, &MainFrame::OnPlayMagicCarpet2, this, ID_BTN_PLAY_MAGIC_CARPET_2);
	Bind(wxEVT_BUTTON, &MainFrame::OnLaunchOptions, this, ID_BTN_LAUNCH_OPTIONS);
	Bind(wxEVT_BUTTON, &MainFrame::OnMultiplayer, this, ID_BTN_MULTIPLAYER);
	Bind(wxEVT_BUTTON, &MainFrame::OnFile, this, ID_BTN_FILE);
	Bind(wxEVT_BUTTON, &MainFrame::OnSettings, this, ID_BTN_SETTINGS);
	Bind(wxEVT_BUTTON, &MainFrame::OnExit, this, ID_BTN_EXIT);

	UpdatePlayButtons();

	// taller with the pictures of the games: the window grows to show every button
	const wxSize best = panel->GetBestSize();
	SetClientSize(wxSize(std::max(GetClientSize().x, best.x), std::max(GetClientSize().y, best.y)));

	Centre(); // centre on screen
}

// ── Play buttons ─────────────────────────────────────────────────────────────
void MainFrame::SetButtonImage(wxButton* button, const wxString& fileName)
{
	const wxString path = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPathWithSep() + "images" +
		wxFileName::GetPathSeparator() + fileName;
	if (!wxFileExists(path))
		return;
	wxImage image(path, wxBITMAP_TYPE_PNG);
	if (!image.IsOk())
		return;
	// the height of the picture on the button, the width as the picture
	constexpr int kImageHeight = 64;
	if (image.GetHeight() != kImageHeight)
		image.Rescale(image.GetWidth() * kImageHeight / image.GetHeight(), kImageHeight, wxIMAGE_QUALITY_HIGH);
	button->SetBitmap(wxBitmap(image), wxLEFT);
	button->SetBitmapMargins(8, 4);
	button->SetMinSize(wxSize(-1, kImageHeight + 12));
}

void MainFrame::UpdatePlayButtons()
{
	const auto paths = m_ptrConfig->GetSettingsFromDoc().m_Paths;
	using GameName = Config::Settings::GameName;
	UpdatePlayButton(m_btnPlayMagicCarpet, GameFilesCheck::Missing(paths, GameName::MagicCarpet));
	UpdatePlayButton(m_btnPlayHiddenWorlds, GameFilesCheck::Missing(paths, GameName::HiddenWorlds));
	UpdatePlayButton(m_btnPlayMagicCarpet2, GameFilesCheck::Missing(paths, GameName::MagicCarpet2));
}

void MainFrame::UpdatePlayButton(wxButton* button, const std::vector<wxString>& missing)
{
	button->Enable(missing.empty());
	button->SetToolTip(missing.empty() ? wxString() : "Missing game files:\n" + GameFilesCheck::ToText(missing));
}

// ── Button handlers ──────────────────────────────────────────────────────────
void MainFrame::Play(const wxString& args)
{
	if (LaunchGame(args))
		Close(true); // close the launcher
}

void MainFrame::OnPlayMagicCarpet(wxCommandEvent&)
{
	Play("--game mc1");
}

void MainFrame::OnPlayHiddenWorlds(wxCommandEvent&)
{
	Play("--game mc1hw");
}

void MainFrame::OnPlayMagicCarpet2(wxCommandEvent&)
{
	Play("--game mc2");
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

		if (dialogResult == wxID_NETWORK && LaunchGame(additional_args + "--network server " + std::to_string(multiplayerSettings.m_ServerPort)))
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
	SetDialogMinSize(dlg, wxSize(430, 230));
	if (dlg.ShowModal() == wxID_OK) {
		m_ptrConfig->SavePathsToDoc(dlg.GetPaths());
		m_ptrConfig->SaveToFile();
		UpdatePlayButtons();
	}
}

void MainFrame::OnSettings(wxCommandEvent&)
{
	SettingsDialog dlg(this, m_ptrConfig);
	SetDialogMinSize(dlg, wxSize(300, 290));
	dlg.ShowModal();
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}