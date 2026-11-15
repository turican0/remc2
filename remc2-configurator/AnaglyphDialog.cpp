#include "AnaglyphDialog.h"
#include <wx/dcbuffer.h>
#include <algorithm>

// ── AnaglyphPreview ───────────────────────────────────────────────────────────
AnaglyphPreview::AnaglyphPreview(wxWindow* parent, float ghostReduction)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(kWidth, kHeight), wxBORDER_SIMPLE),
	m_ghost((std::clamp)(ghostReduction, 0.0f, 0.5f))
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);   // all drawn in OnPaint (double buffered, no flicker)
	Bind(wxEVT_PAINT, &AnaglyphPreview::OnPaint, this);
	Bind(wxEVT_SIZE, [this](wxSizeEvent& evt) { Refresh(); evt.Skip(); });
}

void AnaglyphPreview::SetColours(const wxColour& left, const wxColour& right)
{
	m_left = left;
	m_right = right;
	Combine();
	Refresh();
}

void AnaglyphPreview::SetDarkLevel(bool dark)
{
	m_dark = dark;
	Combine();
	Refresh();
}

// One eye's view: sky and ground, then a far block, a disc and a near square, each moved sideways by its depth (each
// eye the other way: moved apart they are seen behind the screen, crossed in front), and a word only this eye gets
wxImage AnaglyphPreview::DrawEye(int eye) const
{
	// Night and cave levels are dark and blue
	const auto tint = [this](int r, int g, int b) {
		return m_dark ? wxColour(r * 30 / 100, g * 35 / 100, b * 55 / 100) : wxColour(r, g, b);
		};

	wxBitmap bitmap(kWidth, kHeight, 24);
	{
		wxMemoryDC dc(bitmap);
		dc.SetPen(*wxTRANSPARENT_PEN);
		const int horizon = kHeight * 3 / 5;
		dc.GradientFillLinear(wxRect(0, 0, kWidth, horizon), tint(120, 160, 235), tint(200, 215, 250), wxSOUTH);
		dc.GradientFillLinear(wxRect(0, horizon, kWidth, kHeight - horizon), tint(150, 110, 70), tint(95, 65, 40), wxSOUTH);

		// Each shape and its label are moved by its depth: the pixels each eye's image is moved, > 0 behind the screen,
		// < 0 in front
		dc.SetFont(wxFontInfo(10).Bold());
		dc.SetTextForeground(tint(255, 255, 255));
		const auto label = [&](int dx, int centreX, int y, const wxString& text) {
			const wxSize size = dc.GetTextExtent(text);
			dc.DrawText(text, centreX + dx - size.x / 2, y);
			};

		int dx = eye * 12;
		dc.SetBrush(wxBrush(tint(225, 225, 235)));
		dc.DrawRectangle(60 + dx, 70, 110, 110);
		label(dx, 115, 186, "behind");

		dx = 0;
		dc.SetBrush(wxBrush(tint(230, 130, 50)));
		dc.DrawEllipse(190 + dx, 85, 100, 100);
		label(dx, 240, 191, "screen");

		dx = eye * -14;
		dc.SetBrush(wxBrush(tint(60, 110, 230)));
		dc.DrawRectangle(320 + dx, 100, 90, 90);
		label(dx, 365, 196, "in front");

		// The word for this eye alone: with one eye shut, the open eye should see only its own (the other faint at most)
		dc.SetFont(wxFontInfo(22).Bold());
		dc.SetTextForeground(tint(255, 255, 255));
		const wxString word = eye < 0 ? "LEFT" : "RIGHT";
		const wxSize size = dc.GetTextExtent(word);
		dc.DrawText(word, eye < 0 ? 16 : kWidth - 16 - size.x, 14);
		dc.SelectObject(wxNullBitmap);
	}
	return bitmap.ConvertToImage();
}

// As anaglyph.frag: day levels in full colour, dark levels half-colour (the left eye as its brightness), with the
// ghost reduction, each eye through its lens' colour and the two added
void AnaglyphPreview::Combine()
{
	if (!m_left.IsOk() || !m_right.IsOk())
		return;

	const wxImage leftEye = DrawEye(-1);
	const wxImage rightEye = DrawEye(1);
	m_image.Create(kWidth, kHeight, false);

	const unsigned char* l = leftEye.GetData();
	const unsigned char* r = rightEye.GetData();
	unsigned char* out = m_image.GetData();
	const float leftLens[3] = { m_left.Red() / 255.0f, m_left.Green() / 255.0f, m_left.Blue() / 255.0f };
	const float rightLens[3] = { m_right.Red() / 255.0f, m_right.Green() / 255.0f, m_right.Blue() / 255.0f };
	const float k = m_ghost;
	const auto luma = [](const float c[3]) { return 0.299f * c[0] + 0.587f * c[1] + 0.114f * c[2]; };
	const auto clamp01 = [](float v) { return (std::clamp)(v, 0.0f, 1.0f); };

	for (int i = 0; i < kWidth * kHeight; i++, l += 3, r += 3, out += 3)
	{
		float left[3] = { l[0] / 255.0f, l[1] / 255.0f, l[2] / 255.0f };
		const float right[3] = { r[0] / 255.0f, r[1] / 255.0f, r[2] / 255.0f };
		float rightLeak[3] = { right[0], right[1], right[2] };
		if (m_dark)
		{
			const float leftY = luma(left);
			const float rightY = luma(right);
			for (int c = 0; c < 3; c++)
			{
				left[c] = leftY;
				rightLeak[c] = rightY;
			}
		}
		for (int c = 0; c < 3; c++)
		{
			const float leftShown = clamp01(k + (1.0f - k) * left[c] - k * rightLeak[c]);
			const float rightShown = clamp01(k + (1.0f - k) * right[c] - k * left[c]);
			out[c] = (unsigned char)(clamp01(leftShown * leftLens[c] + rightShown * rightLens[c]) * 255.0f + 0.5f);
		}
	}
}

