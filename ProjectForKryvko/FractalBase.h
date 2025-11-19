#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

class MainFrame;

class FractalBase : public wxPanel
{
public:
    FractalBase(wxWindow* parent, MainFrame* mainFrame);
    virtual ~FractalBase() = default;

    virtual void StartAnimation() = 0;
    virtual void StopAnimation() = 0;
    virtual wxString GetFractalName() const = 0;

protected:
    MainFrame* mainFrame;
    wxButton* returnButton;

    void CreateCommonUI();
    virtual void OnReturnButtonClick(wxCommandEvent& event);
};