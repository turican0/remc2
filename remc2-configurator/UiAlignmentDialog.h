#pragma once
#include "Globals.h"
#include <wx/wx.h>
#include <wx/statline.h>

// Edits the position of the in-game UI elements (graphics.inGameUi): the top bar, the spell icons and the spell menu.
// Each has an alignment (left, center, right) and an X / Y margin. Not used at the 320x200 / 640x480 resolutions.
class UiAlignmentDialog : public wxDialog
{
public:
	UiAlignmentDialog(wxWindow* parent, const Config::Settings::InGameUi& cfg);

	Config::Settings::InGameUi GetSettings() const;

private:
	// The controls of one UI element
	struct ElementControls
	{
		wxChoice* alignment = nullptr;
		wxSpinCtrl* marginX = nullptr;
		wxSpinCtrl* marginY = nullptr;
	};

	void OnOK(wxCommandEvent&);
	void OnResetDefaults(wxCommandEvent&);
	ElementControls AddElementRow(wxFlexGridSizer* grid, const wxString& label, const wxString& tooltip,
		const wxString& marginYTooltip, const Config::Settings::UiElement& element);
	static void SetElementControls(const ElementControls& controls, const Config::Settings::UiElement& element);
	static Config::Settings::UiElement GetElement(const ElementControls& controls, const Config::Settings::UiElement& element);

	// The alignment drop down: its entries in order, as they are written to the config
	static const char* const s_alignments[];
	static int FindAlignmentIndex(const std::string& alignment);

	ElementControls m_topBar;
	ElementControls m_spellIcons;
	ElementControls m_spellMenu;

	Config::Settings::InGameUi m_cfg;
};
