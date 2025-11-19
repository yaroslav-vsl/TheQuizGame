//#include "QuestionPanel.h"
//#include "MainFrame.h"
//#include <wx/msgdlg.h>
//
//QuestionPanel::QuestionPanel(wxWindow* parent, MainFrame* mainFrame, int questionIndex)
//    : ImagePanel(parent, L"question_background.jpg"), mainFrame(mainFrame), questionIndex(questionIndex)
//{
//    wxSize sizeImg = mainFrame->GetSize();
//    this->SetSize(sizeImg);
//    this->LoadFromFile(L"background.jpg");
//    // Если фоновая картинка не загрузилась, устанавливаем цвет фона
//    if (!m_hasBackground) {
//        SetBackgroundColour(wxColour(240, 255, 240));
//    }
//
//    if (questionIndex >= (int)mainFrame->questions.size()) {
//        return;
//    }
//
//    MainFrame::Question& question = mainFrame->questions[questionIndex];
//    wxString questionStr(question.question);
//
//    // Создаем основной сайзер
//    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
//
//    // ВЕРХНЯЯ ЧАСТЬ - для игрока сверху (текст нормальный)
//    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
//    questionTextTop = new RotatedText(this, questionStr, 180.0);
//    questionTextTop->SetMinSize(wxSize(700, 100));
//    questionTextTop->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
//    topSizer->AddStretchSpacer();
//    topSizer->Add(questionTextTop, 0, wxALIGN_CENTER);
//    topSizer->AddStretchSpacer();
//    mainSizer->Add(topSizer, 1, wxEXPAND);
//
//    // СРЕДНЯЯ ЧАСТЬ - лево + центр + право
//    wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);
//
//    // ЛЕВАЯ ЧАСТЬ - для игрока слева (текст повернут на 90° против часовой)
//    questionTextLeft = new RotatedText(this, questionStr, 90.0);
//    questionTextLeft->SetMinSize(wxSize(100, 700));
//    questionTextLeft->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
//    middleSizer->Add(questionTextLeft, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
//
//    middleSizer->AddStretchSpacer();
//
//    // ЦЕНТРАЛЬНАЯ ЧАСТЬ С ОТВЕТАМИ
//    wxBoxSizer* centerSizer = new wxBoxSizer(wxVERTICAL);
//
//    // Заголовок
//    //wxStaticText* answersTitle = new wxStaticText(this, wxID_ANY, "Выберите ответ:");
//    //answersTitle->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
//    //answersTitle->SetForegroundColour(wxColour(0, 0, 100));
//    //centerSizer->Add(answersTitle, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);
//
//    // Варианты ответов
//    for (int i = 0; i < 4; i++) {
//        wxString answerText(question.answers[i]);
//        answerButtons[i] = new wxButton(this, wxID_ANY, answerText,
//            wxDefaultPosition, wxSize(350, 60));
//        answerButtons[i]->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
//        answerButtons[i]->SetBackgroundColour(wxColour(100, 150, 200));
//        answerButtons[i]->SetForegroundColour(*wxWHITE);
//        centerSizer->Add(answerButtons[i], 0, wxALIGN_CENTER | wxALL, 8);
//
//        answerButtons[i]->Bind(wxEVT_BUTTON, &QuestionPanel::OnAnswerClick, this);
//    }
//
//    middleSizer->Add(centerSizer, 0, wxALIGN_CENTER);
//    middleSizer->AddStretchSpacer();
//
//    // ПРАВАЯ ЧАСТЬ - для игрока справа (текст повернут на 90° по часовой)
//    questionTextRight = new RotatedText(this, questionStr, -90.0);
//    questionTextRight->SetMinSize(wxSize(100, 700));
//    questionTextRight->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
//    middleSizer->Add(questionTextRight, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
//
//    mainSizer->Add(middleSizer, 3, wxEXPAND);
//
//    // НИЖНЯЯ ЧАСТЬ - для игрока снизу (текст перевернут на 180°)
//    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
//    questionTextBottom = new RotatedText(this, questionStr, 0.0);
//    questionTextBottom->SetMinSize(wxSize(700, 100));
//    questionTextBottom->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
//
//    bottomSizer->AddStretchSpacer();
//    bottomSizer->Add(questionTextBottom, 0, wxALIGN_CENTER);
//    bottomSizer->AddStretchSpacer();
//    mainSizer->Add(bottomSizer, 1, wxEXPAND);
//
//    SetSizer(mainSizer);
//}
//
//void QuestionPanel::OnAnswerClick(wxCommandEvent& event)
//{
//    if (questionIndex >= (int)mainFrame->questions.size()) return;
//
//    int selectedAnswer = -1;
//    for (int i = 0; i < 4; i++) {
//        if (event.GetEventObject() == answerButtons[i]) {
//            selectedAnswer = i;
//            break;
//        }
//    }
//
//    MainFrame::Question& question = mainFrame->questions[questionIndex];
//
//    if (selectedAnswer == question.correct)
//    {
//        if (question.answered == false)
//        {
//            mainFrame->IncreaseCorrectAnswers();
//            mainFrame->MarkQuestionAsAnswered(questionIndex);
//            //mainFrame->ReturnToBoard();
//            mainFrame->ShowFractal();
//        }
//        else
//        {
//            mainFrame->ReturnToBoard();
//        }
//    
//        // Увеличиваем счетчик правильных ответов и отмечаем вопрос как отвеченный
//
//        //wxMessageBox("Правильный ответ! Снежинка Коха становится сложнее!", "Успех", wxOK | wxICON_INFORMATION);
//        //mainFrame->ShowFractal();
//    }
//    else 
//    {
//        wxMessageBox("Неправильный ответ! Попробуйте еще раз.", "Ошибка", wxOK | wxICON_ERROR);
//        //mainFrame->ReturnToBoard();
//    }
//}

