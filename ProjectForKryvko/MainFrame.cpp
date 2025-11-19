#include "MainFrame.h"
#include "StartPanel.h"
#include "GameBoardPanel.h"
#include "QuestionPanel.h"
#include "FractalPanel.h"
#include <fstream>
#include <sstream>
#include <wx/msgdlg.h>
#include <wx/file.h>
#include <wx/textfile.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Викторина - Фракталы", wxDefaultPosition, wxSize(800, 600)),
    startPanel(NULL), gameBoardPanel(NULL), questionPanel(NULL), fractalPanel(NULL),
    m_correctAnswersCount(0), m_fractalType(FRACTAL_KOCH_SNOWFLAKE) // По умолчанию снежинка
{
    this->Maximize();
    //this->SetMinSize(this->GetSize());

    mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    LoadQuestions();
    ShowStartPanel();

    Center();
}

void MainFrame::ShowFractal()
{
    ClearMainSizer();
    fractalPanel = new FractalPanel(this, this); // Теперь используем общий FractalPanel
    mainSizer->Add(fractalPanel, 1, wxEXPAND);
    Layout();
}

void MainFrame::IncreaseCorrectAnswers()
{
    m_correctAnswersCount++;
}

void MainFrame::MarkQuestionAsAnswered(int questionIndex)
{
    if (questionIndex >= 0 && questionIndex < (int)questions.size()) {
        questions[questionIndex].answered = true;
    }
}

bool MainFrame::IsQuestionAnswered(int index) const
{
    if (index >= 0 && index < (int)questions.size()) {
        return questions[index].answered;
    }
    return false;
}

void MainFrame::LoadQuestions()
{
    questions.clear();
    CreateDefaultQuestions();
}

void MainFrame::CreateDefaultQuestions()
{
    questions.clear();

    // 16 вопросов для 16 кнопок
    std::vector<Question> defaultQuestions = {
        {"Поезд длиной 1 км едет со скоростью 1 км/мин. Ему нужно проехать через тоннель длиной 1 км. За сколько минут поезд полностью проедет тоннель?", {"За 1 минуту", "За 1.5 минуты", "За 2 минуты", "За 0.5 минуты"}, 2, false, "Чтобы поезд длиной 1 км полностью проехал тоннель длиной 1 км, его локомотив должен пройти путь, равный сумме длин поезда и тоннеля. Сначала локомотив въезжает в тоннель, и чтобы последний вагон выехал из тоннеля, локомотив должен проехать 1 км (длина тоннеля) + 1 км (длина поезда) = 2 км. При скорости 1 км/мин на это потребуется 2 минуты"},
        {"Цена товара сначала повысилась на 20%, а затем понизилась на 20%. Как изменилась конечная цена по сравнению с первоначальной?", {"3", "4", "5", "6"}, 1, false},
        {"Самая большая планета?", {"Земля", "Марс", "Юпитер", "Венера"}, 2, false},
        {"Сколько цветов у радуги?", {"5", "6", "7", "8"}, 2, false},
        {"Какая планета Красная?", {"Венера", "Марс", "Юпитер", "Сатурн"}, 1, false},
        {"Столица Японии?", {"Пекин", "Сеул", "Токио", "Бангкок"}, 2, false},
        {"Сколько дней в високосном году?", {"365", "366", "367", "364"}, 1, false},
        {"Самое глубокое озеро?", {"Байкал", "Верхнее", "Виктория", "Танганьика"}, 0, false},
        {"Химическая формула воды?", {"O2", "CO2", "H2O", "H2O2"}, 2, false},
        {"Сколько континентов на Земле?", {"5", "6", "7", "8"}, 2, false},
        {"Самый большой океан?", {"Атлантический", "Индийский", "Тихий", "Северный Ледовитый"}, 2, false},
        {"Год основания Москвы?", {"1147", "1247", "1347", "1447"}, 0, false},
        {"Сколько планет в Солнечной системе?", {"7", "8", "9", "10"}, 1, false},
        {"Самая длинная река?", {"Нил", "Амазонка", "Янцзы", "Миссисипи"}, 1, false},
        {"Столица Австралии?", {"Сидней", "Мельбурн", "Канберра", "Перт"}, 2, false},
        {"Сколько часов в сутках?", {"12", "24", "36", "48"}, 1, false}
    };

    questions = defaultQuestions;
}

void MainFrame::ClearMainSizer()
{
    if (mainSizer->GetItemCount() > 0) {
        mainSizer->Clear(true);
    }
    startPanel = NULL;
    gameBoardPanel = NULL;
    questionPanel = NULL;
    fractalPanel = NULL;
}

void MainFrame::ShowStartPanel()
{
    ClearMainSizer();
    startPanel = new StartPanel(this, this);
    mainSizer->Add(startPanel, 1, wxEXPAND);
    Layout();
}

void MainFrame::StartGame()
{
    ClearMainSizer();
    gameBoardPanel = new GameBoardPanel(this, this);
    mainSizer->Add(gameBoardPanel, 1, wxEXPAND);
    Layout();
}

void MainFrame::ShowQuestion(int questionIndex)
{
    ClearMainSizer();
    questionPanel = new QuestionPanel(this, this, questionIndex);
    mainSizer->Add(questionPanel, 1, wxEXPAND);
    Layout();
}

void MainFrame::ReturnToBoard()
{
    ClearMainSizer();
    gameBoardPanel = new GameBoardPanel(this, this);
    mainSizer->Add(gameBoardPanel, 1, wxEXPAND);
    Layout();
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    Destroy();
}