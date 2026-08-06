#pragma once
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(wxWindow* parent, Config* config)
	: wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxSize(300, 290),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX),
	m_ptrConfig(config)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	auto* btnGame = new wxButton(panel, wxID_ANY, "Speed");
	btnGame->SetMinSize(wxSize(-1, 40));
	auto* btnControls = new wxButton(panel, wxID_ANY, "Controls");
	btnControls->SetMinSize(wxSize(-1, 40));
	auto* btnSound = new wxButton(panel, wxID_ANY, "Sound");
	btnSound->SetMinSize(wxSize(-1, 40));
	auto* btnDisplay = new wxButton(panel, wxID_ANY, "Display");
	btnDisplay->SetMinSize(wxSize(-1, 40));
	auto* btnGraphics = new wxButton(panel, wxID_ANY, "Graphics");
	btnGraphics->SetMinSize(wxSize(-1, 40));

	auto* buttonSizer = new wxBoxSizer(wxVERTICAL);
	const auto flags = wxSizerFlags(0).Expand().Border(wxTOP | wxBOTTOM, 4);
	buttonSizer->Add(btnGame, flags);
	buttonSizer->Add(btnControls, flags);
	buttonSizer->Add(btnSound, flags);
	buttonSizer->Add(btnDisplay, flags);
	buttonSizer->Add(btnGraphics, flags);
	buttonSizer->SetMinSize(wxSize(300, -1));

	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	vSizer->Add(buttonSizer, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT, 8));

	wxBoxSizer* outerSizer = new wxBoxSizer(wxVERTICAL);
	outerSizer->AddStretchSpacer(1);
	outerSizer->Add(vSizer, wxSizerFlags(0).Centre());
	outerSizer->AddStretchSpacer(1);

	panel->SetSizer(outerSizer);

	auto* dlgSizer = new wxBoxSizer(wxVERTICAL);
	dlgSizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(dlgSizer);

	btnGame->Bind(wxEVT_BUTTON, &SettingsDialog::OnGame, this);
	btnControls->Bind(wxEVT_BUTTON, &SettingsDialog::OnControls, this);
	btnSound->Bind(wxEVT_BUTTON, &SettingsDialog::OnSound, this);
	btnDisplay->Bind(wxEVT_BUTTON, &SettingsDialog::OnDisplay, this);
	btnGraphics->Bind(wxEVT_BUTTON, &SettingsDialog::OnGraphics, this);

	Centre();
}
void SettingsDialog::OnGame(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	GameDialog dlg(this, settings.m_Game);
	dlg.SetMinSize(wxSize(286, 194));
	if (dlg.ShowModal() == wxID_OK) {
		m_ptrConfig->SaveGameToDoc(dlg.GetSettings());
		m_ptrConfig->SaveToFile();
	}
}

void SettingsDialog::OnControls(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	ControlsDialog dlg(this, settings.m_Controls);
	dlg.SetMinSize(wxSize(666, 950));
	if (dlg.ShowModal() == wxID_OK) {
		m_ptrConfig->SaveControlsToDoc(dlg.GetSettings());
		m_ptrConfig->SaveToFile();
	}
}

void SettingsDialog::OnSound(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	SoundDialog dlg(this, settings.m_Sound);
	dlg.SetMinSize(wxSize(404, 332));
	if (dlg.ShowModal() == wxID_OK) {
		m_ptrConfig->SaveSoundToDoc(dlg.GetSettings());
		m_ptrConfig->SaveToFile();
	}
}

void SettingsDialog::OnDisplay(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	DisplayDialog dlg(this, settings.m_Graphics);
	dlg.SetMinSize(wxSize(270, 300));
	if (dlg.ShowModal() == wxID_OK) {
		m_ptrConfig->SaveGraphicsToDoc(dlg.GetSettings());
		m_ptrConfig->SaveToFile();
	}
}

void SettingsDialog::OnGraphics(wxCommandEvent&)
{
	auto settings = m_ptrConfig->GetSettingsFromDoc();
	GraphicsDialog dlg(this, settings.m_Graphics);
	dlg.SetMinSize(wxSize(210, 310));
	if (dlg.ShowModal() == wxID_OK) {
		m_ptrConfig->SaveGraphicsToDoc(dlg.GetSettings());
		m_ptrConfig->SaveToFile();
	}
}