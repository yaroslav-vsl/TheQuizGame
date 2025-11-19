#include "FractalPanel.h"
#include "MainFrame.h"
#include "FernFractalPanel.h"
#include "KochSnowflakePanel.h"

wxBEGIN_EVENT_TABLE(FractalPanel, wxPanel)
EVT_CHOICE(wxID_ANY, FractalPanel::OnFractalChoice)
wxEND_EVENT_TABLE()

FractalPanel::FractalPanel(wxWindow* parent, MainFrame* mainFrame)
    : wxPanel(parent), mainFrame(mainFrame), m_currentFractal(nullptr)
{
    SetBackgroundColour(*wxBLACK);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Выбор фрактала
    //wxBoxSizer* choiceSizer = new wxBoxSizer(wxHORIZONTAL);
    //choiceSizer->Add(new wxStaticText(this, wxID_ANY, "Выберите фрактал:"),
    //    0, wxALIGN_CENTER | wxRIGHT, 10);

    //m_fractalChoice = new wxChoice(this, wxID_ANY);
    //m_fractalChoice->Append("Снежинка Коха");
    //m_fractalChoice->Append("Папоротник Барнсли");
    //m_fractalChoice->SetSelection(static_cast<int>(mainFrame->GetFractalType()));
    //choiceSizer->Add(m_fractalChoice, 0, wxALIGN_CENTER);

    //mainSizer->Add(choiceSizer, 0, wxALIGN_CENTER | wxTOP, 20);

    // Создаем контейнер для фрактала с правильным сайзером
    m_fractalContainer = new wxPanel(this);
    m_fractalContainer->SetBackgroundColour(*wxBLACK);
    m_fractalContainer->SetMinSize(wxSize(600, 400));

    // Важно: создаем сайзер для контейнера
    wxBoxSizer* containerSizer = new wxBoxSizer(wxVERTICAL);
    m_fractalContainer->SetSizer(containerSizer);

    mainSizer->Add(m_fractalContainer, 1, wxEXPAND | wxALL, 10);

    // Кнопка возврата
    wxButton* returnButton = new wxButton(this, wxID_ANY, "Вернуться на игровое поле");
    returnButton->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    returnButton->SetBackgroundColour(wxColour(100, 200, 100));
    returnButton->Bind(wxEVT_BUTTON, &FractalPanel::OnReturnButtonClick, this);

    mainSizer->Add(returnButton, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    SetSizer(mainSizer);

    // Создаем начальный фрактал
    SetFractalType(mainFrame->GetFractalType());
}

FractalPanel::~FractalPanel()
{
    if (m_currentFractal) {
        m_currentFractal->StopAnimation();
    }
}

void FractalPanel::SetFractalType(MainFrame::FractalType type)
{
    // Останавливаем и удаляем текущий фрактал
    if (m_currentFractal) {
        m_currentFractal->StopAnimation();
        m_currentFractal->Destroy();
        m_currentFractal = nullptr;
    }

    // Получаем сайзер контейнера
    wxSizer* containerSizer = m_fractalContainer->GetSizer();
    if (!containerSizer) {
        // Если сайзера нет, создаем его
        containerSizer = new wxBoxSizer(wxVERTICAL);
        m_fractalContainer->SetSizer(containerSizer);
    }
    else {
        // Очищаем контейнер
        containerSizer->Clear(true);
    }

    // Создаем новый фрактал
    try {
        switch (type) {
        case MainFrame::FRACTAL_KOCH_SNOWFLAKE:
            m_currentFractal = new KochSnowflakePanel(m_fractalContainer, mainFrame);
            //wxLogMessage("Создана снежинка Коха");
            break;
        case MainFrame::FRACTAL_FERN:
            m_currentFractal = new FernFractalPanel(m_fractalContainer, mainFrame);
            //wxLogMessage("Создан папоротник Барнсли");
            break;
        default:
           // wxLogError("Неизвестный тип фрактала: %d", static_cast<int>(type));
            return;
        }

        if (m_currentFractal) {
            // Добавляем фрактал в контейнер
            containerSizer->Add(m_currentFractal, 1, wxEXPAND);

            // Обновляем layout
            m_fractalContainer->Layout();
            Layout();

            // Запускаем анимацию
            m_currentFractal->StartAnimation();

            // Обновляем выбор в комбобоксе
            //m_fractalChoice->SetSelection(static_cast<int>(type));

            // Сохраняем выбор
            mainFrame->SetFractalType(type);

            //wxLogMessage("Фрактал успешно создан и запущен");
        }
        else {
            //wxLogError("Не удалось создать фрактал типа: %d", static_cast<int>(type));
        }

    }
    catch (const std::exception& e) {
        wxLogError("Ошибка при создании фрактала: %s", e.what());
    }
}

void FractalPanel::OnFractalChoice(wxCommandEvent& event)
{
    int selection = event.GetSelection();
    if (selection >= 0 && selection <= 1) {
        MainFrame::FractalType type = static_cast<MainFrame::FractalType>(selection);
        SetFractalType(type);
    }
}

void FractalPanel::OnReturnButtonClick(wxCommandEvent& event)
{
    if (m_currentFractal) {
        m_currentFractal->StopAnimation();
    }
    mainFrame->ReturnToBoard();
}