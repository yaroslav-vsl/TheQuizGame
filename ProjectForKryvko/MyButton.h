//#pragma once
//
//#include <wx/wx.h>
//#include <wx/graphics.h>
//
//#include <memory>
//
//class MyButton : public wxWindow
//{
//public:
//    MyButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style = 0, const wxString& name = wxPanelNameStr)
//        : wxWindow()
//    {
//        SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
//        wxWindow::Create(parent, id, pos, size, style, name);
//
//#if defined(__WXMSW__)
//        int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
//        SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
//#endif
//
//        this->Bind(wxEVT_PAINT, &MyButton::OnPaint, this);
//    }
//
//    void OnPaint(wxPaintEvent& event)
//    {
//        wxPaintDC dc(this);
//
//        std::unique_ptr<wxGraphicsContext> gc{ wxGraphicsContext::Create(dc) };
//
//        if (gc)
//        {
//            DrawOnContext(*gc);
//        }
//    }
//
//    void DrawOnContext(wxGraphicsContext& gc)
//    {
//        //wxColor color{ "#EA3B2D" };
//
//        //gc.SetBrush(wxBrush(color));
//
//        //auto buttonRect = this->GetClientRect();
//
//        //gc.DrawRoundedRectangle(buttonRect.GetLeft(),
//        //    buttonRect.GetTop(),
//        //    buttonRect.GetWidth(),
//        //    buttonRect.GetHeight(),
//        //    buttonRect.GetHeight() / 6);
//
//        //wxFont font(wxFontInfo({ 0, buttonRect.GetHeight() / 2 }).FaceName("Arial"));
//
//        //gc.SetFont(font, *wxWHITE);
//
//        //double textWidth, textHeight;
//        //gc.GetTextExtent(this->text, &textWidth, &textHeight);
//
//        //gc.Clip(buttonRect.GetLeft(),
//        //    buttonRect.GetTop(),
//        //    buttonRect.GetWidth(),
//        //    buttonRect.GetHeight());
//
//        //gc.DrawText(this->text,
//        //    (buttonRect.GetWidth() - textWidth) / 2.0,
//        //    (buttonRect.GetHeight() - textHeight) / 2.0);
//
//        wxImage::AddHandler(new wxPNGHandler);
//        wxImage image;
//        image.LoadFile(L"start.png", wxBITMAP_TYPE_PNG);
//        wxBitmap bitmap;
//        if (image.IsOk())
//        {
//            constexpr auto width = 300;
//            constexpr auto height = 300;
//
//            bitmap = wxBitmap(image.Scale(width, height, wxIMAGE_QUALITY_HIGH));
//
//        }
//
//        double dc_width, dc_height;
//        gc.GetSize(&dc_width, &dc_height);
//
//        if ((bitmap).IsOk())
//        {
//            gc.DrawBitmap(bitmap, 0, 0, 300, 300);
//        }
//    }
//};

#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>
#include <memory>
#include <wx/dcbuffer.h>

class MyButton : public wxWindow
{
public:
        MyButton(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style = 0, const wxString& name = wxPanelNameStr)
            : wxWindow()
        {
            SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
            wxWindow::Create(parent, id, pos, size, style, name);

#if defined(__WXMSW__)
        int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
        SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
#endif

        // Загружаем изображение при создании
        LoadImage();

        this->Bind(wxEVT_PAINT, &MyButton::OnPaint, this);
        this->Bind(wxEVT_SIZE, &MyButton::OnSize, this);
        this->Bind(wxEVT_ERASE_BACKGROUND, &MyButton::OnEraseBackground, this);
    }

    // Метод для загрузки другого изображения
    bool LoadImage(const wxString& filename = "start.png", wxBitmapType type = wxBITMAP_TYPE_PNG)
    {
        wxImage::AddHandler(new wxPNGHandler);

        wxImage image;
        if (image.LoadFile(filename, type) && image.IsOk())
        {
            m_originalBitmap = wxBitmap(image);
            m_imageLoaded = true;
            Refresh();
            return true;
        }
        else
        {
            wxLogError("Не удалось загрузить изображение: %s", filename);
            m_imageLoaded = false;
            return false;
        }
    }

private:
    wxBitmap m_originalBitmap;
    bool m_imageLoaded{ false };
    bool m_autoScale{ true };
    int m_targetWidth{ 300 };
    int m_targetHeight{ 300 };

    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        std::unique_ptr<wxGraphicsContext> gc{ wxGraphicsContext::Create(dc) };

        if (gc)
        {
            DrawOnContext(*gc);
        }
    }

    void OnSize(wxSizeEvent& event)
    {
        // При изменении размера перерисовываем
        Refresh();
        event.Skip();
    }

    void OnEraseBackground(wxEraseEvent& event)
    {
        // Предотвращаем стандартную отрисовку фона
        // Это важно для прозрачности
    }

    void DrawOnContext(wxGraphicsContext& gc)
    {
        if (!m_originalBitmap.IsOk())
            return;

        wxSize clientSize = GetClientSize();
        int width = clientSize.GetWidth();
        int height = clientSize.GetHeight();

        // Определяем размеры для отрисовки
        int drawWidth, drawHeight;

            drawWidth = m_targetWidth;
            drawHeight = m_targetHeight;

        int x = (width - drawWidth) / 2;
        int y = (height - drawHeight) / 2;

        // Масштабируем bitmap если нужно
        wxBitmap drawBitmap = m_originalBitmap;

        // Рисуем bitmap
        if (m_originalBitmap.IsOk())
        {
            gc.DrawBitmap(m_originalBitmap, x, y, drawWidth, drawHeight);
        }

    }
};