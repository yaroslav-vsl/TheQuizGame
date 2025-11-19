#pragma once
#include <wx/wx.h>

class ImagePanel : public wxPanel
{
private:
	wxBitmap* bitmap;
	int width, height;

private:
	void Init(const std::wstring& path, const wxImage &image);

public:
	explicit ImagePanel(wxWindow* parent, const std::wstring &imagePath, const wxSize &size = wxDefaultSize, wxWindowID winid = -1);
	~ImagePanel() override;

	void Open();
	void LoadFromFile(const std::wstring &path);
	void LoadFromFilePNG(const std::wstring &path);

	void Render(wxDC& dc) const;

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void SetSize(wxSize &size);

protected:
	bool m_hasBackground;
};