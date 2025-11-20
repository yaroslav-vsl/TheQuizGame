#include "SierpinskiTetrahedronPanel.h"
#include "MainFrame.h"
#include <wx/dcbuffer.h>
#include <cmath>
#include <algorithm>

wxBEGIN_EVENT_TABLE(SierpinskiTetrahedronPanel, FractalBase)
EVT_PAINT(SierpinskiTetrahedronPanel::OnPaint)
EVT_TIMER(wxID_ANY, SierpinskiTetrahedronPanel::OnTimer)
EVT_IDLE(SierpinskiTetrahedronPanel::OnIdle)
wxEND_EVENT_TABLE()

SierpinskiTetrahedronPanel::SierpinskiTetrahedronPanel(wxWindow* parent, MainFrame* mainFrame)
    : FractalBase(parent, mainFrame),
    m_currentIteration(0),
    m_timer(nullptr),
    m_progressLabel(nullptr),
    m_animationComplete(false),
    m_rotationX(0.0),
    m_rotationY(0.0),
    m_rotationZ(0.0),
    m_scale(1.0)
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

    // Инициализируем вершины базового тетраэдра
    double size = 0.8;
    m_baseVertices[0] = Point3D(0.0, -size, 0.0);  // Верхняя вершина
    m_baseVertices[1] = Point3D(-size, size, -size); // Левая нижняя
    m_baseVertices[2] = Point3D(size, size, -size);  // Правая нижняя
    m_baseVertices[3] = Point3D(0.0, size, size);   // Задняя нижняя

    m_center = Point3D(0.0, 0.0, 0.0);

    // Создаем прогресс-лейбл
    wxBoxSizer* mainSizer = dynamic_cast<wxBoxSizer*>(GetSizer());
    if (mainSizer) {
        m_progressLabel = new wxStaticText(this, wxID_ANY, "");
        m_progressLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        m_progressLabel->SetForegroundColour(wxColour(255, 255, 255));
        m_progressLabel->SetBackgroundColour(wxColour(0, 0, 0, 180));
        mainSizer->Insert(0, m_progressLabel, 0, wxALIGN_CENTER | wxALL, 10);
    }

    UpdateProgressLabel();
}

SierpinskiTetrahedronPanel::~SierpinskiTetrahedronPanel()
{
    StopAnimation();
}

void SierpinskiTetrahedronPanel::StartAnimation()
{
    if (!m_timer && m_currentIteration < m_maxIterations) {
        m_timer = new wxTimer(this, wxID_ANY);
        m_timer->Start(800); // Анимация с интервалом 800 мс для 3D
    }
}

void SierpinskiTetrahedronPanel::StopAnimation()
{
    if (m_timer && m_timer->IsRunning()) {
        m_timer->Stop();
    }
    delete m_timer;
    m_timer = nullptr;
}

void SierpinskiTetrahedronPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    DrawSierpinskiTetrahedron(dc);
}

void SierpinskiTetrahedronPanel::OnTimer(wxTimerEvent& event)
{
    if (m_currentIteration < m_maxIterations) {
        m_currentIteration++;
        UpdateProgressLabel();
        Refresh();

        if (m_currentIteration >= m_maxIterations) {
            m_animationComplete = true;
            StopAnimation();
            UpdateProgressLabel();
        }
    }
}

void SierpinskiTetrahedronPanel::OnIdle(wxIdleEvent& event)
{
    if (m_animationComplete) {
        RotateTetrahedron();
        Refresh();
    }
    event.RequestMore(); // Продолжаем получать события idle
}

void SierpinskiTetrahedronPanel::RotateTetrahedron()
{
    // Медленное вращение для демонстрации 3D
    m_rotationY += 0.01;
    m_rotationX += 0.005;
}

void SierpinskiTetrahedronPanel::DrawSierpinskiTetrahedron(wxDC& dc)
{
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight() - 100;

    if (m_currentIteration == 0 && !m_animationComplete) {
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        wxString message = "Генерация 3D тетраэдра...";
        wxSize textSize = dc.GetTextExtent(message);
        dc.DrawText(message, width / 2 - textSize.GetWidth() / 2, height / 2 - textSize.GetHeight() / 2);
        return;
    }

    // Устанавливаем цвет в зависимости от прогресса
    wxColour lineColor, fillColor;
    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        lineColor = wxColour(255, 215, 0); // Золотой
        fillColor = wxColour(255, 215, 0, 80);
    }
    else if (m_animationComplete) {
        lineColor = wxColour(50, 205, 50); // Зеленый
        fillColor = wxColour(50, 205, 50, 60);
    }
    else {
        lineColor = wxColour(30, 144, 255); // Синий
        fillColor = wxColour(30, 144, 255, 40);
    }

    dc.SetPen(wxPen(lineColor, 2));
    dc.SetBrush(wxBrush(fillColor));

    // Рисуем 3D тетраэдр Серпинского
    DrawTetrahedron(dc, m_baseVertices[0], m_baseVertices[1], m_baseVertices[2], m_baseVertices[3], m_currentIteration);

    // Декорации для завершенного фрактала
    if (m_animationComplete && mainFrame->GetCorrectAnswersCount() >= mainFrame->GetTotalQuestions()) {
        // Рисуем оси координат для демонстрации 3D
        Point3D origin(0, 0, 0);
        Point3D xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1);

        wxPoint o, x, y, z;
        Project3DTo2D(origin, o);
        Project3DTo2D(xAxis, x);
        Project3DTo2D(yAxis, y);
        Project3DTo2D(zAxis, z);

        dc.SetPen(wxPen(wxColour(255, 0, 0), 2));   // X - красный
        dc.DrawLine(o, x);
        dc.SetPen(wxPen(wxColour(0, 255, 0), 2));   // Y - зеленый
        dc.DrawLine(o, y);
        dc.SetPen(wxPen(wxColour(0, 0, 255), 2));   // Z - синий
        dc.DrawLine(o, z);
    }
}

