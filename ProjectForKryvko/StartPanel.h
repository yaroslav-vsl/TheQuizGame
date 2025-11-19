#pragma once
#include "ImagePanel.h"

class MainFrame;

class StartPanel : public ImagePanel
{
public:
    StartPanel(wxWindow* parent, MainFrame* mainFrame);

private:
    MainFrame* mainFrame;
    wxBitmapButton* startButton;

    void OnStartButtonClick(wxMouseEvent &event);
    //void OnStartButtonClick(wxCommandEvent &event);
    wxBitmap CreateRoundButton(const wxString& text, const wxColour& color);
};