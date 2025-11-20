#include "SierpinskiTrianglePanel.h"
#include "MainFrame.h"
#include <wx/dcbuffer.h>
#include <cmath>
#include <algorithm>

wxBEGIN_EVENT_TABLE(SierpinskiTrianglePanel, FractalBase)
EVT_PAINT(SierpinskiTrianglePanel::OnPaint)
EVT_TIMER(wxID_ANY, SierpinskiTrianglePanel::OnTimer)
wxEND_EVENT_TABLE()

SierpinskiTrianglePanel::SierpinskiTrianglePanel(wxWindow* parent, MainFrame* mainFrame)
    : FractalBase(parent, mainFrame),
    m_currentIteration(0),
    m_timer(nullptr),
    m_progressLabel(nullptr),
    m_animationComplete(false)
{
    // Создаем общий UI
    CreateCommonUI();

    // Определяем максимальное количество итераций на основе правильных ответов
    int correctAnswers = mainFrame->GetCorrectAnswersCount();

    // 8 стадий сложности (от 0 до 7 итераций)
    m_maxIterations = 0; // Начинаем с 0
    if (correctAnswers >= 2) m_maxIterations = 1;
    if (correctAnswers >= 4) m_maxIterations = 2;
    if (correctAnswers >= 6) m_maxIterations = 3;
    if (correctAnswers >= 8) m_maxIterations = 4;
    if (correctAnswers >= 10) m_maxIterations = 5;
    if (correctAnswers >= 12) m_maxIterations = 6;
    if (correctAnswers >= 14) m_maxIterations = 7;
    if (correctAnswers >= 16) m_maxIterations = 8;

    // Создаем прогресс-лейбл
    wxBoxSizer* mainSizer = dynamic_cast<wxBoxSizer*>(GetSizer());
    if (mainSizer) {
        m_progressLabel = new wxStaticText(this, wxID_ANY, "");
        m_progressLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        m_progressLabel->SetForegroundColour(wxColour(255, 255, 255));
        m_progressLabel->SetBackgroundColour(wxColour(0, 0, 0, 180)); // Полупрозрачный черный фон
        mainSizer->Insert(0, m_progressLabel, 0, wxALIGN_CENTER | wxALL, 10);
    }

    UpdateProgressLabel();
}

SierpinskiTrianglePanel::~SierpinskiTrianglePanel()
{
    StopAnimation();
}

void SierpinskiTrianglePanel::StartAnimation()
{
    if (!m_timer && m_currentIteration < m_maxIterations) {
        m_timer = new wxTimer(this, wxID_ANY);
        m_timer->Start(500); // Анимация с интервалом 500 мс
    }
}

void SierpinskiTrianglePanel::StopAnimation()
{
    if (m_timer && m_timer->IsRunning()) {
        m_timer->Stop();
    }
    delete m_timer;
    m_timer = nullptr;
}

void SierpinskiTrianglePanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    DrawSierpinskiTriangle(dc);
}

void SierpinskiTrianglePanel::OnTimer(wxTimerEvent& event)
{
    if (m_currentIteration < m_maxIterations) {
        m_currentIteration++;
        UpdateProgressLabel();
        Refresh();

        // Если достигли максимума, останавливаем анимацию
        if (m_currentIteration >= m_maxIterations) {
            m_animationComplete = true;
            StopAnimation();
            UpdateProgressLabel(); // Обновляем для показа завершающего сообщения
        }
    }
}

