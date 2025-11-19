//#pragma once
//#include <wx/wx.h>
//#include <wx/graphics.h>
//
//#include <memory>
//
//class RotatedText : public wxWindow
//{
//public:
//    RotatedText(wxWindow* parent, const wxString& text, double angle)
//        : wxWindow(),
//        m_text(text), m_angle(angle)
//    {
//        SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
//        wxWindow::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxPanelNameStr);
//
//#if defined(__WXMSW__)
//        int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
//        SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
//#endif
//
//        this->Bind(wxEVT_PAINT, &RotatedText::OnPaint, this);
//    }
//
//    wxString m_text;
//    double m_angle;
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
//
//    void DrawOnContext(wxGraphicsContext& gc)
//    {
//        wxSize size = GetClientSize();
//
//        wxColor color{ "#EA3B2D" };
//
//        gc.SetBrush(wxBrush(color));
//        gc.Translate(size.GetWidth() / 2, size.GetHeight() / 2);
//        gc.Rotate(m_angle * (M_PI / 180.0));
//        //wxFont font(wxFontInfo({ 0, size.GetHeight() / 2 }).FaceName("Arial"));
//        wxFont font(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
//        gc.SetFont(font, *wxWHITE);
//
//        double textWidth, textHeight;
//        gc.GetTextExtent(m_text, &textWidth, &textHeight);
//
//        gc.DrawText(m_text, -textWidth / 2, -textHeight / 2);
//    }
//};

#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>
#include <memory>
#include <vector>
#include <wx/tokenzr.h>

class RotatedText : public wxWindow
{
public:
    RotatedText(wxWindow* parent, const wxString& text, double angle, int maxLines = 5, int maxWidth = 700)
        : wxWindow(),
        m_text(text), m_angle(angle), m_maxLines(maxLines), m_maxWidth(maxWidth)
    {
        SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
        wxWindow::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxPanelNameStr);

#if defined(__WXMSW__)
        int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
        SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
#endif

        this->Bind(wxEVT_PAINT, &RotatedText::OnPaint, this);
        this->Bind(wxEVT_SIZE, &RotatedText::OnSize, this);
    }

    void SetText(const wxString& text)
    {
        m_text = text;
        m_wrappedLines.clear(); // Сбрасываем кэш при изменении текста
        Refresh();
    }

    void SetAngle(double angle)
    {
        m_angle = angle;
        Refresh();
    }

    void SetMaxLines(int maxLines)
    {
        m_maxLines = maxLines;
        m_wrappedLines.clear();
        Refresh();
    }

    void SetMaxWidth(int maxWidth)
    {
        m_maxWidth = maxWidth;
        m_wrappedLines.clear();
        Refresh();
    }

    void SetTextColor(const wxColour& color)
    {
        m_textColor = color;
        Refresh();
    }

    void SetFonts(const wxFont& font)
    {
        m_font = font;
        m_wrappedLines.clear();
        Refresh();
    }

    void SetAlignment(wxAlignment align)
    {
        m_alignment = align;
        Refresh();
    }

    void SetLineSpacing(int spacing)
    {
        m_lineSpacing = spacing;
        Refresh();
    }

