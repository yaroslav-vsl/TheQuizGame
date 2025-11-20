#include "MandelbrotPanel.h"
#include "MainFrame.h"
#include <wx/dcbuffer.h>
#include <cmath>
#include <algorithm>
#include <complex>

wxBEGIN_EVENT_TABLE(MandelbrotPanel, FractalBase)
EVT_PAINT(MandelbrotPanel::OnPaint)
EVT_TIMER(wxID_ANY, MandelbrotPanel::OnTimer)
EVT_SIZE(MandelbrotPanel::OnSize)
wxEND_EVENT_TABLE()

MandelbrotPanel::MandelbrotPanel(wxWindow* parent, MainFrame* mainFrame)
    : FractalBase(parent, mainFrame),
    m_currentIteration(0),
    m_timer(nullptr),
    m_progressLabel(nullptr),
    m_animationComplete(false),
    m_centerX(-0.5),
    m_centerY(0.0),
    m_scale(3.0),
    m_maxCalculations(100)
{
    // Создаем общий UI
    CreateCommonUI();

    // Определяем максимальное количество итераций на основе правильных ответов
    int correctAnswers = mainFrame->GetCorrectAnswersCount();

    // 8 стадий сложности (увеличиваем детализацию и глубину вычислений)
    m_maxIterations = 0; // Начинаем с 0
    if (correctAnswers >= 2) m_maxIterations = 1;
    if (correctAnswers >= 4) m_maxIterations = 2;
    if (correctAnswers >= 6) m_maxIterations = 3;
    if (correctAnswers >= 8) m_maxIterations = 4;
    if (correctAnswers >= 10) m_maxIterations = 5;
    if (correctAnswers >= 12) m_maxIterations = 6;
    if (correctAnswers >= 14) m_maxIterations = 7;

    // Увеличиваем глубину вычислений с каждой стадией
    m_maxCalculations = 50 + m_maxIterations * 50;

    // Создаем прогресс-лейбл
    wxBoxSizer* mainSizer = dynamic_cast<wxBoxSizer*>(GetSizer());
    if (mainSizer) {
        m_progressLabel = new wxStaticText(this, wxID_ANY, "");
        m_progressLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        m_progressLabel->SetForegroundColour(wxColour(255, 255, 255));
        m_progressLabel->SetBackgroundColour(wxColour(0, 0, 0, 180));
        mainSizer->Insert(0, m_progressLabel, 0, wxALIGN_CENTER | wxALL, 10);
    }

    // Инициализируем буфер с безопасными размерами
    wxSize size = GetClientSize();
    int width = std::max(1, size.GetWidth());
    int height = std::max(1, size.GetHeight() - 100);
    m_bufferSize = wxSize(width, height);

    // Безопасная инициализация буфера
    try {
        m_colorBuffer.resize(height, std::vector<wxColour>(width, wxColour(0, 0, 0)));
    }
    catch (const std::exception& e) {
        // Если не хватает памяти, используем минимальный размер
        m_bufferSize = wxSize(100, 100);
        m_colorBuffer.resize(100, std::vector<wxColour>(100, wxColour(0, 0, 0)));
    }

    GenerateMandelbrot();
    UpdateProgressLabel();
}

MandelbrotPanel::~MandelbrotPanel()
{
    StopAnimation();
}

void MandelbrotPanel::OnSize(wxSizeEvent& event)
{
    // Обновляем размер буфера при изменении размера окна
    wxSize size = GetClientSize();
    int width = std::max(1, size.GetWidth());
    int height = std::max(1, size.GetHeight() - 100);

    if (width != m_bufferSize.GetWidth() || height != m_bufferSize.GetHeight()) {
        m_bufferSize = wxSize(width, height);

        try {
            m_colorBuffer.resize(height, std::vector<wxColour>(width, wxColour(0, 0, 0)));
            GenerateMandelbrot();
        }
        catch (const std::exception& e) {
            // В случае ошибки оставляем старый размер
            wxLogError("Не удалось изменить размер буфера: %s", e.what());
        }
    }

    event.Skip();
}