#include "QuestionPanel.h"
#include "MainFrame.h"
#include <wx/msgdlg.h>
#include <algorithm>

wxBEGIN_EVENT_TABLE(QuestionPanel, ImagePanel)
EVT_BUTTON(wxID_ANY, QuestionPanel::OnAnswerClick)
EVT_TIMER(wxID_ANY, QuestionPanel::OnTimer)
wxEND_EVENT_TABLE()

QuestionPanel::QuestionPanel(wxWindow* parent, MainFrame* mainFrame, int questionIndex)
    : ImagePanel(parent, L"question_background.jpg"), mainFrame(mainFrame), questionIndex(questionIndex),
    timeElapsed(0), hintTimer(nullptr), hintButton(nullptr), timerLabel(nullptr)
{
    wxSize sizeImg = mainFrame->GetSize();
    this->SetSize(sizeImg);
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
    questionTextTop->SetMinSize(wxSize(700, 100));
    questionTextTop->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    topSizer->AddStretchSpacer();
    topSizer->Add(questionTextTop, 0, wxALIGN_CENTER);
    topSizer->AddStretchSpacer();
    mainSizer->Add(topSizer, 1, wxEXPAND);

    // СРЕДНЯЯ ЧАСТЬ - лево + центр + право
    wxBoxSizer* middleSizer = new wxBoxSizer(wxHORIZONTAL);

    // ЛЕВАЯ ЧАСТЬ - для игрока слева (текст повернут на 90° против часовой)
    questionTextLeft = new RotatedText(this, questionStr, 90.0);
    questionTextLeft->SetMinSize(wxSize(100, 700));
    questionTextLeft->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    middleSizer->Add(questionTextLeft, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

    middleSizer->AddStretchSpacer();

    // ЦЕНТРАЛЬНАЯ ЧАСТЬ С ОТВЕТАМИ
    wxBoxSizer* centerSizer = new wxBoxSizer(wxVERTICAL);

    // Заголовок
    //wxStaticText* answersTitle = new wxStaticText(this, wxID_ANY, "Выберите ответ:");
    //answersTitle->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    //answersTitle->SetForegroundColour(wxColour(0, 0, 100));
    //centerSizer->Add(answersTitle, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);

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

    // Добавляем кнопку подсказки и таймер
    wxBoxSizer* hintSizer = new wxBoxSizer(wxHORIZONTAL);

    // Метка таймера
    timerLabel = new wxStaticText(this, wxID_ANY, "Подсказка через: 60 сек");
    timerLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    timerLabel->SetForegroundColour(wxColour(150, 150, 150));
    hintSizer->Add(timerLabel, 0, wxALIGN_CENTER | wxRIGHT, 10);

    // Кнопка подсказки
    hintButton = new wxButton(this, wxID_ANY, "Подсказка");
    hintButton->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    hintButton->SetBackgroundColour(wxColour(200, 200, 200)); // Серый - неактивная
    hintButton->SetForegroundColour(wxColour(100, 100, 100));
    hintButton->Enable(false); // Изначально неактивна

    hintButton->Bind(wxEVT_BUTTON, &QuestionPanel::OnHintButtonClick, this);
    hintSizer->Add(hintButton, 0, wxALIGN_CENTER);

    centerSizer->Add(hintSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    middleSizer->Add(centerSizer, 0, wxALIGN_CENTER);
    middleSizer->AddStretchSpacer();

    // ПРАВАЯ ЧАСТЬ - для игрока справа (текст повернут на 90° по часовой)
    questionTextRight = new RotatedText(this, questionStr, -90.0);
    questionTextRight->SetMinSize(wxSize(100, 700));
    questionTextRight->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    middleSizer->Add(questionTextRight, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

    mainSizer->Add(middleSizer, 3, wxEXPAND);

    // НИЖНЯЯ ЧАСТЬ - для игрока снизу (текст перевернут на 180°)
    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    questionTextBottom = new RotatedText(this, questionStr, 0.0);
    questionTextBottom->SetMinSize(wxSize(700, 100));
    questionTextBottom->SetFonts(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    bottomSizer->AddStretchSpacer();
    bottomSizer->Add(questionTextBottom, 0, wxALIGN_CENTER);
    bottomSizer->AddStretchSpacer();
    mainSizer->Add(bottomSizer, 1, wxEXPAND);

    SetSizer(mainSizer);

    // Запускаем таймер для подсказки (обновление каждую секунду)
    hintTimer = new wxTimer(this, wxID_ANY);
    hintTimer->Start(1000); // 1000 мс = 1 секунда
}

QuestionPanel::~QuestionPanel()
{
    if (hintTimer && hintTimer->IsRunning()) {
        hintTimer->Stop();
    }
    delete hintTimer;
}

void QuestionPanel::OnTimer(wxTimerEvent& event)
{
    timeElapsed++;
    int timeLeft = 60 - timeElapsed;

    if (timeLeft > 0) {
        UpdateTimerDisplay();
    }
    else {
        // Время вышло - активируем подсказку
        EnableHint();
        hintTimer->Stop();
    }
}

void QuestionPanel::UpdateTimerDisplay()
{
    int timeLeft = 60 - timeElapsed;
    wxString timerText = wxString::Format("Подсказка через: %d сек", timeLeft);
    timerLabel->SetLabel(timerText);

    // Меняем цвет в зависимости от оставшегося времени
    if (timeLeft <= 10) {
        timerLabel->SetForegroundColour(wxColour(255, 100, 100)); // Красный
    }
    else if (timeLeft <= 30) {
        timerLabel->SetForegroundColour(wxColour(255, 165, 0));   // Оранжевый
    }
}

void QuestionPanel::EnableHint()
{
    hintButton->Enable(true);
    hintButton->SetBackgroundColour(wxColour(255, 193, 7)); // Золотой
    hintButton->SetForegroundColour(wxColour(0, 0, 0));     // Черный текст
    timerLabel->SetLabel("Подсказка доступна!");
    timerLabel->SetForegroundColour(wxColour(0, 150, 0));   // Зеленый
}

void QuestionPanel::OnHintButtonClick(wxCommandEvent& event)
{
    ShowHint();
    Refresh();    // Принудительная перерисовка
    Update();     // Немедленное обновление
}

void QuestionPanel::ShowHint()
{
    if (questionIndex >= (int)mainFrame->questions.size()) return;

    MainFrame::Question& question = mainFrame->questions[questionIndex];

    // Разные типы подсказок в зависимости от вопроса
    wxString hint;

    // Определяем тип подсказки на основе вопроса
    //wxString questionText = question.question;
    //questionText.MakeLower();

    //if (questionText.Contains("столица") || questionText.Contains("город")) {
    //    hint = "💡 Подсказка: Это европейский город";
    //}
    //else if (questionText.Contains("планета") || questionText.Contains("солнеч")) {
    //    hint = "💡 Подсказка: Эта планета имеет кольца";
    //}
    //else if (questionText.Contains("сколько") || questionText.Contains("число")) {
    //    // Для числовых вопросов показываем диапазон
    //    int correct = question.correct;
    //    wxString correctAnswer = question.answers[correct];

    //    // Пытаемся извлечь число из правильного ответа
    //    int number;
    //    if (correctAnswer.ToInt(&number)) {
    //        int min = std::max(1, number - 2);
    //        int max = number + 2;
    //        hint = wxString::Format("💡 Подсказка: Число между %d и %d", min, max);
    //    }
    //    else {
    //        hint = "💡 Подсказка: Правильный ответ находится в середине списка";
    //    }
    //}
    //else if (questionText.Contains("формула") || questionText.Contains("химич")) {
    //    hint = "💡 Подсказка: Содержит атомы водорода";
    //}
    //else {
    //    // Общая подсказка - исключаем один неправильный ответ
    //    int wrongToRemove = (question.correct + 1) % 4; // Убираем следующий за правильным ответ
    //    hint = wxString::Format("💡 Подсказка: Ответ '%s' точно неправильный",
    //        question.answers[wrongToRemove]);
    //}

    hint = question.hint;//wxString::Format("%d", question.hint);
    wxMessageBox(hint, "Подсказка", wxOK | wxICON_INFORMATION);

    // Делаем кнопку неактивной после использования
    hintButton->Enable(false);
    hintButton->SetBackgroundColour(wxColour(150, 150, 150));
    hintButton->SetLabel("Подсказка использована");
    timerLabel->SetLabel("");
    timerLabel->Disable();
    Refresh();    // Принудительная перерисовка
    Update();     // Немедленное обновление
    Layout();
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
            // Останавливаем таймер при правильном ответе
            if (hintTimer && hintTimer->IsRunning()) {
                hintTimer->Stop();
            }
            mainFrame->ShowFractal();
        }
        else
        {
            mainFrame->ReturnToBoard();
        }
    }
    else
    {
        wxMessageBox("Неправильный ответ! Попробуйте еще раз.", "Ошибка", wxOK | wxICON_ERROR);
    }
}