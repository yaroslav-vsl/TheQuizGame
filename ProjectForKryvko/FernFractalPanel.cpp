#include "FernFractalPanel.h"
#include "MainFrame.h"
#include <wx/dcbuffer.h>
#include <cmath>

wxBEGIN_EVENT_TABLE(FernFractalPanel, FractalBase)
EVT_PAINT(FernFractalPanel::OnPaint)
EVT_TIMER(wxID_ANY, FernFractalPanel::OnTimer)
wxEND_EVENT_TABLE()

FernFractalPanel::FernFractalPanel(wxWindow* parent, MainFrame* mainFrame)
    : FractalBase(parent, mainFrame),
    m_x(0.0), m_y(0.0),
    m_pointCounter(0),
    m_timer(nullptr),
    m_progressLabel(nullptr)
{
    // Создаем общий UI
    CreateCommonUI();

    // Определяем количество точек на основе правильных ответов
    int correctAnswers = mainFrame->GetCorrectAnswersCount();
    int totalQuestions = mainFrame->GetTotalQuestions();

    // Устанавливаем максимальное количество точек в зависимости от прогресса
    m_maxPoints = 1000; // Базовое количество
    if (correctAnswers > 0) m_maxPoints = 2000;
    if (correctAnswers >= totalQuestions * 0.25) m_maxPoints = 5000;
    if (correctAnswers >= totalQuestions * 0.5) m_maxPoints = 10000;
    if (correctAnswers >= totalQuestions * 0.75) m_maxPoints = 20000;
    if (correctAnswers >= totalQuestions) m_maxPoints = 50000;

    // Инициализируем генератор случайных чисел
    std::random_device rd;
    m_randomGenerator = std::mt19937(rd());
    m_randomDistribution = std::uniform_real_distribution<double>(0.0, 1.0);

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

    // Начинаем с нескольких точек для немедленного отображения
    GenerateFernPoints(100);
}

FernFractalPanel::~FernFractalPanel()
{
    StopAnimation();
}

void FernFractalPanel::StartAnimation()
{
    if (!m_timer) {
        m_timer = new wxTimer(this, wxID_ANY);
        m_timer->Start(30); // Быстрая анимация - 30 мс
    }
}

void FernFractalPanel::StopAnimation()
{
    if (m_timer && m_timer->IsRunning()) {
        m_timer->Stop();
    }
    delete m_timer;
    m_timer = nullptr;
}

void FernFractalPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    DrawFern(dc);
}

void FernFractalPanel::OnTimer(wxTimerEvent& event)
{
    // Генерируем новые точки
    int pointsToGenerate = m_maxPoints / 50; // 2% от общего количества за один раз
    if (pointsToGenerate < 50) pointsToGenerate = 50;
    if (pointsToGenerate > 500) pointsToGenerate = 500;

    if (m_pointCounter < m_maxPoints) {
        GenerateFernPoints(pointsToGenerate);
        UpdateProgressLabel();
        Refresh();
    }
    else {
        // Анимация завершена
        StopAnimation();

        // Если все вопросы отвечены, показываем специальное сообщение
        if (mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
            UpdateProgressLabel(); // Обновляем для показа завершающего сообщения
        }
    }
}

void FernFractalPanel::DrawFern(wxDC& dc)
{
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight() - 100; // Учитываем место для UI

    if (m_points.empty()) {
        // Если точек нет, показываем сообщение
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        wxString message = "Генерация папоротника...";
        wxSize textSize = dc.GetTextExtent(message);
        dc.DrawText(message, width / 2 - textSize.GetWidth() / 2, height / 2 - textSize.GetHeight() / 2);
        return;
    }

    // Рисуем все точки папоротника
    for (const wxPoint& point : m_points) {
        // Вычисляем цвет на основе положения точки (создает градиент)
        double relativeY = static_cast<double>(point.y) / height;
        int greenValue = static_cast<int>(100 + relativeY * 155);
        if (greenValue > 255) greenValue = 255;

        // Определяем цвет в зависимости от части папоротника
        if (relativeY > 0.85) {
            // Нижняя часть - стебель (коричневый)
            dc.SetPen(wxPen(wxColour(139, 69, 19))); // Коричневый
        }
        else if (relativeY > 0.7) {
            // Средняя часть - переходный цвет
            int transition = static_cast<int>((relativeY - 0.7) / 0.15 * 255);
            dc.SetPen(wxPen(wxColour(139 - transition, 69 + transition, 19)));
        }
        else {
            // Верхняя часть - листья (зеленый с градиентом)
            dc.SetPen(wxPen(wxColour(0, greenValue, 0)));
        }

        dc.DrawPoint(point);
    }

    // Если папоротник полностью вырос и все вопросы отвечены, добавляем декорации
    if (m_pointCounter >= m_maxPoints && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Рисуем рамку вокруг папоротника
        dc.SetPen(wxPen(wxColour(255, 255, 100), 2));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);

        int padding = 20;
        int fernWidth = static_cast<int>(width * 0.6);
        int fernHeight = static_cast<int>(height * 0.8);
        int fernX = width / 2 - fernWidth / 2;
        int fernY = height / 2 - fernHeight / 2;

        dc.DrawRectangle(fernX - padding, fernY - padding, fernWidth + 2 * padding, fernHeight + 2 * padding);

        // Добавляем блестящие точки
        dc.SetPen(wxPen(wxColour(255, 255, 200)));
        for (int i = 0; i < 10; i++) {
            int sparkleX = fernX + rand() % fernWidth;
            int sparkleY = fernY + rand() % fernHeight;
            dc.DrawLine(sparkleX - 2, sparkleY, sparkleX + 2, sparkleY);
            dc.DrawLine(sparkleX, sparkleY - 2, sparkleX, sparkleY + 2);
        }
    }
}

