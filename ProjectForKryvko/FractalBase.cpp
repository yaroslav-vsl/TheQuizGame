#include "FractalBase.h"
#include "MainFrame.h"

FractalBase::FractalBase(wxWindow* parent, MainFrame* mainFrame)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
    mainFrame(mainFrame)
{
    //wxLogMessage("Конструктор FractalBase: parent=%p, this=%p", parent, this);
    SetBackgroundColour(*wxBLACK);
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void FractalBase::CreateCommonUI()
{
    //wxLogMessage("FractalBase::CreateCommonUI() для %p", this);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Кнопка возврата
    //returnButton = new wxButton(this, wxID_ANY, "Вернуться на игровое поле");
    //returnButton->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    //returnButton->SetBackgroundColour(wxColour(100, 200, 100));

    //sizer->AddStretchSpacer();
    //sizer->Add(returnButton, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    SetSizer(sizer);

    //returnButton->Bind(wxEVT_BUTTON, &FractalBase::OnReturnButtonClick, this);
}

void FractalBase::OnReturnButtonClick(wxCommandEvent& event)
{
    StopAnimation();
    mainFrame->ReturnToBoard();
}