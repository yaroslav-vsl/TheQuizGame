#include "StartPanel.h"
#include "MainFrame.h"
#include "MyButton.h"

StartPanel::StartPanel(wxWindow* parent, MainFrame* mainFrame)
    : ImagePanel(parent, L"background.jpg"), mainFrame(mainFrame)
{
    this->LoadFromFile(L"background.jpg");
    // Если фоновая картинка не загрузилась, устанавливаем цвет фона
    if (!m_hasBackground) {
        SetBackgroundColour(wxColour(240, 240, 240));
    }

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Добавляем заголовок
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "ВИКТОРИНА");
    title->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    title->SetForegroundColour(wxColour(255, 255, 255));
    title->SetBackgroundColour(wxColour(0, 0, 0, 100)); // Полупрозрачный фон
    sizer->Add(title, 0, wxALIGN_CENTER | wxTOP, 100);

    // Добавляем описание
    wxStaticText* description = new wxStaticText(this, wxID_ANY, "Проверьте свои знания!");
    description->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    description->SetForegroundColour(wxColour(255, 255, 255));
    description->SetBackgroundColour(wxColour(0, 0, 0, 100));
    sizer->Add(description, 0, wxALIGN_CENTER | wxTOP, 20);

    sizer->AddStretchSpacer();

    wxBitmap roundBitmap = CreateRoundButton("СТАРТ", wxColour(255, 0, 0));
    startButton = new wxBitmapButton(this, wxID_ANY, roundBitmap,
        wxDefaultPosition, wxSize(150, 150));

    sizer->Add(startButton, 0, wxALIGN_CENTER | wxALL, 50);

    SetSizer(sizer);

    startButton->Bind(wxEVT_BUTTON, &StartPanel::OnStartButtonClick, this);
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


void StartPanel::OnStartButtonClick(wxCommandEvent& event)
{
    mainFrame->StartGame();
}