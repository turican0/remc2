#include "MultiplayerDialog.h"

// ── HostDialog ───────────────────────────────────────────────────────────────
HostDialog::HostDialog(wxWindow* parent, const Config::Settings::Multiplayer& cfg)
	: wxDialog(parent, wxID_ANY, "Host Game",
		wxDefaultPosition, wxDefaultSize,
		wxDEFAULT_DIALOG_STYLE)
{
	m_cfg = cfg;
	auto* sizer = new wxBoxSizer(wxVERTICAL);
	auto* grid = new wxFlexGridSizer(3, 2, 8, 8);
	grid->AddGrowableCol(1, 1);

	// Port
	grid->Add(new wxStaticText(this, wxID_ANY, "Host Port:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_ctrlServerPort = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(180, -1),
		wxSP_ARROW_KEYS, 0, 65535, 3030);
	m_ctrlServerPort->SetValue(cfg.m_ServerPort);
	m_ctrlServerPort->SetToolTip("The Port the Server will run on.");
	grid->Add(m_ctrlServerPort, 1, wxEXPAND);

	// Host Port
	grid->Add(new wxStaticText(this, wxID_ANY, "Host Client Port:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_ctrlServerClientPort = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(180, -1),
		wxSP_ARROW_KEYS, 0, 65535, 3031);
	m_ctrlServerClientPort->SetValue(cfg.m_ServerPort + 1);
	m_ctrlServerClientPort->SetToolTip("The Port the Host Player will use. Should be different to Host Port.");
	grid->Add(m_ctrlServerClientPort, 1, wxEXPAND);

	// Session Recording File
	grid->Add(new wxStaticText(this, wxID_ANY, "Record Session To:"),
		0, wxALIGN_CENTER_VERTICAL);

	auto* recordingSizer = new wxBoxSizer(wxHORIZONTAL);
	m_recordPlayFile = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(180, -1));
	m_recordPlayFile->SetToolTip("To record a playthough");
	m_recordPlayFile->SetValue(cfg.m_RecordPlayFile);
	recordingSizer->Add(m_recordPlayFile, 1, wxEXPAND | wxRIGHT, 4);

	auto* btnBrowse = new wxButton(this, wxID_ANY, "Browse...", wxDefaultPosition, wxSize(70, -1));
	recordingSizer->Add(btnBrowse, 0);
	btnBrowse->Bind(wxEVT_BUTTON, &HostDialog::OnBrowseRecordingFile, this);

	grid->Add(recordingSizer, 1, wxEXPAND);

	sizer->Add(grid, 0, wxEXPAND | wxALL, 12);

	// Debug Network
	m_ctrlDebugNetwork = new wxCheckBox(this, wxID_ANY, "Debug Network");
	m_ctrlDebugNetwork->SetValue(cfg.m_Debug);
	m_ctrlDebugNetwork->SetToolTip("Enable verbose network logging for this session.");
	sizer->Add(m_ctrlDebugNetwork, 0, wxLEFT | wxRIGHT | wxBOTTOM, 12);

	sizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 8);

	auto* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	auto* btnOK = new wxButton(this, wxID_OK, "Host Game");
	auto* btnSave = new wxButton(this, wxID_SAVE, "Save");
	auto* btnCancel = new wxButton(this, wxID_CANCEL);
	btnSizer->Add(btnOK, 0, wxRIGHT, 4);
	btnSizer->Add(btnSave, 0, wxRIGHT, 4);
	btnSizer->Add(btnCancel, 0);

	sizer->Add(btnSizer, 0, wxALIGN_CENTER | wxALL, 8);

	SetSizerAndFit(sizer);
	Centre();

	btnOK->Bind(wxEVT_BUTTON, &HostDialog::OnOK, this);
	btnSave->Bind(wxEVT_BUTTON, &HostDialog::OnSave, this);
	m_ctrlServerPort->Bind(wxEVT_SPINCTRL, &HostDialog::OnPortChanged, this);
}

