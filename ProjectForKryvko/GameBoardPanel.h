//#pragma once
//#include "ImagePanel.h"
//
//class MainFrame;
//
//class GameBoardPanel : public ImagePanel
//{
//public:
//    GameBoardPanel(wxWindow* parent, MainFrame* mainFrame);
//
//private:
//    MainFrame* mainFrame;
//    wxButton* cellButtons[16];
//
//    void OnCellClick(wxCommandEvent& event);
//    void OnSize(wxSizeEvent& event);
//    void PositionButtonsInInfinityShape();
//    void UpdateButtonColors();
//
//    wxDECLARE_EVENT_TABLE();
//};
//
#pragma once
#include "ImagePanel.h"
#include <wx/mediactrl.h>

class MainFrame;

class VideoPanel : public wxPanel
{
public:
    VideoPanel(wxWindow* parent, const wxString& videoFile, const wxSize& sizeframe);
    ~VideoPanel();

    bool LoadVideo(const wxString& videoFile);
    void Play();
    void Stop();
    bool IsLoaded() const { return m_loaded; }

private:
    wxMediaCtrl* mediaCtrl;
    wxButton* closeButton;
    wxStaticText* statusText;
    bool m_loaded;

    void OnClose(wxCommandEvent& event);
    void OnMediaLoaded(wxMediaEvent& event);
    void OnMediaFinished(wxMediaEvent& event);
    void OnMediaError(wxMediaEvent& event);
    void OnSize(wxSizeEvent& event);

    wxDECLARE_EVENT_TABLE();
};

class GameBoardPanel : public ImagePanel
{
public:
    GameBoardPanel(wxWindow* parent, MainFrame* mainFrame);

private:
    MainFrame* mainFrame;
    wxButton* cellButtons[16];
    wxButton* natureButton;
    wxButton* lifeButton;
    VideoPanel* videoPanel;
    bool isVideoPanelShown;
    bool question_5_answered;

    void OnCellClick(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnNatureButtonClick(wxCommandEvent& event);
    void OnLifeButtonClick(wxCommandEvent& event);
    void OnCloseVideoPanel(wxCommandEvent& event);
    void PositionButtonsInInfinityShape();
    void UpdateButtonColors();
    void ShowVideoPanel(const wxString& videoFile);
    void HideVideoPanel();

    wxDECLARE_EVENT_TABLE();
};