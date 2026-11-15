#pragma once
#include "Globals.h"
#include <wx/statline.h>
#include <wx/clrpicker.h>

// A test image through the chosen lenses: a stereo scene (the two eyes' views of shapes behind, at and in front of the
// screen, and a word only each eye should see) combined as the game's anaglyph.frag does, so it can be checked with the
// glasses on.
class AnaglyphPreview : public wxPanel
{
public:
	AnaglyphPreview(wxWindow* parent, float ghostReduction);

	void SetColours(const wxColour& left, const wxColour& right);
	void SetDarkLevel(bool dark);   // night and cave levels: half-colour, on a dark scene
	// After the dialog is fitted at the full size: lets it shrink to half
	void AllowShrinking() { SetMinSize(wxSize(kWidth / 2, kHeight / 2)); }

private:
	static constexpr int kWidth = 480;   // the test image's size; it is scaled to the panel, keeping its shape
	static constexpr int kHeight = 270;

	void OnPaint(wxPaintEvent&);
	wxImage DrawEye(int eye) const;   // eye: -1 left, +1 right
	void Combine();                   // the eyes into m_image, through the lenses

	wxColour m_left;
	wxColour m_right;
	float m_ghost;
	bool m_dark = false;
	wxImage m_image;
};

// Anaglyph 3D settings of the hardware renderer (F11 in game turns it on and off): the colours of the glasses' lenses.
class AnaglyphDialog : public wxDialog
{
public:
	AnaglyphDialog(wxWindow* parent, const Config::Settings::Anaglyph& cfg);

	Config::Settings::Anaglyph GetSettings() const;

private:
	void OnDefaultColours(wxCommandEvent&);
	void OnOK(wxCommandEvent&);
	void UpdatePreview();
	// A lens: its colour picker with its "#RRGGBB" box beside it, each updating the other
	wxColourPickerCtrl* AddLens(wxFlexGridSizer* grid, const wxString& label, const wxColour& colour,
		const wxString& toolTip, wxTextCtrl*& hexBox);

	// A "#RRGGBB" lens colour from the config (the # optional, as the game reads it), or the fallback if it isn't one
	static wxColour ParseLensColour(const std::string& text, const wxColour& fallback);
	static std::string ToHex(const wxColour& colour);

	// The defaults, as Config::Settings::Anaglyph: red left, cyan dimmed to 66% right (like the original stereo mode)
	static const wxColour s_defaultLeftColour;
	static const wxColour s_defaultRightColour;

	wxColourPickerCtrl* m_pickLeftColour;
	wxColourPickerCtrl* m_pickRightColour;
	wxTextCtrl* m_txtLeftColour;    // the pickers' colours as "#RRGGBB"
	wxTextCtrl* m_txtRightColour;
	AnaglyphPreview* m_preview = nullptr;
	wxCheckBox* m_chkDarkLevel;

	Config::Settings::Anaglyph m_cfg;
};
