#pragma once
#include <wx/wx.h>

class TransparentImagePanel : public wxPanel
{
public:
    TransparentImagePanel(wxWindow* parent,
        const wxString& imagePath = "",
        bool transparent = true);

    void SetBackgroundImage(const wxString& imagePath);
    void SetTransparent(bool transparent);
    bool HasBackground() const { return m_hasBackground; }

private:
    wxBitmap m_backgroundBitmap;
    bool m_hasBackground;
    bool m_isTransparent;

    void OnPaint(wxPaintEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnSize(wxSizeEvent& event);

#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

    wxDECLARE_EVENT_TABLE();
};