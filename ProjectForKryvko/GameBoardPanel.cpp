#include "GameBoardPanel.h"
#include "MainFrame.h"
#include <cmath>
#include "TransparentPanel.h"
#include "MyButton.h"

wxBEGIN_EVENT_TABLE(GameBoardPanel, wxPanel)
EVT_SIZE(GameBoardPanel::OnSize)
wxEND_EVENT_TABLE()

GameBoardPanel::GameBoardPanel(wxWindow* parent, MainFrame* mainFrame)
    : ImagePanel(parent,L""), mainFrame(mainFrame)
{
    wxSize sizeImg = mainFrame->GetSize();
    this->SetSize(sizeImg);
    this->LoadFromFile(L"background.jpg");
    // Если фоновая картинка не загрузилась, устанавливаем цвет фона
    if (!m_hasBackground) {
        SetBackgroundColour(wxColour(230, 230, 250));
    }

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Создаем контейнер для кнопок
    MyTransparentPanel* buttonContainer = new MyTransparentPanel(this);
    buttonContainer->SetMinSize(wxSize(700, 500));

    mainSizer->Add(buttonContainer, 1, wxEXPAND | wxALL, 20);
    
    SetSizer(mainSizer);

    // Создаем 16 кнопок
    for (int i = 0; i < 16; i++) {
        cellButtons[i] = new wxButton(buttonContainer, wxID_ANY, wxString::Format("%d", i + 1), wxDefaultPosition, wxSize(80, 80));
        cellButtons[i]->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        cellButtons[i]->Bind(wxEVT_BUTTON, &GameBoardPanel::OnCellClick, this);
    }

    // Обновляем цвета кнопок
    UpdateButtonColors();

    // Располагаем кнопки в форме бесконечности
    PositionButtonsInInfinityShape();
}

void GameBoardPanel::UpdateButtonColors()
{
    for (int i = 0; i < 16; i++) {
        if (mainFrame->IsQuestionAnswered(i)) {
            // Зеленый цвет для отвеченных вопросов
            cellButtons[i]->SetBackgroundColour(wxColour(67, 160, 71));
            cellButtons[i]->SetForegroundColour(*wxWHITE);
            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1)); // Добавляем галочку
        }
        else {
            // Синий цвет для неотвеченных вопросов
            cellButtons[i]->SetBackgroundColour(wxColour(100, 150, 200));
            cellButtons[i]->SetForegroundColour(*wxWHITE);
            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1));
        }
        // Принудительно обновляем внешний вид кнопки
        cellButtons[i]->Refresh();
    }
}

//void GameBoardPanel::UpdateButtonColors()
//{
//    int answered = mainFrame->GetCorrectAnswersCount();
//
//    for (int i = 0; i < 16; i++) {
//        if (mainFrame->IsQuestionAnswered(i)) {
//            // "Прокачка" цвета по мере прогресса
//            if (answered <= 4) {
//                cellButtons[i]->SetBackgroundColour(wxColour(129, 199, 132)); // Новичок - светло-зеленый
//            }
//            else if (answered <= 8) {
//                cellButtons[i]->SetBackgroundColour(wxColour(56, 142, 60));   // Ученик - зеленый
//            }
//            else if (answered <= 12) {
//                cellButtons[i]->SetBackgroundColour(wxColour(255, 152, 0));   // Мастер - оранжевый
//            }
//            else {
//                cellButtons[i]->SetBackgroundColour(wxColour(233, 30, 99));   // Гуру - розовый
//            }
//            cellButtons[i]->SetForegroundColour(wxColour(255, 255, 255));
//        }
//        else {
//            // Неотвеченные - серые "заблокированные"
//            cellButtons[i]->SetBackgroundColour(wxColour(156, 39, 176));
//            cellButtons[i]->SetForegroundColour(*wxWHITE);
//            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1)); // С замком
//        }
//        cellButtons[i]->Refresh();
//    }
//}

//void GameBoardPanel::UpdateButtonColors()
//{
//    for (int i = 0; i < 16; i++) {
//        if (mainFrame->IsQuestionAnswered(i)) {
//            // Отвеченные - цвета математических констант
//            wxColour mathColors[] = {
//                wxColour(230, 74, 25),   // Пи - оранжево-красный
//                wxColour(155, 81, 224),  // e - фиолетовый
//                wxColour(76, 175, 80),   // Золотое сечение - зеленый
//                wxColour(3, 169, 244),   // Мнимая единица - голубой
//                wxColour(255, 193, 7)    // Бесконечность - золотой
//            };
//            cellButtons[i]->SetBackgroundColour(mathColors[i % 5]);
//            cellButtons[i]->SetForegroundColour(wxColour(255, 255, 255));
//            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1)); // С символом пи
//        }
//        else {
//            // Неотвеченные - серые как нерешенные задачи
//            cellButtons[i]->SetBackgroundColour(wxColour(189, 189, 189));
//            cellButtons[i]->SetForegroundColour(wxColour(66, 66, 66));
//            cellButtons[i]->SetLabel(wxString::Format("? %d", i + 1));
//        }
//        cellButtons[i]->Refresh();
//    }
//}

void GameBoardPanel::PositionButtonsInInfinityShape()
{
    wxSize containerSize = GetClientSize();
    int centerX = containerSize.GetWidth() / 2;
    int centerY = containerSize.GetHeight() / 2;
    int radius = std::min(centerX, centerY) * 0.9;

    // Параметрическое уравнение лемнискаты Бернулли (символ бесконечности)
    for (int i = 0; i < 16; i++) {
        double t = (2.0 * M_PI * i) / 16.0;

        // Уравнение лемнискаты Бернулли
        double scale = 1.0 + sin(t) * sin(t);
        double x = radius * cos(t) / scale;
        double y = radius * sin(t) * cos(t) / scale;

        // Смещаем и масштабируем
        int posX = centerX + static_cast<int>(x);
        int posY = centerY + static_cast<int>(y);

        cellButtons[i]->SetPosition(wxPoint(posX - 40, posY - 40));
    }
}

void GameBoardPanel::OnSize(wxSizeEvent& event)
{
    PositionButtonsInInfinityShape();
    Refresh();
    event.Skip();
}

void GameBoardPanel::OnCellClick(wxCommandEvent& event)
{
    for (int i = 0; i < 16; i++) {
        if (event.GetEventObject() == cellButtons[i]) {
            int questionIndex = i % mainFrame->questions.size();
            mainFrame->ShowQuestion(questionIndex);
            break;
        }
    }
}