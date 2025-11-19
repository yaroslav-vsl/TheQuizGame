#pragma once
#include "ImagePanel.h"
#include "RotatedText.h"

class MainFrame;

class QuestionPanel : public ImagePanel
{
public:
    QuestionPanel(wxWindow* parent, MainFrame* mainFrame, int questionIndex);

private:
    MainFrame* mainFrame;
    int questionIndex;

    // Тексты вопросов для 4 сторон
    RotatedText* questionTextTop;    // Для верхнего игрока
    RotatedText* questionTextRight;  // Для правого игрока  
    RotatedText* questionTextBottom; // Для нижнего игрока
    RotatedText* questionTextLeft;   // Для левого игрока

    wxButton* answerButtons[4];

    void OnAnswerClick(wxCommandEvent& event);
};