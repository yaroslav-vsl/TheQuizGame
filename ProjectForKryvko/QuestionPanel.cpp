#include "QuestionPanel.h"
#include "MainFrame.h"
#include <wx/msgdlg.h>

QuestionPanel::QuestionPanel(wxWindow* parent, MainFrame* mainFrame, int questionIndex)
    : ImagePanel(parent, L"question_background.jpg"), mainFrame(mainFrame), questionIndex(questionIndex)
{
    this->LoadFromFile(L"background.jpg");
    // Если фоновая картинка не загрузилась, устанавливаем цвет фона
    if (!m_hasBackground) {
        SetBackgroundColour(wxColour(240, 255, 240));
    }

    if (questionIndex >= (int)mainFrame->questions.size()) {
        return;
    }

    MainFrame::Question& question = mainFrame->questions[questionIndex];
    wxString questionStr(question.question);

    // Создаем основной сайзер
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // ВЕРХНЯЯ ЧАСТЬ - для игрока сверху (текст нормальный)
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    questionTextTop = new RotatedText(this, questionStr, 180.0);
    questionTextTop->SetMinSize(wxSize(1000, 100));
    questionTextTop->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    topSizer->AddStretchSpacer();
    topSizer->Add(questionTextTop, 0, wxALIGN_CENTER);
    topSizer->AddStretchSpacer();
    mainSizer->Add(topSizer, 1, wxEXPAND);

    // СРЕДНЯЯ ЧАСТЬ - лево + центр + право
    wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);

    // ЛЕВАЯ ЧАСТЬ - для игрока слева (текст повернут на 90° против часовой)
    questionTextLeft = new RotatedText(this, questionStr, 90.0);
    questionTextLeft->SetMinSize(wxSize(100, -1));
    questionTextLeft->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    middleSizer->Add(questionTextLeft, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

    middleSizer->AddStretchSpacer();

    // ЦЕНТРАЛЬНАЯ ЧАСТЬ С ОТВЕТАМИ
    wxBoxSizer* centerSizer = new wxBoxSizer(wxVERTICAL);

    // Заголовок
    wxStaticText* answersTitle = new wxStaticText(this, wxID_ANY, "Выберите ответ:");
    answersTitle->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    answersTitle->SetForegroundColour(wxColour(0, 0, 100));
    centerSizer->Add(answersTitle, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);

    // Варианты ответов
    for (int i = 0; i < 4; i++) {
        wxString answerText(question.answers[i]);
        answerButtons[i] = new wxButton(this, wxID_ANY, answerText,
            wxDefaultPosition, wxSize(350, 60));
        answerButtons[i]->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        answerButtons[i]->SetBackgroundColour(wxColour(100, 150, 200));
        answerButtons[i]->SetForegroundColour(*wxWHITE);
        centerSizer->Add(answerButtons[i], 0, wxALIGN_CENTER | wxALL, 8);

        answerButtons[i]->Bind(wxEVT_BUTTON, &QuestionPanel::OnAnswerClick, this);
    }

    middleSizer->Add(centerSizer, 0, wxALIGN_CENTER);
    middleSizer->AddStretchSpacer();

    // ПРАВАЯ ЧАСТЬ - для игрока справа (текст повернут на 90° по часовой)
    questionTextRight = new RotatedText(this, questionStr, -90.0);
    questionTextRight->SetMinSize(wxSize(100, -1));
    questionTextRight->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    middleSizer->Add(questionTextRight, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

    mainSizer->Add(middleSizer, 3, wxEXPAND);

    // НИЖНЯЯ ЧАСТЬ - для игрока снизу (текст перевернут на 180°)
    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    questionTextBottom = new RotatedText(this, questionStr, 0.0);
    questionTextBottom->SetMinSize(wxSize(1000, 100));
    questionTextBottom->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    bottomSizer->AddStretchSpacer();
    bottomSizer->Add(questionTextBottom, 0, wxALIGN_CENTER);
    bottomSizer->AddStretchSpacer();
    mainSizer->Add(bottomSizer, 1, wxEXPAND);

    SetSizer(mainSizer);
}

void QuestionPanel::OnAnswerClick(wxCommandEvent& event)
{
    if (questionIndex >= (int)mainFrame->questions.size()) return;

    int selectedAnswer = -1;
    for (int i = 0; i < 4; i++) {
        if (event.GetEventObject() == answerButtons[i]) {
            selectedAnswer = i;
            break;
        }
    }

    MainFrame::Question& question = mainFrame->questions[questionIndex];

    if (selectedAnswer == question.correct)
    {
        if (question.answered == false)
        {
            mainFrame->IncreaseCorrectAnswers();
            mainFrame->MarkQuestionAsAnswered(questionIndex);
            //mainFrame->ReturnToBoard();
            mainFrame->ShowFractal();
        }
        else
        {
            mainFrame->ReturnToBoard();
        }
    
        // Увеличиваем счетчик правильных ответов и отмечаем вопрос как отвеченный

        //wxMessageBox("Правильный ответ! Снежинка Коха становится сложнее!", "Успех", wxOK | wxICON_INFORMATION);
        //mainFrame->ShowFractal();
    }
    else 
    {
        wxMessageBox("Неправильный ответ! Попробуйте еще раз.", "Ошибка", wxOK | wxICON_ERROR);
        //mainFrame->ReturnToBoard();
    }
}