#include "KochSnowflakePanel.h"
#include "MainFrame.h"
#include <wx/dcbuffer.h>
#include <cmath>
#include <algorithm>

wxBEGIN_EVENT_TABLE(KochSnowflakePanel, FractalBase)
EVT_PAINT(KochSnowflakePanel::OnPaint)
EVT_TIMER(wxID_ANY, KochSnowflakePanel::OnTimer)
wxEND_EVENT_TABLE()

KochSnowflakePanel::KochSnowflakePanel(wxWindow* parent, MainFrame* mainFrame)
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
    int totalQuestions = mainFrame->GetTotalQuestions();

    m_maxIterations = 0; // Начинаем с 0
    if (correctAnswers == 1) m_maxIterations = 1;
    if (correctAnswers == 2) m_maxIterations = 2;
    if (correctAnswers == 3) m_maxIterations = 3;
    if (correctAnswers == 4) m_maxIterations = 4;
    if (correctAnswers == 5) m_maxIterations = 5;
    if (correctAnswers == 6) m_maxIterations = 6;
    if (correctAnswers == 7) m_maxIterations = 7;
    if (correctAnswers == 8) m_maxIterations = 8;
    if (correctAnswers == 9) m_maxIterations = 9;
    if (correctAnswers == 10) m_maxIterations = 10;
    if (correctAnswers == 11) m_maxIterations = 11;
    if (correctAnswers == 12) m_maxIterations = 12;
    if (correctAnswers == 13) m_maxIterations = 13;
    if (correctAnswers == 14) m_maxIterations = 14;
    if (correctAnswers == 15) m_maxIterations = 15;
    if (correctAnswers == 16) m_maxIterations = 16;

    // Создаем прогресс-лейбл
    wxBoxSizer* mainSizer = dynamic_cast<wxBoxSizer*>(GetSizer());
    if (mainSizer) {
        m_progressLabel = new wxStaticText(this, wxID_ANY, "");
        m_progressLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        m_progressLabel->SetForegroundColour(wxColour(255, 255, 255));
        m_progressLabel->SetBackgroundColour(wxColour(0, 0, 0, 180)); // Полупрозрачный черный фон
        mainSizer->Insert(0, m_progressLabel, 0, wxALIGN_CENTER | wxALL, 10);
    }

    // Генерируем базовую снежинку (итерация 0)
    GenerateKochSnowflake();
    UpdateProgressLabel();
}

KochSnowflakePanel::~KochSnowflakePanel()
{
    StopAnimation();
}

void KochSnowflakePanel::StartAnimation()
{
    if (!m_timer && m_currentIteration < m_maxIterations) {
        m_timer = new wxTimer(this, wxID_ANY);
        m_timer->Start(300); // Медленная анимация для лучшего восприятия
    }
}

void KochSnowflakePanel::StopAnimation()
{
    if (m_timer && m_timer->IsRunning()) {
        m_timer->Stop();
    }
    delete m_timer;
    m_timer = nullptr;
}

void KochSnowflakePanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    DrawKochSnowflake(dc);
}

void KochSnowflakePanel::OnTimer(wxTimerEvent& event)
{
    if (m_currentIteration < m_maxIterations) {
        m_currentIteration++;
        GenerateKochSnowflake();
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

void KochSnowflakePanel::DrawKochSnowflake(wxDC& dc)
{
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight() - 100; // Учитываем место для UI

    if (m_snowflakePoints.empty()) {
        // Если точек нет, показываем сообщение
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        wxString message = "Генерация снежинки...";
        wxSize textSize = dc.GetTextExtent(message);
        dc.DrawText(message, width / 2 - textSize.GetWidth() / 2, height / 2 - textSize.GetHeight() / 2);
        return;
    }

    // Устанавливаем цвет в зависимости от прогресса
    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Завершенная снежинка - сияющий голубой
        dc.SetPen(wxPen(wxColour(200, 230, 255), 3));
    }
    else if (m_animationComplete) {
        // Завершенная но не все вопросы - обычный белый
        dc.SetPen(wxPen(wxColour(255, 255, 255), 2));
    }
    else {
        // В процессе построения - голубой
        dc.SetPen(wxPen(wxColour(150, 200, 255), 2));
    }

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    // Рисуем снежинку
    for (size_t i = 0; i < m_snowflakePoints.size() - 1; i++) {
        dc.DrawLine(m_snowflakePoints[i], m_snowflakePoints[i + 1]);
    }

    // Если снежинка завершена и все вопросы отвечены, добавляем декорации
    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Рисуем внутреннюю снежинку с меньшей детализацией
        wxSize size = GetClientSize();
        int centerX = size.GetWidth() / 2;
        int centerY = size.GetHeight() / 2;
        int outerSize = std::min(size.GetWidth(), size.GetHeight()) * 0.25;

        // Создаем внутреннюю снежинку
        std::vector<wxPoint> innerPoints;
        int sides = 6;
        double angleStep = 2.0 * M_PI / sides;

        for (int i = 0; i < sides; i++) {
            double angle = i * angleStep;
            int x = centerX + outerSize * 0.6 * cos(angle);
            int y = centerY + outerSize * 0.6 * sin(angle);
            innerPoints.push_back(wxPoint(x, y));
        }
        innerPoints.push_back(innerPoints[0]); // Замыкаем

        // Рисуем внутреннюю снежинку с меньшей детализацией
        dc.SetPen(wxPen(wxColour(200, 220, 255), 1));
        for (size_t i = 0; i < innerPoints.size() - 1; i++) {
            std::vector<wxPoint> segment;
            KochCurve(segment, innerPoints[i], innerPoints[i + 1], m_maxIterations - 1);

            for (size_t j = 0; j < segment.size() - 1; j++) {
                dc.DrawLine(segment[j], segment[j + 1]);
            }
        }

        // Добавляем блестящие точки
        dc.SetPen(wxPen(wxColour(255, 255, 200)));
        for (int i = 0; i < 8; i++) {
            double angle = (2.0 * M_PI * i) / 8.0;
            int sparkleX = centerX + static_cast<int>(outerSize * 0.8 * cos(angle));
            int sparkleY = centerY + static_cast<int>(outerSize * 0.8 * sin(angle));

            // Рисуем крестик
            dc.DrawLine(sparkleX - 3, sparkleY, sparkleX + 3, sparkleY);
            dc.DrawLine(sparkleX, sparkleY - 3, sparkleX, sparkleY + 3);
        }
    }
}