void SierpinskiTetrahedronPanel::DrawTetrahedron(wxDC& dc, const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4, int depth)
{
    if (depth == 0) {
        // Рисуем тетраэдр - 4 треугольные грани
        wxPoint points2D[4];
        Project3DTo2D(p1, points2D[0]);
        Project3DTo2D(p2, points2D[1]);
        Project3DTo2D(p3, points2D[2]);
        Project3DTo2D(p4, points2D[3]);

        // Рисуем грани тетраэдра (только видимые)
        wxPoint face1[3] = { points2D[0], points2D[1], points2D[2] };
        wxPoint face2[3] = { points2D[0], points2D[1], points2D[3] };
        wxPoint face3[3] = { points2D[0], points2D[2], points2D[3] };
        wxPoint face4[3] = { points2D[1], points2D[2], points2D[3] };

        // Простая проверка видимости (можно улучшить)
        dc.DrawPolygon(3, face1);
        dc.DrawPolygon(3, face2);
        dc.DrawPolygon(3, face3);
        dc.DrawPolygon(3, face4);

        return;
    }

    // Вычисляем середины ребер для создания 4 меньших тетраэдров
    Point3D mid12((p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2);
    Point3D mid13((p1.x + p3.x) / 2, (p1.y + p3.y) / 2, (p1.z + p3.z) / 2);
    Point3D mid14((p1.x + p4.x) / 2, (p1.y + p4.y) / 2, (p1.z + p4.z) / 2);
    Point3D mid23((p2.x + p3.x) / 2, (p2.y + p3.y) / 2, (p2.z + p3.z) / 2);
    Point3D mid24((p2.x + p4.x) / 2, (p2.y + p4.y) / 2, (p2.z + p4.z) / 2);
    Point3D mid34((p3.x + p4.x) / 2, (p3.y + p4.y) / 2, (p3.z + p4.z) / 2);

    // Рекурсивно рисуем 4 меньших тетраэдра (центральный пропускаем - это и есть фрактал)
    DrawTetrahedron(dc, p1, mid12, mid13, mid14, depth - 1);
    DrawTetrahedron(dc, mid12, p2, mid23, mid24, depth - 1);
    DrawTetrahedron(dc, mid13, mid23, p3, mid34, depth - 1);
    DrawTetrahedron(dc, mid14, mid24, mid34, p4, depth - 1);
}

void SierpinskiTetrahedronPanel::Project3DTo2D(const Point3D& point3D, wxPoint& point2D)
{
    wxSize size = GetClientSize();
    int centerX = size.GetWidth() / 2;
    int centerY = (size.GetHeight() - 100) / 2;

    // Применяем вращения
    double x = point3D.x;
    double y = point3D.y;
    double z = point3D.z;

    // Вращение вокруг Y
    double cosY = cos(m_rotationY);
    double sinY = sin(m_rotationY);
    double x1 = x * cosY - z * sinY;
    double z1 = x * sinY + z * cosY;

    // Вращение вокруг X
    double cosX = cos(m_rotationX);
    double sinX = sin(m_rotationX);
    double y1 = y * cosX - z1 * sinX;
    double z2 = y * sinX + z1 * cosX;

    // Вращение вокруг Z
    double cosZ = cos(m_rotationZ);
    double sinZ = sin(m_rotationZ);
    double x2 = x1 * cosZ - y1 * sinZ;
    double y2 = x1 * sinZ + y1 * cosZ;

    // Простая перспективная проекция
    double scale = 200.0 * m_scale;
    double perspective = 2.0; // Коэффициент перспективы

    point2D.x = centerX + static_cast<int>(x2 * scale / (z2 + perspective));
    point2D.y = centerY + static_cast<int>(y2 * scale / (z2 + perspective));
}

void SierpinskiTetrahedronPanel::UpdateProgressLabel()
{
    if (m_progressLabel) {
        int correctAnswers = mainFrame->GetCorrectAnswersCount();
        int totalQuestions = mainFrame->GetTotalQuestions();

        wxString progressText;

        if (m_animationComplete && correctAnswers >= totalQuestions) {
            progressText = wxString::Format("3D ТЕТРАЭДР СЕРПИНСКОГО - ЗАВЕРШЕН!\n"
                "Все %d вопросов отвечены правильно!\n"
                "Итерация: %d/%d\n"
                "Фрактал вращается в 3D пространстве",
                totalQuestions, m_currentIteration, m_maxIterations);
        }
        else if (m_animationComplete) {
            progressText = wxString::Format("3D Тетраэдр Серпинского - Построен!\n"
                "Правильных ответов: %d/%d\n"
                "Итерация: %d/%d\n"
                "Фрактал вращается в 3D пространстве",
                correctAnswers, totalQuestions,
                m_currentIteration, m_maxIterations);
        }
        else {
            progressText = wxString::Format("3D Тетраэдр Серпинского - Построение...\n"
                "Правильных ответов: %d/%d\n"
                "Итерация: %d/%d",
                correctAnswers, totalQuestions,
                m_currentIteration, m_maxIterations);
        }

        m_progressLabel->SetLabel(progressText);
        m_progressLabel->Wrap(600);
    }
}