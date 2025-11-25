#include "DragonCurvePanel.h"
#include "MainFrame.h"
#include <wx/dcbuffer.h>
#include <cmath>
#include <algorithm>
#include <random>

wxBEGIN_EVENT_TABLE(DragonCurvePanel, FractalBase)
EVT_PAINT(DragonCurvePanel::OnPaint)
EVT_TIMER(wxID_ANY, DragonCurvePanel::OnTimer)
wxEND_EVENT_TABLE()

DragonCurvePanel::DragonCurvePanel(wxWindow* parent, MainFrame* mainFrame)
    : FractalBase(parent, mainFrame),
    m_currentIteration(0),
    m_timer(nullptr),
    m_progressLabel(nullptr),
    m_animationComplete(false),
    m_animationPhase(0.0)
{
    // Создаем общий UI
    CreateCommonUI();

    // Определяем максимальное количество итераций на основе правильных ответов
    int correctAnswers = mainFrame->GetCorrectAnswersCount();

    // 8 стадий сложности (от 0 до 7 итераций)
    m_maxIterations = 0; // Начинаем с 0
    if (correctAnswers >= 1) m_maxIterations = 1;
    if (correctAnswers >= 2) m_maxIterations = 2;
    if (correctAnswers >= 4) m_maxIterations = 3;
    if (correctAnswers >= 6) m_maxIterations = 4;
    if (correctAnswers >= 8) m_maxIterations = 5;
    if (correctAnswers >= 10) m_maxIterations = 6;
    if (correctAnswers >= 12) m_maxIterations = 7;
    if (correctAnswers >= 14) m_maxIterations = 8;
    if (correctAnswers >= 16) m_maxIterations = 9;

    // Создаем прогресс-лейбл
    wxBoxSizer* mainSizer = dynamic_cast<wxBoxSizer*>(GetSizer());
    if (mainSizer) {
        m_progressLabel = new wxStaticText(this, wxID_ANY, "");
        m_progressLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        m_progressLabel->SetForegroundColour(wxColour(255, 255, 255));
        m_progressLabel->SetBackgroundColour(wxColour(0, 0, 0, 180));
        mainSizer->Insert(0, m_progressLabel, 0, wxALIGN_CENTER | wxALL, 10);
    }

    // Создаем головы драконов

    // Генерируем базовые кривые
    GenerateMultiDragon();
    UpdateProgressLabel();
}

DragonCurvePanel::~DragonCurvePanel()
{
    StopAnimation();
}

void DragonCurvePanel::StartAnimation()
{
    if (!m_timer && m_currentIteration < m_maxIterations) {
        m_timer = new wxTimer(this, wxID_ANY);
        m_timer->Start(300); // Анимация для плавного движения
    }
}

void DragonCurvePanel::StopAnimation()
{
    if (m_timer && m_timer->IsRunning()) {
        m_timer->Stop();
    }
    delete m_timer;
    m_timer = nullptr;
}

void DragonCurvePanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    DrawDragonCurve(dc);
}

void DragonCurvePanel::OnTimer(wxTimerEvent& event)
{
   if (m_currentIteration < m_maxIterations) {
        m_currentIteration++;

        // Обновляем анимацию
        m_animationPhase += 0.1;
        if (m_animationPhase > 2 * M_PI) {
            m_animationPhase -= 2 * M_PI;
        }

        // Создаем новые головы для следующей итерации
        GenerateMultiDragon();
        UpdateProgressLabel();
        Refresh();

        if (m_currentIteration >= m_maxIterations) {
            m_animationComplete = true;
            StopAnimation();
            UpdateProgressLabel();
        }
    }
}

void DragonCurvePanel::DrawDragonCurve(wxDC& dc)
{
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight() - 100;

    if (m_dragonTails.empty()) {
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        wxString message = "Генерация многоголового дракона...";
        wxSize textSize = dc.GetTextExtent(message);
        dc.DrawText(message, width / 2 - textSize.GetWidth() / 2, height / 2 - textSize.GetHeight() / 2);
        return;
    }

    // Рисуем все хвосты дракона
    for (size_t tailIndex = 0; tailIndex < m_dragonTails.size(); tailIndex++) {
        const auto& tail = m_dragonTails[tailIndex];

        if (tail.size() < 2) continue;

        // Разные цвета для разных хвостов
        double colorPhase = static_cast<double>(tailIndex) / m_dragonTails.size();

        for (size_t i = 0; i < tail.size() - 1; i++) {
            double progress = static_cast<double>(i) / (tail.size() - 1);

            if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
                // Радужный градиент для завершенного дракона
                int r = static_cast<int>(128 + 127 * sin(progress * 4 * M_PI + colorPhase * 2 * M_PI + m_animationPhase));
                int g = static_cast<int>(128 + 127 * sin(progress * 4 * M_PI + colorPhase * 2 * M_PI + 2 * M_PI / 3 + m_animationPhase));
                int b = static_cast<int>(128 + 127 * sin(progress * 4 * M_PI + colorPhase * 2 * M_PI + 4 * M_PI / 3 + m_animationPhase));
                dc.SetPen(wxPen(wxColour(r, g, b), 2));
            }
            else if (m_animationComplete) {
                // Градиент от зеленого к синему
                int r = static_cast<int>(64 * sin(progress * 2 * M_PI + colorPhase));
                int g = static_cast<int>(128 + 127 * (1 - progress));
                int b = static_cast<int>(128 + 127 * progress);
                dc.SetPen(wxPen(wxColour(r, g, b), 2));
            }
            else {
                // Градиент от красного к оранжевому во время построения
                int r = 200 + static_cast<int>(55 * sin(progress * 2 * M_PI + m_animationPhase));
                int g = static_cast<int>(100 * progress);
                int b = static_cast<int>(50 * progress);
                dc.SetPen(wxPen(wxColour(r, g, b), 2));
            }

            dc.DrawLine(tail[i], tail[i + 1]);
        }
    }
}

