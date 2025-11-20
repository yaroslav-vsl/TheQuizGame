#pragma once

#include "FractalBase.h"
#include "MainFrame.h"
#include <vector>
#include <wx/timer.h>
#include <wx/thread.h>

class MandelbrotPanel : public FractalBase
{
public:
    MandelbrotPanel(wxWindow* parent, MainFrame* mainFrame);
    virtual ~MandelbrotPanel();

    void StartAnimation() override;
    void StopAnimation() override;
    wxString GetFractalName() const override { return "Множество Мандельброта"; }

protected:
    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);
    void DrawMandelbrot(wxDC& dc);
    void GenerateMandelbrot();
    int CalculateMandelbrot(double x0, double y0, int maxIterations);
    wxColour GetColor(int iterations, int maxIterations);
    void UpdateProgressLabel();
    void ZoomToPoint(int x, int y, double zoomFactor);

private:
    int m_currentIteration;
    int m_maxIterations;
    wxTimer* m_timer;
    wxStaticText* m_progressLabel;
    bool m_animationComplete;

    // Параметры Мандельброта
    double m_centerX;
    double m_centerY;
    double m_scale;
    int m_maxCalculations;

    // Буфер для хранения вычисленных цветов
    std::vector<std::vector<wxColour>> m_colorBuffer;
    wxSize m_bufferSize;

    wxDECLARE_EVENT_TABLE();
};