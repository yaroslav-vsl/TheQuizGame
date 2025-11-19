#pragma once
#include "FractalBase.h"
#include <vector>
#include <random>

class MainFrame;

class FernFractalPanel : public FractalBase
{
public:
    FernFractalPanel(wxWindow* parent, MainFrame* mainFrame);
    ~FernFractalPanel();

    void StartAnimation() override;
    void StopAnimation() override;
    wxString GetFractalName() const override { return "Папоротник Барнсли"; }

private:
    wxTimer* m_timer;
    wxStaticText* m_progressLabel;

    std::vector<wxPoint> m_points;
    double m_x, m_y;
    int m_pointCounter;
    int m_maxPoints;

    std::mt19937 m_randomGenerator;
    std::uniform_real_distribution<double> m_randomDistribution;

    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void DrawFern(wxDC& dc);
    void GenerateFernPoints(int count);
    wxPoint TransformPoint(double x, double y, int width, int height);
    void UpdateProgressLabel();

    wxDECLARE_EVENT_TABLE();
};