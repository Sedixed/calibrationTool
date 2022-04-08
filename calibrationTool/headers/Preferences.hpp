#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"



class PreferencesFrame : public wxFrame {
    public:
        PreferencesFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);

    private:
        // Called when a user click on the "Cancel" labeled button
        void OnExitCancel(wxCommandEvent& evt);

        // Called when a user click on the "OK" labeled button
        void OnExitOk(wxCommandEvent& evt);



        wxDECLARE_EVENT_TABLE();


};