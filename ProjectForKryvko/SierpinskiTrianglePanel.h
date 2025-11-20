#pragma once

#include "FractalBase.h"
#include "MainFrame.h"
#include <vector>
#include <wx/timer.h>

class SierpinskiTrianglePanel : public FractalBase
{
public:
    SierpinskiTrianglePanel(wxWindow* parent, MainFrame* mainFrame);
    virtual ~SierpinskiTrianglePanel();

    void StartAnimation() override;
    void StopAnimation() override;
    wxString GetFractalName() const override { return "Треугольник Серпинского"; }

protected:
    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void DrawSierpinskiTriangle(wxDC& dc);
    void GenerateSierpinskiTriangle();
    void SierpinskiRecursive(wxDC& dc, wxPoint p1, wxPoint p2, wxPoint p3, int depth);
    void UpdateProgressLabel();

private:
    int m_currentIteration;
    int m_maxIterations;
    wxTimer* m_timer;
    wxStaticText* m_progressLabel;
    bool m_animationComplete;
    std::vector<wxPoint> m_trianglePoints;

    wxDECLARE_EVENT_TABLE();
};