private:
    wxString m_text;
    double m_angle;
    int m_maxLines;    // Максимальное количество строк (0 - без ограничений)
    int m_maxWidth;    // Максимальная ширина для переноса (0 - авто)
    wxColour m_textColor{ *wxWHITE };
    wxFont m_font{ 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD };
    wxAlignment m_alignment{ wxALIGN_CENTER };
    int m_lineSpacing{ 5 }; // Межстрочный интервал

    std::vector<wxString> m_wrappedLines; // Кэш разбитых на строки текстов

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
        // При изменении размера пересчитываем перенос текста
        m_wrappedLines.clear();
        Refresh();
        event.Skip();
    }

    // Разбивает текст на строки с учетом переноса
    std::vector<wxString> WrapText(wxGraphicsContext& gc, const wxString& text)
    {
        std::vector<wxString> lines;

        if (text.IsEmpty())
            return lines;

        // Если не заданы ограничения, просто разбиваем по символам новой строки
        if (m_maxWidth <= 0 && m_maxLines <= 0)
        {
            wxStringTokenizer tkz(text, "\n");
            while (tkz.HasMoreTokens())
            {
                lines.push_back(tkz.GetNextToken());
            }
            return lines;
        }

        // Разбиваем на слова
        wxArrayString words;
        wxStringTokenizer tkz(text, " \n\t");
        while (tkz.HasMoreTokens())
        {
            words.Add(tkz.GetNextToken());
        }

        if (words.IsEmpty())
            return lines;

        // Автоматический перенос
        wxString currentLine;
        for (size_t i = 0; i < words.GetCount(); i++)
        {
            wxString testLine = currentLine;
            if (!testLine.IsEmpty())
                testLine += " ";
            testLine += words[i];

            double lineWidth, lineHeight;
            gc.GetTextExtent(testLine, &lineWidth, &lineHeight);

            // Проверяем, помещается ли строка
            bool fits = (m_maxWidth <= 0) || (lineWidth <= m_maxWidth);
            bool isNewWordOnNewLine = words[i].StartsWith("\n");

            if (isNewWordOnNewLine || !fits || (m_maxLines > 0 && lines.size() >= static_cast<size_t>(m_maxLines - 1) && i < words.GetCount() - 1))
            {
                // Сохраняем текущую строку и начинаем новую
                if (!currentLine.IsEmpty())
                {
                    lines.push_back(currentLine);
                    currentLine.Clear();
                }

                // Если слово начинается с новой строки, убираем символ
                if (isNewWordOnNewLine)
                {
                    currentLine = words[i].Mid(1); // Пропускаем \n
                }
                else
                {
                    currentLine = words[i];
                }

                // Проверяем ограничение по количеству строк
                if (m_maxLines > 0 && lines.size() >= static_cast<size_t>(m_maxLines - 1))
                {
                    // Если достигли максимума строк, добавляем многоточие
                    if (i < words.GetCount() - 1)
                    {
                        currentLine += "...";
                        lines.push_back(currentLine);
                        return lines;
                    }
                }
            }
            else
            {
                currentLine = testLine;
            }
        }

        // Добавляем последнюю строку
        if (!currentLine.IsEmpty())
        {
            lines.push_back(currentLine);
        }

        return lines;
    }

    // Разбивает текст на отдельные символы (для вертикального отображения)
    std::vector<wxString> SplitToCharacters(const wxString& text)
    {
        std::vector<wxString> characters;
        for (size_t i = 0; i < text.length(); i++)
        {
            characters.push_back(wxString(text[i]));
        }
        return characters;
    }

    void DrawOnContext(wxGraphicsContext& gc)
    {
        wxSize size = GetClientSize();

        // Устанавливаем шрифт и цвет
        gc.SetFont(m_font, m_textColor);

        // Разбиваем текст на строки если нужно
        if (m_wrappedLines.empty())
        {
            // Для углов 90 и 270 градусов разбиваем на символы (вертикальный текст)
            //if (std::abs(std::fmod(m_angle, 180)) == 90)
            //{
                //m_wrappedLines = SplitToCharacters(m_text);
            //}
            //else
            //{
                m_wrappedLines = WrapText(gc, m_text);
            //}
        }

        if (m_wrappedLines.empty())
            return;

        // Перемещаем в центр и поворачиваем
        gc.Translate(size.GetWidth() / 2, size.GetHeight() / 2);
        gc.Rotate(m_angle * (M_PI / 180.0));

        // Вычисляем общую высоту текста
        double totalHeight = 0;
        double maxWidth = 0;
        std::vector<double> lineHeights;
        std::vector<double> lineWidths;

        for (const auto& line : m_wrappedLines)
        {
            double width, height;
            gc.GetTextExtent(line, &width, &height);
            lineWidths.push_back(width);
            lineHeights.push_back(height);
            totalHeight += height + m_lineSpacing;
            maxWidth = std::max(maxWidth, width);
        }

        // Убираем лишний интервал после последней строки
        if (!m_wrappedLines.empty())
        {
            totalHeight -= m_lineSpacing;
        }

        // Рисуем строки
        double currentY = -totalHeight / 2;
        for (size_t i = 0; i < m_wrappedLines.size(); i++)
        {
            double xPos = -lineWidths[i] / 2; // Центрирование по умолчанию

            // Выравнивание по левому краю
            if (m_alignment == wxALIGN_LEFT)
            {
                xPos = -maxWidth / 2;
            }
            // Выравнивание по правому краю
            else if (m_alignment == wxALIGN_RIGHT)
            {
                xPos = maxWidth / 2 - lineWidths[i];
            }
            // Центрирование (по умолчанию)
            else
            {
                xPos = -lineWidths[i] / 2;
            }

            gc.DrawText(m_wrappedLines[i], xPos, currentY);
            currentY += lineHeights[i] + m_lineSpacing;
        }
    }
};