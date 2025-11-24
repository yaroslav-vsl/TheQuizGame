//#include "GameBoardPanel.h"
//#include "MainFrame.h"
//#include <cmath>
//#include "TransparentPanel.h"
//#include "MyButton.h"
//
//wxBEGIN_EVENT_TABLE(GameBoardPanel, wxPanel)
//EVT_SIZE(GameBoardPanel::OnSize)
//wxEND_EVENT_TABLE()
//
//GameBoardPanel::GameBoardPanel(wxWindow* parent, MainFrame* mainFrame)
//    : ImagePanel(parent,L""), mainFrame(mainFrame)
//{
//    wxSize sizeImg = mainFrame->GetSize();
//    this->SetSize(sizeImg);
//    this->LoadFromFile(L"background.jpg");
//    // Если фоновая картинка не загрузилась, устанавливаем цвет фона
//    if (!m_hasBackground) {
//        SetBackgroundColour(wxColour(230, 230, 250));
//    }
//
//    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
//
//    // Создаем контейнер для кнопок
//    MyTransparentPanel* buttonContainer = new MyTransparentPanel(this);
//    buttonContainer->SetMinSize(wxSize(700, 500));
//
//    mainSizer->Add(buttonContainer, 1, wxEXPAND | wxALL, 20);
//    
//    SetSizer(mainSizer);
//
//    // Создаем 16 кнопок
//    for (int i = 0; i < 16; i++) {
//        cellButtons[i] = new wxButton(buttonContainer, wxID_ANY, wxString::Format("%d", i + 1), wxDefaultPosition, wxSize(80, 80));
//        cellButtons[i]->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
//        cellButtons[i]->Bind(wxEVT_BUTTON, &GameBoardPanel::OnCellClick, this);
//    }
//
//    // Обновляем цвета кнопок
//    UpdateButtonColors();
//
//    // Располагаем кнопки в форме бесконечности
//    PositionButtonsInInfinityShape();
//}
//
//void GameBoardPanel::UpdateButtonColors()
//{
//    for (int i = 0; i < 16; i++) {
//        if (mainFrame->IsQuestionAnswered(i)) {
//            // Зеленый цвет для отвеченных вопросов
//            cellButtons[i]->SetBackgroundColour(wxColour(67, 160, 71));
//            cellButtons[i]->SetForegroundColour(*wxWHITE);
//            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1)); // Добавляем галочку
//        }
//        else {
//            // Синий цвет для неотвеченных вопросов
//            cellButtons[i]->SetBackgroundColour(wxColour(100, 150, 200));
//            cellButtons[i]->SetForegroundColour(*wxWHITE);
//            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1));
//        }
//        // Принудительно обновляем внешний вид кнопки
//        cellButtons[i]->Refresh();
//    }
//}
//
////void GameBoardPanel::UpdateButtonColors()
////{
////    int answered = mainFrame->GetCorrectAnswersCount();
////
////    for (int i = 0; i < 16; i++) {
////        if (mainFrame->IsQuestionAnswered(i)) {
////            // "Прокачка" цвета по мере прогресса
////            if (answered <= 4) {
////                cellButtons[i]->SetBackgroundColour(wxColour(129, 199, 132)); // Новичок - светло-зеленый
////            }
////            else if (answered <= 8) {
////                cellButtons[i]->SetBackgroundColour(wxColour(56, 142, 60));   // Ученик - зеленый
////            }
////            else if (answered <= 12) {
////                cellButtons[i]->SetBackgroundColour(wxColour(255, 152, 0));   // Мастер - оранжевый
////            }
////            else {
////                cellButtons[i]->SetBackgroundColour(wxColour(233, 30, 99));   // Гуру - розовый
////            }
////            cellButtons[i]->SetForegroundColour(wxColour(255, 255, 255));
////        }
////        else {
////            // Неотвеченные - серые "заблокированные"
////            cellButtons[i]->SetBackgroundColour(wxColour(156, 39, 176));
////            cellButtons[i]->SetForegroundColour(*wxWHITE);
////            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1)); // С замком
////        }
////        cellButtons[i]->Refresh();
////    }
////}
//
////void GameBoardPanel::UpdateButtonColors()
////{
////    for (int i = 0; i < 16; i++) {
////        if (mainFrame->IsQuestionAnswered(i)) {
////            // Отвеченные - цвета математических констант
////            wxColour mathColors[] = {
////                wxColour(230, 74, 25),   // Пи - оранжево-красный
////                wxColour(155, 81, 224),  // e - фиолетовый
////                wxColour(76, 175, 80),   // Золотое сечение - зеленый
////                wxColour(3, 169, 244),   // Мнимая единица - голубой
////                wxColour(255, 193, 7)    // Бесконечность - золотой
////            };
////            cellButtons[i]->SetBackgroundColour(mathColors[i % 5]);
////            cellButtons[i]->SetForegroundColour(wxColour(255, 255, 255));
////            cellButtons[i]->SetLabel(wxString::Format("%d", i + 1)); // С символом пи
////        }
////        else {
////            // Неотвеченные - серые как нерешенные задачи
////            cellButtons[i]->SetBackgroundColour(wxColour(189, 189, 189));
////            cellButtons[i]->SetForegroundColour(wxColour(66, 66, 66));
////            cellButtons[i]->SetLabel(wxString::Format("? %d", i + 1));
////        }
////        cellButtons[i]->Refresh();
////    }
////}
//
//void GameBoardPanel::PositionButtonsInInfinityShape()
//{
//    wxSize containerSize = GetClientSize();
//    int centerX = containerSize.GetWidth() / 2;
//    int centerY = containerSize.GetHeight() / 2;
//    int radius = std::min(centerX, centerY) * 0.9;
//
//    // Параметрическое уравнение лемнискаты Бернулли (символ бесконечности)
//    for (int i = 0; i < 16; i++) {
//        double t = (2.0 * M_PI * i) / 16.0;
//
//        // Уравнение лемнискаты Бернулли
//        double scale = 1.0 + sin(t) * sin(t);
//        double x = radius * cos(t) / scale;
//        double y = radius * sin(t) * cos(t) / scale;
//
//        // Смещаем и масштабируем
//        int posX = centerX + static_cast<int>(x);
//        int posY = centerY + static_cast<int>(y);
//
//        cellButtons[i]->SetPosition(wxPoint(posX - 40, posY - 40));
//    }
//}
//
//void GameBoardPanel::OnSize(wxSizeEvent& event)
//{
//    PositionButtonsInInfinityShape();
//    Refresh();
//    event.Skip();
//}
//
//void GameBoardPanel::OnCellClick(wxCommandEvent& event)
//{
//    for (int i = 0; i < 16; i++) {
//        if (event.GetEventObject() == cellButtons[i]) {
//            int questionIndex = i % mainFrame->questions.size();
//            mainFrame->ShowQuestion(questionIndex);
//            break;
//        }
//    }
//}

