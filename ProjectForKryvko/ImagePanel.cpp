#include "ImagePanel.h"
#include <functional>
#include <wx/dcbuffer.h>

#include <wx/mstream.h>

BEGIN_EVENT_TABLE(ImagePanel, wxWindow)
EVT_ERASE_BACKGROUND(ImagePanel::OnEraseBackground)
END_EVENT_TABLE()

//===========================================================================================================

ImagePanel::ImagePanel(wxWindow* parent, const std::wstring &imagePath, wxWindowID winid) :
	wxPanel(parent, winid, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE),
	bitmap(nullptr),
	m_hasBackground(false)
{
	wxImage::AddHandler(new wxJPEGHandler);
	wxImage::AddHandler(new wxPNGHandler);
	Bind(wxEVT_PAINT, &ImagePanel::OnPaint, this);
	Bind(wxEVT_SIZE, &ImagePanel::OnSize, this);


	const auto w = this->GetSize().GetX();
	const auto h = this->GetSize().GetY();

	wxImage image(w, h);
	image.Clear(255);

	Init(L"def", image);

	SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);

	int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
	SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
}

//===========================================================================================================

ImagePanel::~ImagePanel()
{

}

//===========================================================================================================

void ImagePanel::Init(const std::wstring& path_, const wxImage& image)
{
	if (image.IsOk())
	{
		constexpr auto width = 1920;
		constexpr auto height = 1080;

		bitmap = new wxBitmap(image.Scale(width, height, wxIMAGE_QUALITY_HIGH));
		m_hasBackground = true;
	}
}


//===========================================================================================================

void ImagePanel::LoadFromFile(const std::wstring& path)
{
	wxImage image;
	image.LoadFile(path, wxBITMAP_TYPE_JPEG);

	Init(path, image);

	Refresh();
}

void ImagePanel::LoadFromFilePNG(const std::wstring& path)
{
	wxImage image;
	image.LoadFile(path, wxBITMAP_TYPE_PNG);

	Init(path, image);

	Refresh();
}

//===========================================================================================================

void ImagePanel::Render(wxDC& dc) const
{
	int dc_width, dc_height;
	dc.GetSize(&dc_width, &dc_height);

	if ((*bitmap).IsOk())
	{
		dc.DrawBitmap(*bitmap, 0, 0, false);
	}
}

//===========================================================================================================

void ImagePanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	Render(dc);
}

//===========================================================================================================

void ImagePanel::OnSize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void ImagePanel::OnEraseBackground(wxEraseEvent& event)
{

}
