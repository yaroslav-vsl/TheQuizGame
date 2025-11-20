#pragma once

#include "FractalBase.h"
#include "MainFrame.h"
#include <vector>
#include <wx/timer.h>

class DragonCurvePanel : public FractalBase
{
public:
    DragonCurvePanel(wxWindow* parent, MainFrame* mainFrame);
    virtual ~DragonCurvePanel();

    void StartAnimation() override;
    void StopAnimation() override;
    wxString GetFractalName() const override { return "Многоголовый Дракон"; }

protected:
    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void DrawDragonCurve(wxDC& dc);
    void GenerateMultiDragon();
    void DragonRecursive(const wxPoint& p1, const wxPoint& p2, int depth, int direction, std::vector<wxPoint>& points);
    void UpdateProgressLabel();
    void CreateDragonHeads();

private:
    int m_currentIteration;
    int m_maxIterations;
    wxTimer* m_timer;
    wxStaticText* m_progressLabel;
    bool m_animationComplete;

    // Множество драконьих хвостов
    std::vector<std::vector<wxPoint>> m_dragonTails;
    std::vector<wxPoint> m_dragonHeads;
    std::vector<double> m_headAngles;

    // Параметры анимации
    double m_animationPhase;

    wxDECLARE_EVENT_TABLE();
};