// The test image as big as fits, keeping its shape, centred on black
void AnaglyphPreview::OnPaint(wxPaintEvent&)
{
	wxAutoBufferedPaintDC dc(this);
	dc.SetBackground(*wxBLACK_BRUSH);
	dc.Clear();
	if (!m_image.IsOk())
		return;

	const wxSize client = GetClientSize();
	const double scale = (std::min)((double)client.x / kWidth, (double)client.y / kHeight);
	const int width = (std::max)(1, (int)(kWidth * scale));
	const int height = (std::max)(1, (int)(kHeight * scale));
	const wxBitmap scaled(m_image.Scale(width, height, wxIMAGE_QUALITY_BILINEAR));
	dc.DrawBitmap(scaled, (client.x - width) / 2, (client.y - height) / 2);
}

// ── AnaglyphDialog ────────────────────────────────────────────────────────────
const wxColour AnaglyphDialog::s_defaultLeftColour(0xFF, 0x00, 0x00);
const wxColour AnaglyphDialog::s_defaultRightColour(0x00, 0xA8, 0xA8);

// ── AnaglyphDialog ────────────────────────────────────────────────────────────
AnaglyphDialog::AnaglyphDialog(wxWindow* parent, const Config::Settings::Anaglyph& cfg)
	: wxDialog(parent, wxID_ANY, "Anaglyph 3D Settings",
		wxDefaultPosition, wxSize(675, 650),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_cfg = cfg;

	auto* main = new wxBoxSizer(wxVERTICAL);

	auto* intro = new wxStaticText(this, wxID_ANY,
		"3D for red / cyan glasses. F11 in game turns it on and off.");
	main->Add(intro, 0, wxLEFT | wxRIGHT | wxTOP, 14);

	auto* grid = new wxFlexGridSizer(/*rows*/2, /*cols*/3, /*vgap*/10, /*hgap*/12);
	grid->AddGrowableCol(1, 1);

	// ── Lenses ────────────────────────────────────────────────────────────────
	m_pickLeftColour = AddLens(grid, "Left lens colour:", ParseLensColour(cfg.m_LeftColour, s_defaultLeftColour),
		"The colour of the glasses' left lens. Default red (#FF0000).", m_txtLeftColour);
	m_pickRightColour = AddLens(grid, "Right lens colour:", ParseLensColour(cfg.m_RightColour, s_defaultRightColour),
		"The colour of the glasses' right lens. Default cyan dimmed to 66% (#00A8A8), like the original stereo mode. "
		"#00FFFF for full strength cyan, #0000FF for glasses with a blue right lens.", m_txtRightColour);

	main->Add(grid, 0, wxEXPAND | wxALL, 14);

	auto* btnDefaults = new wxButton(this, wxID_ANY, "Default Colours");
	btnDefaults->SetToolTip("Red left, cyan dimmed to 66% right.");
	main->Add(btnDefaults, 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM, 14);
	btnDefaults->Bind(wxEVT_BUTTON, &AnaglyphDialog::OnDefaultColours, this);

	// ── Test image ────────────────────────────────────────────────────────────
	auto* previewBox = new wxStaticBoxSizer(wxVERTICAL, this, "Test image (look at it with the glasses on)");
	m_preview = new AnaglyphPreview(previewBox->GetStaticBox(), cfg.m_GhostReduction);
	m_preview->SetToolTip("The grey block should be behind the screen, the orange disc on it and the blue square in "
		"front. With one eye shut, the open eye should see only its own word (LEFT or RIGHT); a faint other word is "
		"ghosting.");
	previewBox->Add(m_preview, 1, wxEXPAND | wxALL, 6);
	m_chkDarkLevel = new wxCheckBox(previewBox->GetStaticBox(), wxID_ANY, "Night / cave level");
	m_chkDarkLevel->SetToolTip("Shows a dark scene the way night and cave levels are drawn: the left eye as its "
		"brightness (half-colour), so the dark scene still reaches it.");
	previewBox->Add(m_chkDarkLevel, 0, wxLEFT | wxRIGHT | wxBOTTOM, 6);
	m_chkDarkLevel->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent&) { m_preview->SetDarkLevel(m_chkDarkLevel->GetValue()); });
	main->Add(previewBox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 14);
	UpdatePreview();

	main->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 14);

	// ── OK / Cancel ───────────────────────────────────────────────────────────
	auto* btnSizer = new wxStdDialogButtonSizer();
	auto* btnOK = new wxButton(this, wxID_OK, "OK");
	auto* btnCancel = new wxButton(this, wxID_CANCEL, "Cancel");
	btnOK->SetDefault();
	btnSizer->AddButton(btnOK);
	btnSizer->AddButton(btnCancel);
	btnSizer->Realize();
	main->Add(btnSizer, 0, wxEXPAND | wxALL, 10);

	SetSizer(main);
	btnOK->Bind(wxEVT_BUTTON, &AnaglyphDialog::OnOK, this);

	// Opens with the test image at full size; can shrink to the image at half size, but no smaller
	Fit();
	m_preview->AllowShrinking();
	SetMinSize(ClientToWindowSize(main->GetMinSize()));
	Centre();
}