void SierpinskiTrianglePanel::DrawSierpinskiTriangle(wxDC& dc)
{
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight() - 100; // Учитываем место для UI

    if (m_currentIteration == 0 && !m_animationComplete) {
        // Если итерация 0 и анимация не завершена, показываем сообщение
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        wxString message = "Генерация треугольника...";
        wxSize textSize = dc.GetTextExtent(message);
        dc.DrawText(message, width / 2 - textSize.GetWidth() / 2, height / 2 - textSize.GetHeight() / 2);
        return;
    }

    // Устанавливаем цвет в зависимости от прогресса
    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Завершенный треугольник - золотистый цвет
        dc.SetPen(wxPen(wxColour(255, 215, 0), 1));
        dc.SetBrush(wxBrush(wxColour(255, 215, 0, 100))); // Полупрозрачная заливка
    }
    else if (m_animationComplete) {
        // Завершенный но не все вопросы - зеленый
        dc.SetPen(wxPen(wxColour(50, 205, 50), 1));
        dc.SetBrush(wxBrush(wxColour(50, 205, 50, 80)));
    }
    else {
        // В процессе построения - синий
        dc.SetPen(wxPen(wxColour(30, 144, 255), 1));
        dc.SetBrush(wxBrush(wxColour(30, 144, 255, 60)));
    }

    // Вычисляем вершины начального треугольника
    int triangleSize = std::min(width, height) * 0.8;
    wxPoint top(width / 2, (height - triangleSize) / 2);
    wxPoint left(width / 2 - triangleSize / 2, (height - triangleSize) / 2 + triangleSize);
    wxPoint right(width / 2 + triangleSize / 2, (height - triangleSize) / 2 + triangleSize);

    // Рисуем треугольник Серпинского
    SierpinskiRecursive(dc, top, left, right, m_currentIteration);

    // Если треугольник завершен и все вопросы отвечены, добавляем декорации
    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Добавляем контур основного треугольника
        dc.SetPen(wxPen(wxColour(255, 255, 255), 2));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        wxPoint outline[4] = { top, left, right, top };
        dc.DrawLines(4, outline);

        // Добавляем блестящие точки в вершинах
        dc.SetPen(wxPen(wxColour(255, 255, 200)));
        dc.SetBrush(wxBrush(wxColour(255, 255, 200)));

        // Рисуем круги в вершинах
        dc.DrawCircle(top, 5);
        dc.DrawCircle(left, 5);
        dc.DrawCircle(right, 5);
    }
}

void SierpinskiTrianglePanel::SierpinskiRecursive(wxDC& dc, wxPoint p1, wxPoint p2, wxPoint p3, int depth)
{
    if (depth == 0) {
        // База рекурсии - рисуем заполненный треугольник
        wxPoint triangle[3] = { p1, p2, p3 };
        dc.DrawPolygon(3, triangle);
        return;
    }

    // Вычисляем середины сторон
    wxPoint mid12((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    wxPoint mid23((p2.x + p3.x) / 2, (p2.y + p3.y) / 2);
    wxPoint mid31((p3.x + p1.x) / 2, (p3.y + p1.y) / 2);

    // Рекурсивно рисуем три меньших треугольника
    SierpinskiRecursive(dc, p1, mid12, mid31, depth - 1);
    SierpinskiRecursive(dc, mid12, p2, mid23, depth - 1);
    SierpinskiRecursive(dc, mid31, mid23, p3, depth - 1);
}

void SierpinskiTrianglePanel::UpdateProgressLabel()
{
    if (m_progressLabel) {
        int correctAnswers = mainFrame->GetCorrectAnswersCount();
        int totalQuestions = mainFrame->GetTotalQuestions();

        wxString progressText;

        if (m_animationComplete && correctAnswers >= totalQuestions) {
            progressText = wxString::Format("ТРЕУГОЛЬНИК СЕРПИНСКОГО - ЗАВЕРШЕН!\nВсе %d вопросов отвечены правильно!\nИтерация: %d/%d",
                totalQuestions, m_currentIteration, m_maxIterations);
        }
        else if (m_animationComplete) {
            progressText = wxString::Format("Треугольник Серпинского - Построен!\nПравильных ответов: %d/%d\nИтерация: %d/%d",
                correctAnswers, totalQuestions, m_currentIteration, m_maxIterations);
        }
        else {
            progressText = wxString::Format("Треугольник Серпинского - Построение...\nПравильных ответов: %d/%d\nИтерация: %d/%d",
                correctAnswers, totalQuestions, m_currentIteration, m_maxIterations);
        }

        m_progressLabel->SetLabel(progressText);
        m_progressLabel->Wrap(600); // Перенос текста если нужно
    }
}

void SierpinskiTrianglePanel::GenerateSierpinskiTriangle()
{
    // Метод оставлен для совместимости, основная логика в DrawSierpinskiTriangle
}