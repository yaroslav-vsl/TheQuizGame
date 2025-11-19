#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

#include <memory>

class RotatedText : public wxWindow
{
public:
    RotatedText(wxWindow* parent, const wxString& text, double angle)
        : wxWindow(),
        m_text(text), m_angle(angle)
    {
        SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
        wxWindow::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxPanelNameStr);

#if defined(__WXMSW__)
        int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
        SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
#endif

        this->Bind(wxEVT_PAINT, &RotatedText::OnPaint, this);
    }

    wxString m_text;
    double m_angle;

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
        ////gc.SetFont(GetFont(), dc.GetTextForeground());
        wxSize size = GetClientSize();
        //wxColor color{ "#EA3B2D" };

        //gc.SetBrush(wxBrush(color));

        //// Перемещаем начало координат в центр
        //gc.Translate(size.GetWidth() / 2, size.GetHeight() / 2);

        //// Поворачиваем на заданный угол
        //gc.Rotate(m_angle * (M_PI / 180.0));

        //// Измеряем текст
        //double textWidth, textHeight;
        //gc.GetTextExtent(m_text, &textWidth, &textHeight);

        //// Рисуем текст так, чтобы он был центрирован
        //gc.DrawText(m_text, -textWidth / 2, -textHeight / 2);

        wxColor color{ "#EA3B2D" };

        gc.SetBrush(wxBrush(color));
        gc.Translate(size.GetWidth() / 2, size.GetHeight() / 2);
        gc.Rotate(m_angle * (M_PI / 180.0));
        //wxFont font(wxFontInfo({ 0, size.GetHeight() / 2 }).FaceName("Arial"));
        wxFont font(40, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        gc.SetFont(font, *wxWHITE);

        double textWidth, textHeight;
        gc.GetTextExtent(m_text, &textWidth, &textHeight);

        gc.DrawText(m_text, -textWidth / 2, -textHeight / 2);
    }
};