void MandelbrotPanel::StartAnimation()
{
    if (!m_timer && m_currentIteration < m_maxIterations) {
        m_timer = new wxTimer(this, wxID_ANY);
        m_timer->Start(1000); // Медленная анимация для сложных вычислений
    }
}

void MandelbrotPanel::StopAnimation()
{
    if (m_timer && m_timer->IsRunning()) {
        m_timer->Stop();
    }
    delete m_timer;
    m_timer = nullptr;
}

void MandelbrotPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    DrawMandelbrot(dc);
}

void MandelbrotPanel::OnTimer(wxTimerEvent& event)
{
    if (m_currentIteration < m_maxIterations) {
        m_currentIteration++;

        // Увеличиваем детализацию с каждой итерацией
        m_maxCalculations = 50 + m_currentIteration * 50;

        // Приближаемся к интересным областям
        if (m_currentIteration >= 3) {
            // Масштабируем в интересные области
            double zoomFactor = 0.7;
            m_scale *= zoomFactor;

            // Перемещаемся к интересным точкам поочередно
            if (m_currentIteration == 3) {
                m_centerX = -0.743643887037151;
                m_centerY = 0.131825904205330;
            }
            else if (m_currentIteration == 4) {
                m_centerX = -1.250000000000000;
                m_centerY = 0.000000000000000;
            }
            else if (m_currentIteration == 5) {
                m_centerX = -0.156520166833755;
                m_centerY = -1.032247108922833;
            }
        }

        GenerateMandelbrot();
        UpdateProgressLabel();
        Refresh();

        if (m_currentIteration >= m_maxIterations) {
            m_animationComplete = true;
            StopAnimation();
            UpdateProgressLabel();
        }
    }
}

void MandelbrotPanel::DrawMandelbrot(wxDC& dc)
{
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight() - 100;

    if (m_colorBuffer.empty() || m_colorBuffer[0].empty() || width <= 0 || height <= 0) {
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        wxString message = "Вычисление множества Мандельброта...";
        wxSize textSize = dc.GetTextExtent(message);
        dc.DrawText(message, width / 2 - textSize.GetWidth() / 2, height / 2 - textSize.GetHeight() / 2);
        return;
    }

    // Рисуем вычисленные пиксели
    int drawHeight = std::min(height, m_bufferSize.GetHeight());
    int drawWidth = std::min(width, m_bufferSize.GetWidth());

    for (int y = 0; y < drawHeight; y++) {
        for (int x = 0; x < drawWidth; x++) {
            dc.SetPen(wxPen(m_colorBuffer[y][x], 1));
            dc.DrawPoint(x, y);
        }
    }

    // Декорации для завершенного фрактала
    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Добавляем информационную панель
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        wxString info = wxString::Format("Центр: (%.6f, %.6f)  Масштаб: %.2e",
            m_centerX, m_centerY, m_scale);
        wxSize textSize = dc.GetTextExtent(info);
        dc.DrawText(info, 10, height - 30);

        // Рамка вокруг фрактала
        dc.SetPen(wxPen(wxColour(255, 255, 255, 128), 2));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(0, 0, width, height);
    }
}

void MandelbrotPanel::GenerateMandelbrot()
{
    wxSize size = GetClientSize();
    int width = std::max(1, size.GetWidth());
    int height = std::max(1, size.GetHeight() - 100);

    // Обновляем размер буфера если нужно
    if (m_bufferSize.GetWidth() != width || m_bufferSize.GetHeight() != height) {
        m_bufferSize = wxSize(width, height);
        try {
            m_colorBuffer.resize(height, std::vector<wxColour>(width, wxColour(0, 0, 0)));
        }
        catch (const std::exception& e) {
            wxLogError("Ошибка выделения памяти для буфера: %s", e.what());
            return;
        }
    }

    // Вычисляем Мандельброта для каждого пикселя
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Преобразуем координаты пикселя в координаты комплексной плоскости
            double x0 = (x - width / 2.0) * (m_scale / width) + m_centerX;
            double y0 = (y - height / 2.0) * (m_scale / height) + m_centerY;

            int iterations = CalculateMandelbrot(x0, y0, m_maxCalculations);
            m_colorBuffer[y][x] = GetColor(iterations, m_maxCalculations);
        }
    }
}

