#include "LaunchOptionsDialog.h"

LaunchOptionsDialog::LaunchOptionsDialog(wxWindow* parent, std::string launchArguments)
	: wxDialog(parent, wxID_ANY, "Launch Options", wxDefaultPosition, wxSize(430, 460),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	auto* mainSizer = new wxBoxSizer(wxVERTICAL);

	// --- Set Level ---
	auto* levelSizer = new wxBoxSizer(wxHORIZONTAL);
	m_setLevel = new wxCheckBox(this, wxID_ANY, "Set Level");
	m_setLevel->SetToolTip("Jump to a selected Level");
	m_levelSpin = new wxSpinCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0, 25, 0);
	m_levelSpin->Enable(false);
	m_levelSpin->SetToolTip("Jump to a selected Level");
	levelSizer->Add(m_setLevel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	levelSizer->Add(m_levelSpin, 0, wxALIGN_CENTER_VERTICAL);
	mainSizer->Add(levelSizer, 0, wxLEFT | wxRIGHT | wxTOP, 10);

	// --- Custom Map ---
	mainSizer->Add(new wxStaticText(this, wxID_ANY, "Run Custom Map"),
		0, wxLEFT | wxRIGHT | wxTOP, 10);
	auto* mapSizer = new wxBoxSizer(wxHORIZONTAL);
	m_customMap = new wxTextCtrl(this, wxID_ANY);
	m_customMap->SetToolTip("Path of a custom level you have created using the editor");
	m_browseMapBtn = new wxButton(this, wxID_ANY, "Browse...");
	mapSizer->Add(m_customMap, 1, wxRIGHT, 5);
	mapSizer->Add(m_browseMapBtn, 0);
	mainSizer->Add(mapSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	// --- Record / Play ---
	auto* radioSizer = new wxBoxSizer(wxHORIZONTAL);
	m_radioRecord = new wxRadioButton(this, wxID_ANY, "Record", wxDefaultPosition,
		wxDefaultSize, wxRB_GROUP);
	m_radioRecord->SetToolTip("To playback a playthough");

	m_radioPlay = new wxRadioButton(this, wxID_ANY, "Play");
	m_radioPlay->SetToolTip("To record a playthough");
	radioSizer->Add(m_radioRecord, 0, wxRIGHT, 15);
	radioSizer->Add(m_radioPlay, 0);
	mainSizer->Add(radioSizer, 0, wxLEFT | wxRIGHT | wxTOP, 10);

	auto* recordFileSizer = new wxBoxSizer(wxHORIZONTAL);
	m_recordPlayFile = new wxTextCtrl(this, wxID_ANY);
	auto* browseRecordBtn = new wxButton(this, wxID_ANY, "Browse...");
	recordFileSizer->Add(m_recordPlayFile, 1, wxRIGHT, 5);
	recordFileSizer->Add(browseRecordBtn, 0);
	mainSizer->Add(recordFileSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	// --- Log Level ---
	mainSizer->Add(new wxStaticText(this, wxID_ANY, "Log Level"),
		0, wxLEFT | wxRIGHT | wxTOP, 10);
	wxArrayString logLevels;
	logLevels.Add("Info");
	logLevels.Add("Warning");
	logLevels.Add("Debug");
	logLevels.Add("Trace");
	m_logLevel = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, logLevels);
	m_logLevel->SetSelection(0);
	m_logLevel->SetToolTip("To change the log level of the game: Info (default), Warning, Debug or Trace (will impact performance)");
	mainSizer->Add(m_logLevel, 0, wxLEFT | wxRIGHT | wxTOP, 10);

	// --- Enable In-Game Debug ---
	m_enableInGameDebug = new wxCheckBox(this, wxID_ANY, "Enable In-Game Debug");
	m_enableInGameDebug->SetToolTip("To run the game with option to show in game debug info (press 'J' to show)");
	mainSizer->Add(m_enableInGameDebug, 0, wxLEFT | wxRIGHT | wxTOP, 10);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	// --- Additional Arguments ---
	mainSizer->Add(new wxStaticText(this, wxID_ANY, "Custom Arguments"),
		0, wxLEFT | wxRIGHT | wxTOP, 10);
	m_extraArgs = new wxTextCtrl(this, wxID_ANY);
	mainSizer->Add(m_extraArgs, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);

	// --- Command Line (read-only) ---
	mainSizer->Add(new wxStaticText(this, wxID_ANY, "Command Line:"),
		0, wxLEFT | wxRIGHT | wxTOP, 10);
	m_commandLineLabel = new wxStaticText(this, wxID_ANY, "");
	m_commandLineLabel->SetFont(m_commandLineLabel->GetFont().MakeBold());
	mainSizer->Add(m_commandLineLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// --- Buttons ---
	auto* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	auto* btnOK = new wxButton(this, wxID_OK, "Launch Game");
	auto* btnSave = new wxButton(this, wxID_SAVE, "Save");
	auto* btnCancel = new wxButton(this, wxID_CANCEL);
	btnSizer->Add(btnOK, 0, wxRIGHT, 4);
	btnSizer->Add(btnSave, 0, wxRIGHT, 4);
	btnSizer->Add(btnCancel, 0);

	mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxALL, 8);
	SetSizerAndFit(mainSizer);

	// --- Bindings ---
	m_extraArgs->Bind(wxEVT_TEXT, &LaunchOptionsDialog::OnControlChanged, this);
	m_customMap->Bind(wxEVT_TEXT, &LaunchOptionsDialog::OnControlChanged, this);
	m_setLevel->Bind(wxEVT_CHECKBOX, &LaunchOptionsDialog::OnSetLevel, this);
	m_levelSpin->Bind(wxEVT_SPINCTRL, [this](wxSpinEvent&) { BuildCommandLine(); });
	m_radioRecord->Bind(wxEVT_RADIOBUTTON, &LaunchOptionsDialog::OnControlChanged, this);
	m_radioPlay->Bind(wxEVT_RADIOBUTTON, &LaunchOptionsDialog::OnControlChanged, this);
	m_recordPlayFile->Bind(wxEVT_TEXT, &LaunchOptionsDialog::OnControlChanged, this);
	m_logLevel->Bind(wxEVT_CHOICE, &LaunchOptionsDialog::OnControlChanged, this);
	m_enableInGameDebug->Bind(wxEVT_CHECKBOX, &LaunchOptionsDialog::OnControlChanged, this);
	m_browseMapBtn->Bind(wxEVT_BUTTON, &LaunchOptionsDialog::OnBrowseMap, this);
	browseRecordBtn->Bind(wxEVT_BUTTON, &LaunchOptionsDialog::OnBrowseRecordPlay, this);

	SetMinSize(wxSize(430, 460));
	SetSize(wxSize(430, 460));
	Centre();

	btnOK->Bind(wxEVT_BUTTON, &LaunchOptionsDialog::OnOK, this);
	btnSave->Bind(wxEVT_BUTTON, &LaunchOptionsDialog::OnSave, this);

	// --- Populate controls from launchArguments ---
	ParseAndApplyArguments(launchArguments);
}

// ---------------------------------------------------------------------------
// Command line builder
// ---------------------------------------------------------------------------

void LaunchOptionsDialog::BuildCommandLine()
{
	std::string result;

	const auto append = [&](const std::string& token)
		{
			if (!result.empty())
				result += ' ';
			result += token;
		};

	// Free-text additional arguments go first
	const auto extra = m_extraArgs->GetValue().ToStdString();
	if (!extra.empty())
		append(extra);

	// --set_level and --custom_level are mutually exclusive:
	// Set Level takes priority when checked; Custom Map is active only when unchecked.
	if (m_setLevel->IsChecked())
	{
		append("--set_level " + std::to_string(m_levelSpin->GetValue()));
	}
	else
	{
		const auto customLevel = m_customMap->GetValue().ToStdString();
		if (!customLevel.empty())
			append("--custom_level \"" + customLevel + "\"");
	}

	// --record_file / --play_file
	const auto recordPath = m_recordPlayFile->GetValue().ToStdString();
	if (!recordPath.empty())
	{
		if (m_radioRecord->GetValue())
			append("--record_file \"" + recordPath + "\"");
		else
			append("--play_file \"" + recordPath + "\"");
	}

	// --log_level (omit if default "Info")
	const auto logLevel = m_logLevel->GetStringSelection().ToStdString();
	if (logLevel != "Info")
		append("--log_level " + logLevel);

	// --enable_in_game_debug
	if (m_enableInGameDebug->IsChecked())
		append("--enable_in_game_debug");

	m_commandLineLabel->SetLabel(wxString::FromUTF8(result));
	Layout();
}

std::string LaunchOptionsDialog::GetLaunchArguments() const
{
	return m_commandLineLabel->GetLabel().ToStdString();
}

// ---------------------------------------------------------------------------
// Argument parser (populate controls from an existing command line string)
// ---------------------------------------------------------------------------

static std::string ExtractValue(const std::string& args, const std::string& flag)
{
	const auto pos = args.find(flag);
	if (pos == std::string::npos)
		return {};

	auto start = pos + flag.size();
	while (start < args.size() && args[start] == ' ')
		++start;

	if (start >= args.size())
		return {};

	if (args[start] == '"')
	{
		++start;
		const auto end = args.find('"', start);
		if (end == std::string::npos)
			return args.substr(start);
		return args.substr(start, end - start);
	}

	auto end = start;
	while (end < args.size() && args[end] != ' ')
		++end;
	return args.substr(start, end - start);
}

static std::string StripKnownFlags(std::string args)
{
	static const std::vector<std::string> valuedFlags = {
		"--custom_level", "--set_level", "--record_file", "--play_file", "--log_level"
	};
	static const std::vector<std::string> boolFlags = {
		"--enable_in_game_debug"
	};

	for (const auto& flag : boolFlags)
	{
		const auto pos = args.find(flag);
		if (pos != std::string::npos)
			args.erase(pos, flag.size());
	}

	for (const auto& flag : valuedFlags)
	{
		const auto pos = args.find(flag);
		if (pos == std::string::npos)
			continue;

		auto end = pos + flag.size();
		while (end < args.size() && args[end] == ' ')
			++end;
		if (end < args.size() && args[end] == '"')
		{
			++end;
			end = args.find('"', end);
			if (end != std::string::npos)
				++end;
		}
		else
		{
			while (end < args.size() && args[end] != ' ')
				++end;
		}
		args.erase(pos, end - pos);
	}

	// Trim and collapse whitespace
	std::string result;
	bool inSpace = true;
	for (char c : args)
	{
		if (c == ' ')
		{
			if (!inSpace)
				result += ' ';
			inSpace = true;
		}
		else
		{
			result += c;
			inSpace = false;
		}
	}
	if (!result.empty() && result.back() == ' ')
		result.pop_back();
	return result;
}

void LaunchOptionsDialog::ParseAndApplyArguments(const std::string& args)
{
	const auto customLevel = ExtractValue(args, "--custom_level");
	if (!customLevel.empty())
		m_customMap->SetValue(wxString::FromUTF8(customLevel));

	const auto setLevel = ExtractValue(args, "--set_level");
	if (!setLevel.empty())
	{
		try
		{
			m_setLevel->SetValue(true);
			m_levelSpin->Enable(true);
			m_levelSpin->SetValue(std::stoi(setLevel));
		}
		catch (...) {}
	}

	const auto recordFile = ExtractValue(args, "--record_file");
	const auto playFile = ExtractValue(args, "--play_file");
	if (!recordFile.empty())
	{
		m_radioRecord->SetValue(true);
		m_recordPlayFile->SetValue(wxString::FromUTF8(recordFile));
	}
	else if (!playFile.empty())
	{
		m_radioPlay->SetValue(true);
		m_recordPlayFile->SetValue(wxString::FromUTF8(playFile));
	}

	const auto logLevel = ExtractValue(args, "--log_level");
	if (!logLevel.empty())
	{
		const int idx = m_logLevel->FindString(wxString::FromUTF8(logLevel), false);
		if (idx != wxNOT_FOUND)
			m_logLevel->SetSelection(idx);
	}

	if (args.find("--enable_in_game_debug") != std::string::npos)
		m_enableInGameDebug->SetValue(true);

	const auto extra = StripKnownFlags(args);
	if (!extra.empty())
		m_extraArgs->SetValue(wxString::FromUTF8(extra));

	BuildCommandLine();
}

// ---------------------------------------------------------------------------
// Event handlers
// ---------------------------------------------------------------------------

void LaunchOptionsDialog::OnControlChanged(wxCommandEvent&)
{
	BuildCommandLine();
}

void LaunchOptionsDialog::OnSetLevel(wxCommandEvent&)
{
	m_levelSpin->Enable(m_setLevel->IsChecked());
	m_customMap->Enable(!m_setLevel->IsChecked());
	m_browseMapBtn->Enable(!m_setLevel->IsChecked());

	if (m_setLevel->IsChecked())
		m_customMap->SetValue("");

	BuildCommandLine();
}

void LaunchOptionsDialog::OnBrowseMap(wxCommandEvent&)
{
	wxFileDialog dlg(this, "Select Map File", "", "",
		"Map files (*.mc2)|*.mc2|All files (*.*)|*.*",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
		m_customMap->SetValue(dlg.GetPath());
}

void LaunchOptionsDialog::OnBrowseRecordPlay(wxCommandEvent&)
{
	const bool isRecord = m_radioRecord->GetValue();
	wxFileDialog dlg(this, isRecord ? "Save Recording" : "Open Recording", "", "",
		"Demo files (*.dem)|*.dem|All files (*.*)|*.*",
		isRecord ? wxFD_SAVE | wxFD_OVERWRITE_PROMPT
		: wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
		m_recordPlayFile->SetValue(dlg.GetPath());
}

void LaunchOptionsDialog::OnSave(wxCommandEvent&)
{
	EndModal(wxID_SAVE);
}

void LaunchOptionsDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}