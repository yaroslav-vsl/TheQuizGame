#pragma once
#include "FractalBase.h"
#include <vector>

class MainFrame;

class KochSnowflakePanel : public FractalBase
{
public:
    KochSnowflakePanel(wxWindow* parent, MainFrame* mainFrame);
    ~KochSnowflakePanel();

    void StartAnimation() override;
    void StopAnimation() override;
    wxString GetFractalName() const override { return "Снежинка Коха"; }

private:
    wxTimer* m_timer;
    wxStaticText* m_progressLabel;

    std::vector<wxPoint> m_snowflakePoints;
    int m_currentIteration;
    int m_maxIterations;
    bool m_animationComplete;

    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void DrawKochSnowflake(wxDC& dc);
    void GenerateKochSnowflake();
    void KochCurve(std::vector<wxPoint>& points, wxPoint p1, wxPoint p2, int depth);
    void UpdateProgressLabel();
    wxPoint RotatePoint(wxPoint center, wxPoint point, double angle);

    wxDECLARE_EVENT_TABLE();
};