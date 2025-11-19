#include "TransparentImagePanel.h"
#include <wx/dcbuffer.h>

#ifdef __WXMSW__
#include <wx/msw/private.h>
#include <windows.h>
#endif

wxBEGIN_EVENT_TABLE(TransparentImagePanel, wxPanel)
EVT_PAINT(TransparentImagePanel::OnPaint)
EVT_ERASE_BACKGROUND(TransparentImagePanel::OnEraseBackground)
EVT_SIZE(TransparentImagePanel::OnSize)
wxEND_EVENT_TABLE()

TransparentImagePanel::TransparentImagePanel(wxWindow* parent,
    const wxString& imagePath,
    bool transparent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE),
    m_hasBackground(false),
    m_isTransparent(transparent)
{
    // Критически важно для прозрачности
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Устанавливаем полностью прозрачный цвет фона
    SetBackgroundColour(wxColour(0, 0, 0, 0));

    if (!imagePath.IsEmpty()) {
        SetBackgroundImage(imagePath);
    }

#ifdef __WXMSW__
    if (m_isTransparent) {
        // Устанавливаем стиль прозрачного окна для Windows
        LONG_PTR style = ::GetWindowLongPtr(GetHWND(), GWL_EXSTYLE);
        style |= WS_EX_LAYERED;
        ::SetWindowLongPtr(GetHWND(), GWL_EXSTYLE, style);

        // Устанавливаем уровень прозрачности (0-255, где 255 - полностью непрозрачно)
        ::SetLayeredWindowAttributes(GetHWND(), 0, 255, LWA_ALPHA);
    }
#endif
}

void TransparentImagePanel::SetBackgroundImage(const wxString& imagePath)
{
    wxImage image;
    if (image.LoadFile(imagePath, wxBITMAP_TYPE_ANY)) {
        m_backgroundBitmap = wxBitmap(image);
        m_hasBackground = true;
        Refresh();
    }
}

void TransparentImagePanel::SetTransparent(bool transparent)
{
    m_isTransparent = transparent;

#ifdef __WXMSW__
    if (transparent) {
        LONG_PTR style = ::GetWindowLongPtr(GetHWND(), GWL_EXSTYLE);
        style |= WS_EX_LAYERED;
        ::SetWindowLongPtr(GetHWND(), GWL_EXSTYLE, style);
        ::SetLayeredWindowAttributes(GetHWND(), 0, 255, LWA_ALPHA);
    }
    else {
        LONG_PTR style = ::GetWindowLongPtr(GetHWND(), GWL_EXSTYLE);
        style &= ~WS_EX_LAYERED;
        ::SetWindowLongPtr(GetHWND(), GWL_EXSTYLE, style);
    }
#endif

    Refresh();
}

void TransparentImagePanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);

    if (m_isTransparent) {
        // Прозрачный режим - очищаем прозрачным цветом
        dc.SetBackground(*wxTRANSPARENT_BRUSH);
        dc.Clear();

    }
    else if (m_hasBackground) {
        // Режим с фоновым изображением
        wxSize size = GetClientSize();
        if (size.GetWidth() > 0 && size.GetHeight() > 0) {
            wxBitmap scaledBitmap = wxBitmap(m_backgroundBitmap.ConvertToImage()
                .Scale(size.GetWidth(), size.GetHeight(),
                    wxIMAGE_QUALITY_HIGH));
            dc.DrawBitmap(scaledBitmap, 0, 0, false);
        }
    }
    else {
        // Обычный режим с цветом фона
        dc.SetBackground(GetBackgroundColour());
        dc.Clear();
    }
}

void TransparentImagePanel::OnEraseBackground(wxEraseEvent& event)
{
    if (m_isTransparent) {
        // Предотвращаем стандартную отрисовку фона для прозрачных панелей
        return;
    }

    // Разрешаем стандартную обработку для других случаев
    event.Skip();
}

void TransparentImagePanel::OnSize(wxSizeEvent& event)
{
    // При изменении размера перерисовываем
    Refresh();
    event.Skip();
}

#ifdef __WXMSW__
WXLRESULT TransparentImagePanel::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    switch (message) {
    case WM_ERASEBKGND:
        if (m_isTransparent) {
            return 1; // Говорим Windows, что фон уже "очищен"
        }
        break;

    case WM_PAINT:
        // Обеспечиваем правильную обработку WM_PAINT
        break;
    }

    return wxPanel::MSWWindowProc(message, wParam, lParam);
}
#endif