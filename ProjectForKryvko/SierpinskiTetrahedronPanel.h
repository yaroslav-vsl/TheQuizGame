#pragma once

#include "FractalBase.h"
#include "MainFrame.h"
#include <vector>
#include <wx/timer.h>

struct Point3D {
    double x, y, z;
    Point3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

class SierpinskiTetrahedronPanel : public FractalBase
{
public:
    SierpinskiTetrahedronPanel(wxWindow* parent, MainFrame* mainFrame);
    virtual ~SierpinskiTetrahedronPanel();

    void StartAnimation() override;
    void StopAnimation() override;
    wxString GetFractalName() const override { return "3D Тетраэдр Серпинского"; }

protected:
    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnIdle(wxIdleEvent& event);
    void DrawSierpinskiTetrahedron(wxDC& dc);
    void DrawTetrahedron(wxDC& dc, const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4, int depth);
    void Project3DTo2D(const Point3D& point3D, wxPoint& point2D);
    void UpdateProgressLabel();
    void RotateTetrahedron();

private:
    int m_currentIteration;
    int m_maxIterations;
    wxTimer* m_timer;
    wxStaticText* m_progressLabel;
    bool m_animationComplete;

    // 3D параметры
    double m_rotationX;
    double m_rotationY;
    double m_rotationZ;
    double m_scale;
    Point3D m_center;

    // Вершины базового тетраэдра
    Point3D m_baseVertices[4];

    wxDECLARE_EVENT_TABLE();
};