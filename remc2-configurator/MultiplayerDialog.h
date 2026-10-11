#pragma once
#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/artprov.h>
#include "PathHelpers.h"
#include "../remc2/portability/Config.h"

// ── HostDialog ───────────────────────────────────────────────────────────────
class HostDialog : public wxDialog
{
public:
	HostDialog(wxWindow* parent, const Config::Settings::Multiplayer& cfg);

	int GetServerPort() const { return m_cfg.m_ServerPort; }
	std::string GetRecordPlayFile() const { return m_cfg.m_RecordPlayFile; }
	bool GetDebugSession() const { return m_cfg.m_Debug; }

private:
	Config::Settings::Multiplayer m_cfg;
	wxSpinCtrl* m_ctrlServerPort = nullptr;
	wxTextCtrl* m_recordPlayFile = nullptr;
	wxCheckBox* m_ctrlDebugNetwork = nullptr;

	void OnPortChanged(wxSpinEvent& event);
	void OnSave(wxCommandEvent&);
	void OnOK(wxCommandEvent&);
	void OnBrowseRecordingFile(wxCommandEvent&);
};

// ── JoinDialog ───────────────────────────────────────────────────────────────
class JoinDialog : public wxDialog
{
public:
	JoinDialog(wxWindow* parent, const Config::Settings::Multiplayer& cfg);

	wxString GetIP()   const { return m_cfg.m_ClientServerIp; }
	int GetServerPort() const { return m_cfg.m_ClientServerPort; }
	int GetClientPort() const { return m_cfg.m_ClientPort; }
	bool GetDebugSession() const { return m_cfg.m_Debug; }

private:
	Config::Settings::Multiplayer m_cfg;

	wxTextCtrl* m_ctrlServerIP = nullptr;
	wxSpinCtrl* m_ctrlServerPort = nullptr;
	wxSpinCtrl* m_ctrlClientPort = nullptr;
	wxCheckBox* m_ctrlDebugNetwork = nullptr;

	void OnSave(wxCommandEvent&);
	void OnOK(wxCommandEvent&);
};

// ── MultiplayerDialog ────────────────────────────────────────────────────────
class MultiplayerDialog : public wxDialog
{
public:
	MultiplayerDialog(wxWindow* parent, const Config::Settings::Multiplayer& cfg);
	Config::Settings::Multiplayer GetMultiplayer() const;

private:
	Config::Settings::Multiplayer m_cfg;
	
	void OnHost(wxCommandEvent&);
	void OnJoin(wxCommandEvent&);

	enum { ID_BTN_HOST = wxID_HIGHEST + 1, ID_BTN_JOIN };
};