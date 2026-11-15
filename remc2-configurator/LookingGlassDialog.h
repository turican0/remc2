#pragma once
#include "Globals.h"
#include <wx/statline.h>

// Looking Glass (lenticular display) settings of the hardware renderer: the quilt of views the game draws and how it
// is shown. A device preset fills in the quilt for that display.
class LookingGlassDialog : public wxDialog
{
public:
	LookingGlassDialog(wxWindow* parent, const Config::Settings::LookingGlass& cfg);

	Config::Settings::LookingGlass GetSettings() const;

	// Turning the Looking Glass on (it was off) also sets these, as the views need them: the original view distance
	// (each view is drawn again per frame) and the in-game UI kept in from the screen's edges (they are cut off on the
	// display). The user is told when ticking it; the caller applies them if this was OK'd with it on.
	static constexpr int kViewDistanceScale = 1;
	static constexpr int kUiMargin = 25;   // pixels
	bool TurnedOn() const { return !m_cfg.m_Enabled && m_chkEnabled->GetValue(); }

private:
	// A device's quilt: columns x rows views in a quiltWidth x quiltHeight image (Looking Glass's own settings)
	struct Preset { const char* name; int columns; int rows; int quiltWidth; int quiltHeight; };
	static const Preset s_presets[];
	static const int s_presetCount;
	int CustomIndex() const { return s_presetCount; }   // the last entry in the drop down

	void OnPreset(wxCommandEvent&);
	void OnQuiltChanged(wxSpinEvent&);
	void SelectMatchingPreset();   // the preset the quilt values are, or Custom
	void UpdateQuiltSize();
	void UpdateEnabled();
	void OnBrowseBridge(wxCommandEvent&);
	void OnOK(wxCommandEvent&);
	void OnEnabledChanged(wxCommandEvent&);

	wxCheckBox* m_chkEnabled;
	wxChoice* m_choicePreset;
	wxSpinCtrl* m_spinColumns;
	wxSpinCtrl* m_spinRows;
	wxSpinCtrl* m_spinCellWidth;
	wxSpinCtrl* m_spinCellHeight;
	wxStaticText* m_lblQuiltSize;
	wxSpinCtrl* m_spinViewSpacing;
	wxSpinCtrl* m_spinConvergence;
	wxCheckBox* m_chkDisplay;
	wxTextCtrl* m_txtBridgePath;
	wxButton* m_btnBrowseBridge;

	Config::Settings::LookingGlass m_cfg;
};