int MandelbrotPanel::CalculateMandelbrot(double x0, double y0, int maxIterations)
{
    double x = 0.0;
    double y = 0.0;
    double x2 = 0.0;
    double y2 = 0.0;

    int iteration = 0;

    // Оптимизированный алгоритм без использования std::complex
    while (x2 + y2 <= 4.0 && iteration < maxIterations) {
        y = 2 * x * y + y0;
        x = x2 - y2 + x0;
        x2 = x * x;
        y2 = y * y;
        iteration++;
    }

    return iteration;
}

wxColour MandelbrotPanel::GetColor(int iterations, int maxIterations)
{
    if (iterations == maxIterations) {
        // Точка принадлежит множеству Мандельброта - черный цвет
        return wxColour(0, 0, 0);
    }

    // Градиент цвета в зависимости от количества итераций
    double progress = static_cast<double>(iterations) / maxIterations;

    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Радужная палитра для завершенного фрактала
        int r = static_cast<int>(128 + 127 * sin(progress * 2 * M_PI + 0));
        int g = static_cast<int>(128 + 127 * sin(progress * 2 * M_PI + 2 * M_PI / 3));
        int b = static_cast<int>(128 + 127 * sin(progress * 2 * M_PI + 4 * M_PI / 3));
        return wxColour(r, g, b);
    }
    else if (m_animationComplete) {
        // Сине-зеленая палитра
        int r = static_cast<int>(64 * progress);
        int g = static_cast<int>(128 + 127 * progress);
        int b = static_cast<int>(192 + 63 * progress);
        return wxColour(r, g, b);
    }
    else {
        // Красно-оранжевая палитра во время построения
        int r = static_cast<int>(128 + 127 * progress);
        int g = static_cast<int>(64 * progress);
        int b = static_cast<int>(32 * progress);
        return wxColour(r, g, b);
    }
}

void MandelbrotPanel::UpdateProgressLabel()
{
    if (m_progressLabel) {
        int correctAnswers = mainFrame->GetCorrectAnswersCount();
        int totalQuestions = mainFrame->GetTotalQuestions();

        wxString progressText;

        if (m_animationComplete && correctAnswers >= totalQuestions) {
            progressText = wxString::Format("МНОЖЕСТВО МАНДЕЛЬБРОТА - ЗАВЕРШЕНО!\n"
                "Все %d вопросов отвечены правильно!\n"
                "Детализация: %d/%d\n"
                "Итераций вычисления: %d\n"
                "Масштаб: %.2e",
                totalQuestions, m_currentIteration, m_maxIterations,
                m_maxCalculations, m_scale);
        }
        else if (m_animationComplete) {
            progressText = wxString::Format("Множество Мандельброта - Построено!\n"
                "Правильных ответов: %d/%d\n"
                "Детализация: %d/%d\n"
                "Итераций вычисления: %d",
                correctAnswers, totalQuestions,
                m_currentIteration, m_maxIterations,
                m_maxCalculations);
        }
        else {
            progressText = wxString::Format("Множество Мандельброта - Вычисление...\n"
                "Правильных ответов: %d/%d\n"
                "Детализация: %d/%d\n"
                "Итераций вычисления: %d",
                correctAnswers, totalQuestions,
                m_currentIteration + 1, m_maxIterations,
                m_maxCalculations);
        }

        m_progressLabel->SetLabel(progressText);
        m_progressLabel->Wrap(600);
    }
}