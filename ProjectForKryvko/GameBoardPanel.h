#pragma once
#include "ImagePanel.h"

class MainFrame;

class GameBoardPanel : public ImagePanel
{
public:
    GameBoardPanel(wxWindow* parent, MainFrame* mainFrame);

private:
    MainFrame* mainFrame;
    wxButton* cellButtons[16];

    void OnCellClick(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void PositionButtonsInInfinityShape();
    void UpdateButtonColors();

    wxDECLARE_EVENT_TABLE();
};