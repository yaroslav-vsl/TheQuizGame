#pragma once
#include <wx/wx.h>
#include <vector>
#include <string>

class StartPanel;
class GameBoardPanel;
class QuestionPanel;
class FractalPanel;

class MainFrame : public wxFrame
{
public:
    enum FractalType {
        FRACTAL_KOCH_SNOWFLAKE = 0,
        FRACTAL_FERN = 1,
        FRACTAL_MANDELBROT = 2,
        FRACTAL_JULIA = 3  // Добавляем Жюлиа
    };

    FractalType GetFractalType() const { return m_fractalType; }
    
    MainFrame();

    void ShowStartPanel();
    void StartGame();
    void ShowQuestion(int questionIndex);
    void ShowFractal();
    void ReturnToBoard();
    void LoadQuestions();
    void IncreaseCorrectAnswers();
    void MarkQuestionAsAnswered(int questionIndex);
    void SetFractalType(FractalType type) { m_fractalType = type; }
    struct Question {
        std::string question;
        std::vector<std::string> answers;
        int correct;
        bool answered;
        std::string hint;
    };
    std::vector<bool> answered_question;
    std::vector<Question> questions;
    int GetCorrectAnswersCount() const { return m_correctAnswersCount; }
    int GetTotalQuestions() const { return questions.size(); }
    bool IsQuestionAnswered(int index) const;

private:
    FractalType m_fractalType;
    wxBoxSizer* mainSizer;
    StartPanel* startPanel;
    GameBoardPanel* gameBoardPanel;
    QuestionPanel* questionPanel;
    FractalPanel* fractalPanel;

    int m_correctAnswersCount;

    void ClearMainSizer();
    void CreateDefaultQuestions();
    void OnClose(wxCloseEvent& event);

    wxDECLARE_EVENT_TABLE();
};