#include "GameBoardPanel.h"
#include "MainFrame.h"
#include <cmath>
#include "TransparentPanel.h"
#include "MyButton.h"
#include <wx/sizer.h>
#include <wx/mediactrl.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/log.h>

// ID для событий медиа
enum {
    ID_MEDIA_CTRL = 10002
};

wxBEGIN_EVENT_TABLE(VideoPanel, wxPanel)
EVT_BUTTON(wxID_ANY, VideoPanel::OnClose)
EVT_MEDIA_LOADED(ID_MEDIA_CTRL, VideoPanel::OnMediaLoaded)
EVT_MEDIA_FINISHED(ID_MEDIA_CTRL, VideoPanel::OnMediaFinished)
EVT_MEDIA_STOP(ID_MEDIA_CTRL, VideoPanel::OnMediaFinished)
EVT_MEDIA_PLAY(ID_MEDIA_CTRL, VideoPanel::OnMediaLoaded)
EVT_SIZE(VideoPanel::OnSize)
wxEND_EVENT_TABLE()

VideoPanel::VideoPanel(wxWindow* parent, const wxString& videoFile)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE),
    m_loaded(false)
{
    SetBackgroundColour(wxColour(0, 0, 0));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Создаем медиа-контрол
    mediaCtrl = new wxMediaCtrl();

    // Пробуем разные бэкенды в зависимости от платформы
    bool created = false;

#ifdef __WXMSW__
    // Для Windows пробуем WMP, затем DirectShow
    created = mediaCtrl->Create(this, ID_MEDIA_CTRL, wxEmptyString,
        wxDefaultPosition, wxSize(800, 600),
        0, wxMEDIABACKEND_WMP10);
    if (!created) {
        created = mediaCtrl->Create(this, ID_MEDIA_CTRL, wxEmptyString,
            wxDefaultPosition, wxSize(800, 600),
            0, wxMEDIABACKEND_DIRECTSHOW);
    }
#elif defined(__WXGTK__)
    // Для Linux пробуем GStreamer
    created = mediaCtrl->Create(this, ID_MEDIA_CTRL, wxEmptyString,
        wxDefaultPosition, wxSize(800, 600),
        0, wxMEDIABACKEND_GSTREAMER);
#elif defined(__WXMAC__)
    // Для Mac пробуем QuickTime
    created = mediaCtrl->Create(this, ID_MEDIA_CTRL, wxEmptyString,
        wxDefaultPosition, wxSize(800, 600),
        0, wxMEDIABACKEND_QUICKTIME);
#endif

    // Если не удалось создать с конкретным бэкендом, пробуем по умолчанию
    if (!created) {
        created = mediaCtrl->Create(this, ID_MEDIA_CTRL, wxEmptyString,
            wxDefaultPosition, wxSize(800, 600));
    }

    if (!created) {
        wxLogError("Не удалось создать медиа-контрол");
        mediaCtrl = nullptr;
    }

    // Текст статуса
    statusText = new wxStaticText(this, wxID_ANY, "Загрузка видео...");
    statusText->SetForegroundColour(*wxWHITE);
    statusText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Кнопка закрытия
    closeButton = new wxButton(this, wxID_ANY, "Закрыть видео",
        wxDefaultPosition, wxSize(120, 40));
    closeButton->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    closeButton->SetBackgroundColour(wxColour(200, 50, 50));
    closeButton->SetForegroundColour(*wxWHITE);

    if (mediaCtrl) {
        mainSizer->Add(mediaCtrl, 1, wxEXPAND | wxALL, 10);
    }
    mainSizer->Add(statusText, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    mainSizer->Add(closeButton, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    SetSizer(mainSizer);

    // Загружаем видео
    if (mediaCtrl && !videoFile.empty()) {
        LoadVideo(videoFile);
    }
}

VideoPanel::~VideoPanel()
{
    if (mediaCtrl) {
        mediaCtrl->Stop();
    }
}

bool VideoPanel::LoadVideo(const wxString& videoFile)
{
    if (!mediaCtrl) {
        statusText->SetLabel("Ошибка: медиа-контрол не создан");
        return false;
    }

    if (!wxFileExists(videoFile)) {
        statusText->SetLabel("Ошибка: видеофайл не найден");
        return false;
    }

    statusText->SetLabel("Загрузка видео...");

    // Пытаемся загрузить видео
    if (mediaCtrl->Load(videoFile)) {
        m_loaded = true;
        statusText->SetLabel("Видео загружено. Нажмите Play для воспроизведения.");

        // Устанавливаем обработчики событий
        mediaCtrl->Bind(wxEVT_MEDIA_LOADED, &VideoPanel::OnMediaLoaded, this, ID_MEDIA_CTRL);
        mediaCtrl->Bind(wxEVT_MEDIA_FINISHED, &VideoPanel::OnMediaFinished, this, ID_MEDIA_CTRL);

        return true;
    }
    else {
        m_loaded = false;
        statusText->SetLabel("Ошибка загрузки видео");

        // Пробуем альтернативный метод загрузки
        if (mediaCtrl->LoadURI(videoFile)) {
            m_loaded = true;
            statusText->SetLabel("Видео загружено через URI");
            return true;
        }

        return false;
    }
}

void VideoPanel::Play()
{
    if (m_loaded && mediaCtrl) {
        mediaCtrl->SetVolume(1);
        mediaCtrl->Play();
        statusText->SetLabel("Воспроизведение...");
    }
}

void VideoPanel::Stop()
{
    if (mediaCtrl) {
        mediaCtrl->Stop();
        statusText->SetLabel("Воспроизведение остановлено");
    }
}

void VideoPanel::OnClose(wxCommandEvent& event)
{
    Stop();
    wxCommandEvent closeEvent(wxEVT_BUTTON, wxID_CLOSE);
    closeEvent.SetEventObject(this);
    GetParent()->ProcessWindowEvent(closeEvent);
}

void VideoPanel::OnMediaLoaded(wxMediaEvent& event)
{
    m_loaded = true;
    statusText->SetLabel("Видео готово к воспроизведению");

    // Автоматически запускаем воспроизведение после загрузки
    if (mediaCtrl) {
        mediaCtrl->Play();
        statusText->SetLabel("Воспроизведение...");
    }
}

void VideoPanel::OnMediaFinished(wxMediaEvent& event)
{
    statusText->SetLabel("Воспроизведение завершено");

    // Перематываем в начало для возможного повторного воспроизведения
    if (mediaCtrl) {
        mediaCtrl->Seek(0);
    }
}

void VideoPanel::OnMediaError(wxMediaEvent& event)
{
    statusText->SetLabel("Ошибка воспроизведения видео");
    m_loaded = false;
}

void VideoPanel::OnSize(wxSizeEvent& event)
{
    // При изменении размера обновляем размер медиа-контрола
    if (mediaCtrl) {
        wxSize size = GetClientSize();
        mediaCtrl->SetSize(size.GetWidth() - 20, size.GetHeight() - 100);
    }
    event.Skip();
}

wxBEGIN_EVENT_TABLE(GameBoardPanel, wxPanel)
EVT_SIZE(GameBoardPanel::OnSize)
EVT_BUTTON(wxID_ANY, GameBoardPanel::OnCellClick)
wxEND_EVENT_TABLE()

GameBoardPanel::GameBoardPanel(wxWindow* parent, MainFrame* mainFrame)
    : ImagePanel(parent, L""), mainFrame(mainFrame), isVideoPanelShown(false), videoPanel(nullptr), question_5_answered(false)
{

    wxSize sizeImg = mainFrame->GetSize();
    this->SetSize(sizeImg);
    this->LoadFromFile(L"background.jpg");

    if (!m_hasBackground) {
        SetBackgroundColour(wxColour(230, 230, 250));
    }

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Создаем контейнер для кнопок
    MyTransparentPanel* buttonContainer = new MyTransparentPanel(this);
    buttonContainer->SetMinSize(wxSize(700, 500));

    mainSizer->Add(buttonContainer, 1, wxEXPAND | wxALL, 20);

    // Создаем sizer для нижних кнопок
    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

    int count = mainFrame->GetCountQuestionAnswere();
    question_5_answered = mainFrame->IsQuestion5Answered();

    // Создаем кнопку "Природа" слева
    natureButton = new wxButton(this, wxID_ANY, "Природа",
        wxDefaultPosition, wxSize(150, 60));
    //if (count < 16)
    //{
    //    natureButton->Disable();
    //}
    natureButton->Bind(wxEVT_BUTTON, &GameBoardPanel::OnNatureButtonClick, this);

    // Создаем кнопку "Жизнь" справа  
    lifeButton = new wxButton(this, wxID_ANY, "Жизнь",
        wxDefaultPosition, wxSize(150, 60));
    //if (count < 16)
    //{
    //    lifeButton->Disable();
    //}
    lifeButton->Bind(wxEVT_BUTTON, &GameBoardPanel::OnLifeButtonClick, this);

    // Добавляем кнопки в bottomSizer с отступами
    bottomSizer->Add(natureButton, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 20);
    bottomSizer->AddStretchSpacer(1);
    bottomSizer->Add(lifeButton, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 20);

    mainSizer->Add(bottomSizer, 0, wxEXPAND);

    SetSizer(mainSizer);

    // Создаем 16 кнопок
    for (int i = 0; i < 16; i++) 
    {
        if (question_5_answered == true && i == 4)
        {

        }
        else
        {
            cellButtons[i] = new wxButton(buttonContainer, wxID_ANY,
                wxString::Format("%d", i + 1),
                wxDefaultPosition, wxSize(80, 80));
            cellButtons[i]->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT,
                wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            cellButtons[i]->Bind(wxEVT_BUTTON, &GameBoardPanel::OnCellClick, this);
        }
    }

    UpdateButtonColors();
    PositionButtonsInInfinityShape();
}

void GameBoardPanel::UpdateButtonColors()
{
    for (int i = 0; i < 16; i++) 
    {
        if (question_5_answered == true && i == 4)
        {

        }
        else
        {
            if (mainFrame->IsQuestionAnswered(i)) {
                cellButtons[i]->SetBackgroundColour(wxColour(67, 160, 71));
                cellButtons[i]->SetForegroundColour(*wxWHITE);
                cellButtons[i]->SetLabel(wxString::Format("%d", i + 1));
            }
            else {
                cellButtons[i]->SetBackgroundColour(wxColour(100, 150, 200));
                cellButtons[i]->SetForegroundColour(*wxWHITE);
                cellButtons[i]->SetLabel(wxString::Format("%d", i + 1));
            }
            cellButtons[i]->Refresh();
        }
    }
}

void GameBoardPanel::PositionButtonsInInfinityShape()
{
    wxSize containerSize = GetClientSize();
    int centerX = containerSize.GetWidth() / 2;
    int centerY = containerSize.GetHeight() / 2;
    int radius = std::min(centerX, centerY) * 0.9;

    for (int i = 0; i < 16; i++) 
    {
        if (question_5_answered == true && i == 4)
        {

        }
        else
        {
            double t = (2.0 * M_PI * i) / 16.0;
            double scale = 1.0 + sin(t) * sin(t);
            double x = radius * cos(t) / scale;
            double y = radius * sin(t) * cos(t) / scale;

            int posX = centerX + static_cast<int>(x);
            int posY = centerY + static_cast<int>(y);

            cellButtons[i]->SetPosition(wxPoint(posX - 40, posY - 40));
        }
    }
}

void GameBoardPanel::OnSize(wxSizeEvent& event)
{
    PositionButtonsInInfinityShape();

    // Обновляем размер videoPanel если он показан
    if (isVideoPanelShown && videoPanel) {
        videoPanel->SetSize(GetClientSize());
    }

    Refresh();
    event.Skip();
}

void GameBoardPanel::OnCellClick(wxCommandEvent& event)
{
    for (int i = 0; i < 16; i++) 
    {
        if (question_5_answered == true && i == 4)
        {

        }
        else
        {
            if (event.GetEventObject() == cellButtons[i]) {
                int questionIndex = i % mainFrame->questions.size();
                mainFrame->ShowQuestion(questionIndex);
                break;
            }
        }
    }
}
void GameBoardPanel::OnNatureButtonClick(wxCommandEvent& event)
{
    // Используем абсолютные пути для надежности
    wxString videoPath = wxFileName::GetCwd() + wxFILE_SEP_PATH + "video.mp4";
    if (wxFileExists(videoPath)) {
        ShowVideoPanel(videoPath);
    }
    else {
        // Пробуем относительный путь
        //videoPath = "nature_video.mp4";
        videoPath = "video.mp4";
        if (wxFileExists(videoPath)) {
            ShowVideoPanel(videoPath);
        }
        else {
            wxMessageBox("Видеофайл 'video.mp4' не найден.\nПоместите файл в папку с приложением.",
                "Файл не найден", wxOK | wxICON_ERROR);
        }
    }
}

void GameBoardPanel::OnLifeButtonClick(wxCommandEvent& event)
{
    // Используем абсолютные пути для надежности
    wxString videoPath = wxFileName::GetCwd() + wxFILE_SEP_PATH + "life_video.mp4";
    if (wxFileExists(videoPath)) {
        ShowVideoPanel(videoPath);
    }
    else {
        // Пробуем относительный путь
        videoPath = "life_video.mp4";
        if (wxFileExists(videoPath)) {
            ShowVideoPanel(videoPath);
        }
        else {
            wxMessageBox("Видеофайл 'life_video.mp4' не найден.\nПоместите файл в папку с приложением.",
                "Файл не найден", wxOK | wxICON_ERROR);
        }
    }
}

void GameBoardPanel::ShowVideoPanel(const wxString& videoFile)
{
    if (isVideoPanelShown) return;

    // Скрываем основные элементы
    GetSizer()->Show(false);

    // Создаем и показываем панель с видео
    videoPanel = new VideoPanel(this, videoFile);
    videoPanel->SetSize(GetClientSize());
    videoPanel->Show();

    // Привязываем событие закрытия
    videoPanel->Bind(wxEVT_BUTTON, &GameBoardPanel::OnCloseVideoPanel, this);

    isVideoPanelShown = true;
    Layout();
}

void GameBoardPanel::HideVideoPanel()
{
    if (!isVideoPanelShown) return;

    if (videoPanel) {
        videoPanel->Destroy();
        videoPanel = nullptr;
    }

    // Показываем основные элементы
    GetSizer()->Show(true);

    isVideoPanelShown = false;
    Layout();
    Refresh();
}

void GameBoardPanel::OnCloseVideoPanel(wxCommandEvent& event)
{
    HideVideoPanel();
}