void KochSnowflakePanel::GenerateKochSnowflake()
{
    m_snowflakePoints.clear();

    wxSize size = GetClientSize();
    int centerX = size.GetWidth() / 2;
    int centerY = size.GetHeight() / 2;
    int snowflakeSize = std::min(size.GetWidth(), size.GetHeight()) * 0.3;

    // Создаем базовый шестиугольник (6 сторон для снежинки)
    int sides = 6;
    double angleStep = 2.0 * M_PI / sides;
    std::vector<wxPoint> basePoints;

    for (int i = 0; i < sides; i++) {
        double angle = i * angleStep;
        int x = centerX + snowflakeSize * cos(angle);
        int y = centerY + snowflakeSize * sin(angle);
        basePoints.push_back(wxPoint(x, y));
    }
    basePoints.push_back(basePoints[0]); // Замыкаем шестиугольник

    // Генерируем снежинку Коха
    for (int i = 0; i < sides; i++) {
        KochCurve(m_snowflakePoints, basePoints[i], basePoints[i + 1], m_currentIteration);
    }
}

void KochSnowflakePanel::KochCurve(std::vector<wxPoint>& points, wxPoint p1, wxPoint p2, int depth)
{
    if (depth == 0) {
        // База рекурсии - просто добавляем отрезок
        points.push_back(p1);
        if (points.empty() || points.back() != p2) {
            points.push_back(p2);
        }
        return;
    }

    // Вычисляем точки для кривой Коха
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    // Три равные части отрезка
    wxPoint a(p1.x + dx / 3.0, p1.y + dy / 3.0);
    wxPoint c(p1.x + 2.0 * dx / 3.0, p1.y + 2.0 * dy / 3.0);

    // Вычисляем точку B (вершина равностороннего треугольника)
    // Поворачиваем вектор AC на 60 градусов
    double angle = M_PI / 3.0; // 60 градусов в радианах

    // Вектор от A к C
    double acx = c.x - a.x;
    double acy = c.y - a.y;

    // Поворачиваем вектор на 60 градусов
    double bx = a.x + acx * cos(angle) - acy * sin(angle);
    double by = a.y + acx * sin(angle) + acy * cos(angle);

    wxPoint b(static_cast<int>(bx), static_cast<int>(by));

    // Рекурсивно генерируем четыре отрезка
    KochCurve(points, p1, a, depth - 1);
    KochCurve(points, a, b, depth - 1);
    KochCurve(points, b, c, depth - 1);
    KochCurve(points, c, p2, depth - 1);
}

wxPoint KochSnowflakePanel::RotatePoint(wxPoint center, wxPoint point, double angle)
{
    // Переносим точку в систему координат с центром в center
    double x = point.x - center.x;
    double y = point.y - center.y;

    // Поворачиваем
    double newX = x * cos(angle) - y * sin(angle);
    double newY = x * sin(angle) + y * cos(angle);

    // Возвращаем в исходную систему координат
    return wxPoint(static_cast<int>(newX + center.x), static_cast<int>(newY + center.y));
}

void KochSnowflakePanel::UpdateProgressLabel()
{
    if (m_progressLabel) {
        int correctAnswers = mainFrame->GetCorrectAnswersCount();
        int totalQuestions = mainFrame->GetTotalQuestions();

        wxString progressText;

        if (m_animationComplete && correctAnswers >= totalQuestions) {
            progressText = wxString::Format("ЗАВЕРШЕНА! Все %d вопросов отвечены правильно!\nИтерация: %d/%d",
                GetFractalName(), totalQuestions, m_currentIteration, m_maxIterations);
        }
        else if (m_animationComplete) {
            progressText = wxString::Format("%s - Построена!\nПравильных ответов: %d/%d\nИтерация: %d/%d",
                GetFractalName(), correctAnswers, totalQuestions,
                m_currentIteration, m_maxIterations);
        }
        else {
            progressText = wxString::Format("%s - Построение...\nПравильных ответов: %d/%d\nИтерация: %d/%d",
                GetFractalName(), correctAnswers, totalQuestions,
                m_currentIteration + 1, m_maxIterations);
        }

        m_progressLabel->SetLabel(progressText);
        m_progressLabel->Wrap(600); // Перенос текста если нужно
    }
}