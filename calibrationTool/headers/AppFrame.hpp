#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"

class AppFrame : public wxFrame {
    // Attributes
    std::vector<wxButton*> buttons;
    wxBoxSizer* vbox;
    wxPanel *panel;
    Calib dataCalib;

    public:
        static const int INITIAL_BTNS_NB = 4;
        static const int PERSPECTIVE_BTNS_NB = 8;

        AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);

    private:
        // Called when a user clicks on the "Exit" labeled button
        void OnExit(wxCommandEvent& evt);

        // Called when a user clicks on the "Help" labeled button
        void OnHelp(wxCommandEvent& evt);

        // Called when a user clicks on the "Perspective camera" labeled button
        void OnPerspectiveSelection(wxCommandEvent& evt);

        // Called when a user clicks on the "Spherical camera" labeled button
        void OnSphericalSelection(wxCommandEvent& evt);

        // Called when a user clicks on the "Load images" labeled button
        void OnLoadImages(wxCommandEvent& evt);

        /**
        * Reset the attribute buttons and destroys every button displayed on the frame. 
        * 
        */
        void resetButtons();

        /**
         * Resize the buttons contained in the attribute buttons to match the largest
         * and display them in vbox. Puts some space depending on spacing under each 
         * button except the last one.
         * 
         * @param spacing the space to put between two buttons
         */
        void placeButtons(int spacing);

        wxDECLARE_EVENT_TABLE();
};
