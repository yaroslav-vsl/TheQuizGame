#include <wx/wx.h>
#include <wx/app.h>
#include "MainFrame.h"

class QuizApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        // Инициализация wxWidgets
        if (!wxApp::OnInit())
            return false;

        MainFrame* mainFrame = new MainFrame();
        mainFrame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(QuizApp);