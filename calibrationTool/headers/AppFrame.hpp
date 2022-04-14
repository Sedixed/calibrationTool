#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"

class AppFrame : public wxFrame {
    // Attributes

    // Vector used for storing the buttons of the frame
    std::vector<wxButton*> buttons;
    // Sizer used inside panel
    wxBoxSizer* vbox;
    // Panel of the frame
    wxPanel *panel;
    // Calib structure associated to the running instance of the program using this frame
    Calib dataCalib;

    public:
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

        // Called when a user click on the "Extract grid corners" labeled button
        void OnExtractGridCorners(wxCommandEvent& evt);

        // Called when a user click on the "Calibration" labeled button
        void OnCalibration(wxCommandEvent& evt);

        // Called when a user click on the "Show corners projection" labeled button
        void OnShowReprojection(wxCommandEvent& evt);

        // Called when a user click on the "Calibration results" labeled button
        void OnCalibResults(wxCommandEvent& evt);

        // Called when a user click on the "Save" labeled button
        void OnSave(wxCommandEvent& evt);

        // Called when a user click on the "Load YML file" labeled button
        void OnLoadFile(wxCommandEvent& evt);

        // Called when a user click on the "Preferences" labeled button
        void OnPreferences(wxCommandEvent& evt);

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

        /**
         * Set the default values for the Preferences field associated to
         * the Calib attribute of the frame.
         * 
         */
        void SetDefaultPreferences();

        wxDECLARE_EVENT_TABLE();
};
