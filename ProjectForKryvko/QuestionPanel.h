//#pragma once
//#include "ImagePanel.h"
//#include "RotatedText.h"
//
//class MainFrame;
//
//class QuestionPanel : public ImagePanel
//{
//public:
//    QuestionPanel(wxWindow* parent, MainFrame* mainFrame, int questionIndex);
//
//private:
//    MainFrame* mainFrame;
//    int questionIndex;
//
//    // Тексты вопросов для 4 сторон
//    RotatedText* questionTextTop;    // Для верхнего игрока
//    RotatedText* questionTextRight;  // Для правого игрока  
//    RotatedText* questionTextBottom; // Для нижнего игрока
//    RotatedText* questionTextLeft;   // Для левого игрока
//
//    wxButton* answerButtons[4];
//
//    void OnAnswerClick(wxCommandEvent& event);
//};

#pragma once
#include "ImagePanel.h"
#include "RotatedText.h"
#include <wx/timer.h>

class MainFrame;

class QuestionPanel : public ImagePanel
{
public:
    QuestionPanel(wxWindow* parent, MainFrame* mainFrame, int questionIndex);
    ~QuestionPanel();

private:
    MainFrame* mainFrame;
    int questionIndex;

    // Тексты вопросов для 4 сторон
    RotatedText* questionTextTop;
    RotatedText* questionTextRight;
    RotatedText* questionTextBottom;
    RotatedText* questionTextLeft;

    wxButton* answerButtons[4];
    wxButton* hintButton;          // Кнопка подсказки
    wxStaticText* timerLabel;      // Метка для отображения таймера
    wxTimer* hintTimer;            // Таймер для подсказки
    int timeElapsed;               // Прошедшее время в секундах

    void OnAnswerClick(wxCommandEvent& event);
    void OnHintButtonClick(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void UpdateTimerDisplay();
    void EnableHint();
    void ShowHint();

    wxDECLARE_EVENT_TABLE();
};