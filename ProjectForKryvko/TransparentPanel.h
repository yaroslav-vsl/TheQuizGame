#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>

#include <memory>

class MyTransparentPanel : public wxWindow
{
public:
    MyTransparentPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxPanelNameStr)
        : wxWindow()
    {
        SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
        wxWindow::Create(parent, id, pos, size, style, name);

#if defined(__WXMSW__)
        int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
        SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
#endif

        this->Bind(wxEVT_PAINT, &MyTransparentPanel::OnPaint, this);
    }

    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);

        std::unique_ptr<wxGraphicsContext> gc{ wxGraphicsContext::Create(dc) };

        if (gc)
        {
            DrawOnContext(*gc);
        }
    }

    void DrawOnContext(wxGraphicsContext& gc)
    {

    }
};