void FernFractalPanel::GenerateFernPoints(int count)
{
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight() - 100;

    for (int i = 0; i < count && m_pointCounter < m_maxPoints; i++) {
        double r = m_randomDistribution(m_randomGenerator);
        double nx, ny;

        // Применяем одно из четырех преобразований для папоротника Барнсли
        if (r < 0.01) {
            // Преобразование 1: стебель (1% вероятность)
            nx = 0.0;
            ny = 0.16 * m_y;
        }
        else if (r < 0.86) {
            // Преобразование 2: последовательные маленькие листочки (85% вероятность)
            nx = 0.85 * m_x + 0.04 * m_y;
            ny = -0.04 * m_x + 0.85 * m_y + 1.6;
        }
        else if (r < 0.93) {
            // Преобразование 3: левый лист (7% вероятность)
            nx = 0.2 * m_x - 0.26 * m_y;
            ny = 0.23 * m_x + 0.22 * m_y + 1.6;
        }
        else {
            // Преобразование 4: правый лист (7% вероятность)
            nx = -0.15 * m_x + 0.28 * m_y;
            ny = 0.26 * m_x + 0.24 * m_y + 0.44;
        }

        m_x = nx;
        m_y = ny;

        // Преобразуем координаты фрактала в координаты экрана
        wxPoint screenPoint = TransformPoint(m_x, m_y, width, height);
        m_points.push_back(screenPoint);

        m_pointCounter++;
    }
}

wxPoint FernFractalPanel::TransformPoint(double x, double y, int width, int height)
{
    // Масштабируем и сдвигаем координаты для отображения в центре экрана
    // Папоротник Барнсли имеет естественные границы примерно:
    // x: -2.5 до 2.5, y: 0 до 10

    double scaleX = width / 6.0;  // Для охвата по X от -3 до 3
    double scaleY = height / 11.0; // Для охвата по Y от 0 до 11

    // Центрируем папоротник
    int screenX = width / 2 + static_cast<int>(x * scaleX);
    int screenY = height - static_cast<int>(y * scaleY); // Переворачиваем Y (экранные координаты)

    // Добавляем небольшой вертикальный сдвиг для лучшего позиционирования
    screenY -= static_cast<int>(height * 0.1);

    return wxPoint(screenX, screenY);
}

void FernFractalPanel::UpdateProgressLabel()
{
    if (m_progressLabel) {
        int correctAnswers = mainFrame->GetCorrectAnswersCount();
        int totalQuestions = mainFrame->GetTotalQuestions();

        wxString progressText;

        if (m_pointCounter >= m_maxPoints && correctAnswers >= totalQuestions) {
            progressText = wxString::Format("ЗАВЕРШЕН! Все %d вопросов отвечены правильно!\nПапоротник содержит %d точек",
                GetFractalName(), totalQuestions, m_pointCounter);
        }
        else if (m_pointCounter >= m_maxPoints) {
            progressText = wxString::Format("%s - Построен!\nПравильных ответов: %d/%d\nПапоротник содержит %d точек",
                GetFractalName(), correctAnswers, totalQuestions, m_pointCounter);
        }
        else {
            int progressPercent = static_cast<int>((static_cast<double>(m_pointCounter) / m_maxPoints) * 100);
            progressText = wxString::Format("%s - Построение: %d%%\nПравильных ответов: %d/%d\nТочки: %d/%d",
                GetFractalName(), progressPercent, correctAnswers, totalQuestions,
                m_pointCounter, m_maxPoints);
        }

        m_progressLabel->SetLabel(progressText);
        m_progressLabel->Wrap(600); // Перенос текста если нужно
    }
}