wxColourPickerCtrl* AnaglyphDialog::AddLens(wxFlexGridSizer* grid, const wxString& label, const wxColour& colour,
	const wxString& toolTip, wxTextCtrl*& hexBox)
{
	grid->Add(new wxStaticText(this, wxID_ANY, label), 0, wxALIGN_CENTER_VERTICAL);

	auto* picker = new wxColourPickerCtrl(this, wxID_ANY, colour);
	picker->SetToolTip(toolTip);
	grid->Add(picker, 0, wxEXPAND);

	// Wide enough for "#RRGGBB"; at most 7 characters
	auto* hex = new wxTextCtrl(this, wxID_ANY, ToHex(colour), wxDefaultPosition,
		wxSize(GetTextExtent("#WWWWWW").GetWidth() + 12, -1));
	hex->SetMaxLength(7);
	hex->SetToolTip("The colour as #RRGGBB. Type a value to set it.");
	grid->Add(hex, 0, wxALIGN_CENTER_VERTICAL);
	hexBox = hex;

	// The picker shows in the box; a whole "#RRGGBB" typed in the box sets the picker (anything else waits for more)
	picker->Bind(wxEVT_COLOURPICKER_CHANGED, [this, hex](wxColourPickerEvent& evt) {
		hex->ChangeValue(ToHex(evt.GetColour()));
		UpdatePreview();
		});
	hex->Bind(wxEVT_TEXT, [this, hex, picker](wxCommandEvent&) {
		const wxColour typed = ParseLensColour(hex->GetValue().ToStdString(), wxNullColour);
		if (typed.IsOk())
		{
			picker->SetColour(typed);
			UpdatePreview();
		}
		});
	// Leaving the box puts back the picker's colour, so a part typed value doesn't stay shown
	hex->Bind(wxEVT_KILL_FOCUS, [hex, picker](wxFocusEvent& evt) {
		hex->ChangeValue(ToHex(picker->GetColour()));
		evt.Skip();
		});

	return picker;
}

void AnaglyphDialog::OnDefaultColours(wxCommandEvent&)
{
	m_pickLeftColour->SetColour(s_defaultLeftColour);
	m_pickRightColour->SetColour(s_defaultRightColour);
	m_txtLeftColour->ChangeValue(ToHex(s_defaultLeftColour));
	m_txtRightColour->ChangeValue(ToHex(s_defaultRightColour));
	UpdatePreview();
}

void AnaglyphDialog::UpdatePreview()
{
	if (m_preview)   // not while the lenses are being made
		m_preview->SetColours(m_pickLeftColour->GetColour(), m_pickRightColour->GetColour());
}

void AnaglyphDialog::OnOK(wxCommandEvent&)
{
	EndModal(wxID_OK);
}

wxColour AnaglyphDialog::ParseLensColour(const std::string& text, const wxColour& fallback)
{
	const size_t start = (!text.empty() && text[0] == '#') ? 1 : 0;
	if (text.size() != start + 6 || text.find_first_not_of("0123456789abcdefABCDEF", start) != std::string::npos)
		return fallback;
	const unsigned long rgb = std::stoul(text.substr(start), nullptr, 16);
	return wxColour((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}

// "#RRGGBB", as the game reads it
std::string AnaglyphDialog::ToHex(const wxColour& colour)
{
	return wxString::Format("#%02X%02X%02X", colour.Red(), colour.Green(), colour.Blue()).ToStdString();
}

Config::Settings::Anaglyph AnaglyphDialog::GetSettings() const
{
	Config::Settings::Anaglyph cfg = m_cfg;   // keeps the ghost reduction

	cfg.m_LeftColour = ToHex(m_pickLeftColour->GetColour());
	cfg.m_RightColour = ToHex(m_pickRightColour->GetColour());

	return cfg;
}
