#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"

namespace Pref {

    // IDs of the differents labels of the Preferences frame
    enum LabelsId : int {
        ID_SEARCH_WINDOW_SIZE = 0,
        ID_RENDER_WINDOW_SIZE,
        ID_CALIB_PROPERTIES
    };

    // Labels of the Preferences frame
    const std::string labels[] = {
        "Size of the window for the corners detection :",
        "Render window size",
        "Calibration pattern properties"
    };

    // Size of the window for the corners detection
    const int searchSizes[] = {3, 5, 9, 15, 21};

    // Render window sizes
    const cv::Size renderSizes[] = {
        cv::Size(800, 600),
        cv::Size(1024, 768),
        cv::Size(1152, 864),
        cv::Size(1280, 1024)
    };

}

class PreferencesFrame : public wxFrame {
    // Attributes
    wxPanel* panel;
    cv::Size searchWindowSize;
    cv::Size renderWindowSize;

    public:
        PreferencesFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);

    private:
        // Called when a user click on the "Cancel" labeled button
        void OnExitCancel(wxCommandEvent& evt);

        // Called when a user click on the "OK" labeled button
        void OnExitOk(wxCommandEvent& evt);

        /**
         * Create all the required components for the frame, and places them
         * on the frame.
         * 
         */
        void CreateAndPlaceComponents();


        wxDECLARE_EVENT_TABLE();


};