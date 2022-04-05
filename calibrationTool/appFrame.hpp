#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class AppFrame : public wxFrame {

    public:
        AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    private:
        void OnExit(wxCommandEvent& evt);
        void OnHelp(wxCommandEvent& evt);
        void OnPerspectiveSelection(wxCommandEvent& evt);
        void OnSphericalSelection(wxCommandEvent& evt);

        wxDECLARE_EVENT_TABLE();
};

enum buttons_id {
    ID_EXIT = 0,
    ID_HELP,
    ID_PERSPECTIVE,
    ID_SPHERICAL
};