void HostDialog::OnBrowseRecordingFile(wxCommandEvent&)
{
	wxString defaultDir, defaultFile = m_recordPlayFile->GetValue();
	if (!defaultFile.IsEmpty())
	{
		wxFileName fn(defaultFile);
		defaultDir = fn.GetPath();
		defaultFile = fn.GetFullName();
	}

	wxFileDialog dlg(this, "Save Recording", "", "",
		"Demo files (*.dem)|*.dem|All files (*.*)|*.*",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (dlg.ShowModal() == wxID_OK)
		m_recordPlayFile->SetValue(dlg.GetPath());
}

void HostDialog::OnPortChanged(wxSpinEvent&)
{
	m_ctrlServerClientPort->SetValue(m_ctrlServerPort->GetValue() + 1);
}

void HostDialog::OnSave(wxCommandEvent&)
{
	m_cfg.m_ServerPort = m_ctrlServerPort->GetValue();
	m_cfg.m_ServerClientPort = m_ctrlServerClientPort->GetValue();
	m_cfg.m_RecordPlayFile = m_recordPlayFile->GetValue().Trim();
	m_cfg.m_Debug = m_ctrlDebugNetwork->GetValue();
	EndModal(wxID_SAVE);
}

void HostDialog::OnOK(wxCommandEvent&)
{
	m_cfg.m_ServerPort = m_ctrlServerPort->GetValue();
	m_cfg.m_ServerClientPort = m_ctrlServerClientPort->GetValue();
	m_cfg.m_RecordPlayFile = m_recordPlayFile->GetValue().Trim();
	m_cfg.m_Debug = m_ctrlDebugNetwork->GetValue();
	EndModal(wxID_OK);
}

// ── JoinDialog ───────────────────────────────────────────────────────────────
JoinDialog::JoinDialog(wxWindow* parent, const Config::Settings::Multiplayer& cfg)
	: wxDialog(parent, wxID_ANY, "Join Game",
		wxDefaultPosition, wxDefaultSize,
		wxDEFAULT_DIALOG_STYLE)
{
	m_cfg = cfg;
	auto* sizer = new wxBoxSizer(wxVERTICAL);
	auto* grid = new wxFlexGridSizer(3, 2, 8, 8);
	grid->AddGrowableCol(1, 1);

	// Server IP
	grid->Add(new wxStaticText(this, wxID_ANY, "Server IP:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_ctrlServerIP = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(180, -1));
	m_ctrlServerIP->SetValue(cfg.m_ClientServerIp);
	grid->Add(m_ctrlServerIP, 1, wxEXPAND);

	// Server Port
	grid->Add(new wxStaticText(this, wxID_ANY, "Server Port:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_ctrlServerPort = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(180, -1),
		wxSP_ARROW_KEYS, 0, 65535, 3030);
	m_ctrlServerPort->SetValue(cfg.m_ClientServerPort);
	grid->Add(m_ctrlServerPort, 2, wxEXPAND);

	// Client Port
	grid->Add(new wxStaticText(this, wxID_ANY, "Client Port:"),
		0, wxALIGN_CENTER_VERTICAL);
	m_ctrlClientPort = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(180, -1),
		wxSP_ARROW_KEYS, 0, 65535, 3031);
	m_ctrlClientPort->SetValue(cfg.m_ClientPort);
	m_ctrlClientPort->SetToolTip("The Port this client will use.");
	grid->Add(m_ctrlClientPort, 1, wxEXPAND);

	sizer->Add(grid, 0, wxEXPAND | wxALL, 12);

	// Debug Network
	m_ctrlDebugNetwork = new wxCheckBox(this, wxID_ANY, "Debug Network");
	m_ctrlDebugNetwork->SetValue(cfg.m_Debug);
	m_ctrlDebugNetwork->SetToolTip("Enable verbose network logging for this session.");
	sizer->Add(m_ctrlDebugNetwork, 0, wxLEFT | wxRIGHT | wxBOTTOM, 12);

	sizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 8);

	auto* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	auto* btnOK = new wxButton(this, wxID_OK, "Join Game");
	auto* btnSave = new wxButton(this, wxID_SAVE, "Save");
	auto* btnCancel = new wxButton(this, wxID_CANCEL);
	btnSizer->Add(btnOK, 0, wxRIGHT, 4);
	btnSizer->Add(btnSave, 0, wxRIGHT, 4);
	btnSizer->Add(btnCancel, 0);

	sizer->Add(btnSizer, 0, wxALIGN_CENTER | wxALL, 8);

	SetSizerAndFit(sizer);
	Centre();

	btnOK->Bind(wxEVT_BUTTON, &JoinDialog::OnOK, this);
	btnSave->Bind(wxEVT_BUTTON, &JoinDialog::OnSave, this);
}

void JoinDialog::OnSave(wxCommandEvent&)
{
	m_cfg.m_ClientServerIp = m_ctrlServerIP->GetValue().Trim();
	m_cfg.m_ClientServerPort = m_ctrlServerPort->GetValue();
	m_cfg.m_ClientPort = m_ctrlClientPort->GetValue();
	m_cfg.m_Debug = m_ctrlDebugNetwork->GetValue();
	EndModal(wxID_SAVE);
}

void JoinDialog::OnOK(wxCommandEvent& event)
{
	if (m_ctrlServerIP->GetValue().Trim().IsEmpty())
	{
		wxMessageBox("Please enter the server IP address.",
			"Validation Error", wxOK | wxICON_WARNING, this);
		return;
	}
	m_cfg.m_ClientServerIp = m_ctrlServerIP->GetValue().Trim();
	m_cfg.m_ClientServerPort = m_ctrlServerPort->GetValue();
	m_cfg.m_ClientPort = m_ctrlClientPort->GetValue();
	m_cfg.m_Debug = m_ctrlDebugNetwork->GetValue();
	EndModal(wxID_OK);
}

// ── MultiplayerDialog ────────────────────────────────────────────────────────
MultiplayerDialog::MultiplayerDialog(wxWindow* parent, const Config::Settings::Multiplayer& cfg)
	: wxDialog(parent, wxID_ANY, "Multiplayer",
		wxDefaultPosition, wxDefaultSize,
		wxDEFAULT_DIALOG_STYLE)
{
	m_cfg = cfg;
	auto* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(new wxStaticText(this, wxID_ANY, "Choose a multiplayer mode:"),
		0, wxALL, 12);

	auto* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	auto* btnHost = new wxButton(this, ID_BTN_HOST, "Host");
	btnHost->SetMinSize(wxSize(100, 40));
	auto* btnJoin = new wxButton(this, ID_BTN_JOIN, "Join");
	btnJoin->SetMinSize(wxSize(100, 40));

	btnSizer->Add(btnHost, 0, wxRIGHT, 8);
	btnSizer->Add(btnJoin, 0);

	sizer->Add(btnSizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 12);

	sizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 8);

	auto* closeSizer = new wxStdDialogButtonSizer();
	closeSizer->AddButton(new wxButton(this, wxID_CANCEL, "Close"));
	closeSizer->Realize();
	sizer->Add(closeSizer, 0, wxEXPAND | wxALL, 8);

	SetSizerAndFit(sizer);
	Centre();

	Bind(wxEVT_BUTTON, &MultiplayerDialog::OnHost, this, ID_BTN_HOST);
	Bind(wxEVT_BUTTON, &MultiplayerDialog::OnJoin, this, ID_BTN_JOIN);
}

void MultiplayerDialog::OnHost(wxCommandEvent&)
{
	HostDialog dlg(this, m_cfg);
	int result = dlg.ShowModal();
	if (result == wxID_OK || result == wxID_SAVE)
	{
		m_cfg.m_ServerPort = dlg.GetServerPort();
		m_cfg.m_ServerClientPort = dlg.GetServerClientPort();
		m_cfg.m_RecordPlayFile = dlg.GetRecordPlayFile();
		m_cfg.m_Debug = dlg.GetDebugSession();

		if (result == wxID_OK)
			EndModal(wxID_NETWORK);
		else
			EndModal(wxID_OK);
	}
}

void MultiplayerDialog::OnJoin(wxCommandEvent&)
{
	JoinDialog dlg(this, m_cfg);
	int result = dlg.ShowModal();
	if (result == wxID_OK || result == wxID_SAVE)
	{
		m_cfg.m_ClientServerPort = dlg.GetServerPort();
		m_cfg.m_ClientPort = dlg.GetClientPort();
		m_cfg.m_ClientServerIp = dlg.GetIP();
		m_cfg.m_Debug = dlg.GetDebugSession();

		if (result == wxID_OK)
			EndModal(wxID_ADD);
		else
			EndModal(wxID_OK);
	}
}

Config::Settings::Multiplayer MultiplayerDialog::GetMultiplayer() const
{
	return m_cfg;
}
