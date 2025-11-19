//#include "RotatedText.h"
//#include <wx/wx.h>
//#include <wx/graphics.h>
//
//#include <memory>
//
////wxBEGIN_EVENT_TABLE(RotatedText, wxWindow)
////EVT_PAINT(RotatedText::OnPaint)
////wxEND_EVENT_TABLE()
//
//RotatedText::RotatedText(wxWindow* parent, const wxString &text, double angle)
//    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
//    m_text(text), m_angle(angle)
//{
//
//    SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
//    wxWindow::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxPanelNameStr);
//
//#if defined(__WXMSW__)
//    int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
//    SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
//#endif
//    this->Bind(wxEVT_PAINT, &RotatedText::OnPaint, this);
//    //SetBackgroundStyle(wxBG_STYLE_PAINT);
//    //SetTransparent(wxByte(0));
//    //SetBackgroundColour(wxColour(255, 255, 255, 0));//parent->GetBackgroundColour());
//}
//
//void RotatedText::SetText(const wxString& text)
//{
//    //m_text = text;
//    //Refresh();
//}
//
//void RotatedText::SetAngle(double angle)
//{
//    //m_angle = angle;
//    //Refresh();
//}
//
//void RotatedText::OnPaint(wxPaintEvent& event)
//{
//    wxPaintDC dc(this);
//
//    std::unique_ptr<wxGraphicsContext> gc{ wxGraphicsContext::Create(dc) };
//
//    if (gc)
//    {
//        DrawOnContext(*gc);
//    }
//
//    //wxAutoBufferedPaintDC dc(this);
//    //dc.Clear();
//
//    // Используем GraphicsContext для поворота текста
//    //wxGraphicsRenderer* renderer = wxGraphicsRenderer::GetDefaultRenderer();
//    //wxGraphicsContext* gc = renderer->CreateContext(dc);
//
//    //if (gc) 
//    //{
//    //    DrawOnContext(*gc);   
//    //}
//}
//
//void RotatedText::DrawOnContext(wxGraphicsContext &gc)
//{
//    //gc.SetFont(GetFont(), dc.GetTextForeground());
//    wxSize size = GetClientSize();
//    wxColor color{ "#EA3B2D" };
//
//    gc.SetBrush(wxBrush(color));
//
//    // Перемещаем начало координат в центр
//    gc.Translate(size.GetWidth() / 2, size.GetHeight() / 2);
//
//    // Поворачиваем на заданный угол
//    gc.Rotate(m_angle * (M_PI / 180.0));
//
//    // Измеряем текст
//    double textWidth, textHeight;
//    gc.GetTextExtent(m_text, &textWidth, &textHeight);
//
//    // Рисуем текст так, чтобы он был центрирован
//    gc.DrawText(m_text, -textWidth / 2, -textHeight / 2);
//}