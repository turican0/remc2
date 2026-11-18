#include "MainFrame.h"

#include <filesystem>

MainFrame::MainFrame(const wxString& title, const std::string fileName) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(416, 616), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
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
	m_btnPlayMagicCarpet = new wxButton(panel, ID_BTN_PLAY_MAGIC_CARPET, "Magic Carpet");
	m_btnPlayMagicCarpet->SetMinSize(wxSize(-1, 40));
	m_btnPlayHiddenWorlds = new wxButton(panel, ID_BTN_PLAY_HIDDEN_WORLDS, "Magic Carpet Hidden Worlds");
	m_btnPlayHiddenWorlds->SetMinSize(wxSize(-1, 40));
	m_btnPlayMagicCarpet2 = new wxButton(panel, ID_BTN_PLAY_MAGIC_CARPET_2, "Magic Carpet 2");
	m_btnPlayMagicCarpet2->SetMinSize(wxSize(-1, 40));
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
	buttonSizer->Add(m_btnPlayMagicCarpet, flags);
	buttonSizer->Add(m_btnPlayHiddenWorlds, flags);
	buttonSizer->Add(m_btnPlayMagicCarpet2, flags);
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
	Bind(wxEVT_BUTTON, &MainFrame::OnPlayMagicCarpet, this, ID_BTN_PLAY_MAGIC_CARPET);
	Bind(wxEVT_BUTTON, &MainFrame::OnPlayHiddenWorlds, this, ID_BTN_PLAY_HIDDEN_WORLDS);
	Bind(wxEVT_BUTTON, &MainFrame::OnPlayMagicCarpet2, this, ID_BTN_PLAY_MAGIC_CARPET_2);
	Bind(wxEVT_BUTTON, &MainFrame::OnLaunchOptions, this, ID_BTN_LAUNCH_OPTIONS);
	Bind(wxEVT_BUTTON, &MainFrame::OnMultiplayer, this, ID_BTN_MULTIPLAYER);
	Bind(wxEVT_BUTTON, &MainFrame::OnFile, this, ID_BTN_FILE);
	Bind(wxEVT_BUTTON, &MainFrame::OnSettings, this, ID_BTN_SETTINGS);
	Bind(wxEVT_BUTTON, &MainFrame::OnExit, this, ID_BTN_EXIT);

	UpdatePlayButtons();

	Centre(); // centre on screen
}

// ── Play buttons ─────────────────────────────────────────────────────────────
void MainFrame::UpdatePlayButtons()
{
	const auto paths = m_ptrConfig->GetSettingsFromDoc().m_Paths;
	using GameName = Config::Settings::GameName;
	using PathKey = Config::Settings::PathKey;
	// the installer fills both folders of each game (the game files and the extracted CD files)
	auto folders = [&paths](const char* name) {
		return std::vector<std::string>{ paths.Get(name, PathKey::GameFolder), paths.Get(name, PathKey::CdFolder) };
	};
	UpdatePlayButton(m_btnPlayMagicCarpet, folders(GameName::MagicCarpet));
	UpdatePlayButton(m_btnPlayHiddenWorlds, folders(GameName::HiddenWorlds));
	UpdatePlayButton(m_btnPlayMagicCarpet2, folders(GameName::MagicCarpet2));
}

void MainFrame::UpdatePlayButton(wxButton* button, const std::vector<std::string>& folders)
{
	wxString missing;
	for (const auto& folder : folders)
	{
		if (!FolderHasFiles(folder))
			missing += "\n" + (folder.empty() ? wxString("(no folder set)") : ToAbsolute(wxString::FromUTF8(folder)));
	}
	button->Enable(missing.empty());
	button->SetToolTip(missing.empty() ? wxString() : "No game files in:" + missing);
}

bool MainFrame::FolderHasFiles(const std::string& folder)
{
	if (folder.empty())
		return false;
	std::error_code error;
	const std::filesystem::path path(ToAbsolute(wxString::FromUTF8(folder)).ToStdWstring());
	if (!std::filesystem::is_directory(path, error))
		return false;
	for (std::filesystem::recursive_directory_iterator it(path, std::filesystem::directory_options::skip_permission_denied, error), end;
		!error && it != end; it.increment(error))
	{
		if (it->is_regular_file(error))
			return true;
	}
	return false;
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
	SetDialogMinSize(dlg, wxSize(430, 195));
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