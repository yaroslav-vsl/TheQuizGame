#pragma once
#include "FractalBase.h"
#include "FernFractalPanel.h"
#include "KochSnowflakePanel.h"
#include "JuliaFractalPanel.h"

class FractalPanel : public wxPanel
{
public:
    FractalPanel(wxWindow* parent, MainFrame* mainFrame);
    ~FractalPanel();

    void SetFractalType(MainFrame::FractalType type);

private:
    MainFrame* mainFrame;
    FractalBase* m_currentFractal;
    wxPanel* m_fractalContainer; // Добавляем явное хранение контейнера
    wxChoice* m_fractalChoice;

    void OnFractalChoice(wxCommandEvent& event);
    void OnReturnButtonClick(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};