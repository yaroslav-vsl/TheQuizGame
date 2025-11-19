#include "StartPanel.h"
#include "MainFrame.h"
#include "MyButton.h"

StartPanel::StartPanel(wxWindow* parent, MainFrame* mainFrame)
    : ImagePanel(parent, L"background.jpg", mainFrame->GetClientSize()), mainFrame(mainFrame)
{
    wxSize sizeImg = mainFrame->GetSize();
    this->SetSize(sizeImg);
    this->LoadFromFile(L"background.jpg");

    // Если фоновая картинка не загрузилась, устанавливаем цвет фона
    if (!m_hasBackground) {
        SetBackgroundColour(wxColour(240, 240, 240));
    }

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    //wxBitmap roundBitmap = CreateRoundButton("СТАРТ", wxColour(255, 0, 0));
    //startButton = new wxBitmapButton(this, wxID_ANY, roundBitmap, wxDefaultPosition, wxSize(300, 300));
    //sizer->AddStretchSpacer();
    //sizer->Add(startButton, 0, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL);
    //sizer->AddStretchSpacer();

    MyButton* startButton = new MyButton(this, wxID_ANY, wxPoint(100, 100), wxSize(300, 300));
    sizer->AddStretchSpacer();
    sizer->Add(startButton, 0, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL);
    sizer->AddStretchSpacer();

    SetSizer(sizer);

    //startButton->Bind(wxEVT_BUTTON, &StartPanel::OnStartButtonClick, this);
    startButton->Bind(wxEVT_LEFT_DOWN, &StartPanel::OnStartButtonClick, this);
}

wxBitmap StartPanel::CreateRoundButton(const wxString& text, const wxColour& color)
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage image;
    image.LoadFile(L"start.png", wxBITMAP_TYPE_PNG);
    wxBitmap bitmap;
    if (image.IsOk())
    {
        constexpr auto width = 150;
        constexpr auto height = 150;

        bitmap = wxBitmap(image.Scale(width, height, wxIMAGE_QUALITY_HIGH));
        m_hasBackground = true;
    }
    wxPaintDC dc(this);

    int dc_width, dc_height;
    dc.GetSize(&dc_width, &dc_height);

    if ((bitmap).IsOk())
    {
        dc.DrawBitmap(bitmap, 0, 0, false);
    }

    dc.SetBackground(*wxTRANSPARENT_BRUSH);
    dc.Clear();

    return bitmap;
}

void StartPanel::OnStartButtonClick(wxMouseEvent &event)
{
    mainFrame->StartGame();
}

//void StartPanel::OnStartButtonClick(wxCommandEvent &event)
//{
//    mainFrame->StartGame();
//}