void DragonCurvePanel::GenerateMultiDragon()
{
    m_dragonTails.clear();

    wxSize size = GetClientSize();
    int centerX = size.GetWidth() / 2;
    int centerY = (size.GetHeight() + 100) / 2;

    double scale = 0.75;

    if (size.GetWidth() > 2000)
    {
        scale = 2;
    }

    // Создаем несколько драконьих хвостов, исходящих из центра
    int numTails = 3 + m_currentIteration; // Количество хвостов увеличивается с итерациями

    for (int i = 0; i < numTails; i++) {
        double angle = 2.0 * M_PI * i / numTails + m_animationPhase * 0.1;
        int tailLength = 100 * scale + m_currentIteration * 30 * scale;

        wxPoint start(centerX, centerY);
        wxPoint end(
            centerX + static_cast<int>(tailLength * cos(angle)),
            centerY + static_cast<int>(tailLength * sin(angle))
        );

        std::vector<wxPoint> tailPoints;
        tailPoints.push_back(start);
        DragonRecursive(start, end, m_currentIteration, (i % 2 == 0) ? 1 : -1, tailPoints);
        tailPoints.push_back(end);

        m_dragonTails.push_back(tailPoints);
    }
}

void DragonCurvePanel::DragonRecursive(const wxPoint& p1, const wxPoint& p2, int depth, int direction, std::vector<wxPoint>& points)
{
    if (depth <= 0) {
        return;
    }

    // Вычисляем среднюю точку и смещаем ее для создания изгиба
    wxPoint mid((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

    // Вектор от p1 к p2
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    // Поворачиваем вектор на 90 градусов в зависимости от направления
    double newX, newY;
    if (direction > 0) {
        newX = -dy;
        newY = dx;
    }
    else {
        newX = dy;
        newY = -dx;
    }

    // Нормализуем и масштабируем вектор
    double length = sqrt(newX * newX + newY * newY);
    if (length > 0) {
        newX = newX / length;
        newY = newY / length;
    }

    // Смещаем среднюю точку перпендикулярно исходному отрезку
    double distance = sqrt(dx * dx + dy * dy) / 2;
    double curveFactor = 0.5 + 0.2 * sin(m_animationPhase + depth); // Динамический изгиб

    wxPoint bendPoint(
        static_cast<int>(mid.x + newX * distance * curveFactor),
        static_cast<int>(mid.y + newY * distance * curveFactor)
    );

    // Рекурсивно генерируем две половинки
    DragonRecursive(p1, bendPoint, depth - 1, 1, points);
    points.push_back(bendPoint);
    DragonRecursive(bendPoint, p2, depth - 1, -1, points);
}

void DragonCurvePanel::UpdateProgressLabel()
{
    if (m_progressLabel) {
        int correctAnswers = mainFrame->GetCorrectAnswersCount();
        int totalQuestions = mainFrame->GetTotalQuestions();

        wxString progressText;

        if (m_animationComplete && correctAnswers >= totalQuestions) {
            progressText = wxString::Format("МНОГОГОЛОВЫЙ ДРАКОН - ЗАВЕРШЕН!\n"
                "Все %d вопросов отвечены правильно!\n"
                "Итерация: %d/%d\n",
                totalQuestions, m_currentIteration, m_maxIterations);
        }
        else if (m_animationComplete) {
            progressText = wxString::Format("Многоголовый Дракон - Построен!\n"
                "Правильных ответов: %d/%d\n"
                "Итерация: %d/%d\n",
                correctAnswers, totalQuestions,
                m_currentIteration, m_maxIterations);
        }
        else {
            progressText = wxString::Format("Многоголовый Дракон - Построение...\n"
                "Правильных ответов: %d/%d\n"
                "Итерация: %d/%d\n",
                correctAnswers, totalQuestions,
                m_currentIteration + 1, m_maxIterations);
        }

        m_progressLabel->SetLabel(progressText);
        m_progressLabel->Wrap(600);
    }
}