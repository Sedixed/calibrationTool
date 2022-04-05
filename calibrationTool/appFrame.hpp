#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class AppFrame : public wxFrame {
    // Attributes
    std::vector<wxButton*> buttons;
    wxBoxSizer* vbox;

    public:
        static const int INITIAL_BTNS_NB = 4;
        static const int PERSPECTIVE_BTNS_NB = 8;

        AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    private:
        // Called when a user clicks on the "Exit" labeled button
        void OnExit(wxCommandEvent& evt);
        // Called when a user clicks on the "Help" labeled button
        void OnHelp(wxCommandEvent& evt);
        // Called when a user clicks on the "Perspective camera" labeled button
        void OnPerspectiveSelection(wxCommandEvent& evt);
        // Called when a user clicks on the "Spherical camera" labeled button
        void OnSphericalSelection(wxCommandEvent& evt);

        wxDECLARE_EVENT_TABLE();
};
