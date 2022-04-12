#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"

namespace Pref {

    // Index of the default search size in the searchSizes array below
    const int DEFAULT_SEARCH_INDEX = 2;
    // Index of the default render size in the renderSizes array below
    const int DEFAULT_RENDER_INDEX = 2;
    // Default number of squares along X
    const int DEFAULT_NB_X = 7;
    // Default number of squares along Y
    const int DEFAULT_NB_Y = 5;
    // Default size of a square along X
    const double DEFAULT_SIZE_X = 20.;
    // Default size of a square along Y
    const double DEFAULT_SIZE_Y = 20.;

    // IDs of the differents labels of the Preferences frame
    enum LabelsId : int {
        ID_PARAMETERS = 0,
        ID_SEARCH_WINDOW_SIZE,
        ID_RENDER_WINDOW_SIZE,
        ID_CALIB_PROPERTIES
    };

    // Labels of the Preferences frame
    const std::string labels[] = {
        "Parameters to estimate :",
        "Size of the window for the corners detection :",
        "Render window size :",
        "Calibration pattern properties :"
    };

    // Size of the window for the corners detection
    const int searchSizes[] = {3, 5, 9, 15, 21};

    // ID's corresponding to the window sizes above
    const int searchId[] = {20, 21, 22, 23, 24};

    // Render window sizes
    const cv::Size renderSizes[] = {
        cv::Size(800, 600),
        cv::Size(1024, 768),
        cv::Size(1152, 864),
        cv::Size(1280, 1024)
    };

    //ID's corresponding to the render sizes above
    const int renderId[] = {30, 31, 32, 33};

}

class PreferencesFrame : public wxFrame {
    // Attributes
    wxPanel* panel;
    int searchWindowSize;
    cv::Size renderWindowSize;
    Calib* dataCalib;

    wxTextCtrl* nbX;
    wxTextCtrl* nbY;
    wxTextCtrl* sizeX;
    wxTextCtrl* sizeY;

    public:
        PreferencesFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib);

    private:
        // Called when a user click on the "Cancel" labeled button.
        void OnExitCancel(wxCommandEvent& evt);

        // Called when a user click on the "OK" labeled button.
        void OnExitOk(wxCommandEvent& evt);

        /**
         * Set the value of searchWindowSize with the value associated to
         * the wxRadioButton clicked whose ID equals evt.GetId().
         * 
         * @param evt wxCommandEvent associated to the event.
         *  
         */
        void SetLastSearch(wxCommandEvent& evt);

         /**
         * Set the value of renderWindowSize with the value associated to
         * the wxRadioButton clicked whose ID equals evt.GetId().
         * 
         * @param evt wxCommandEvent associated to the event.
         *  
         */
        void SetLastRender(wxCommandEvent& evt);

        /**
         * Create all the required components for the frame, and places them
         * on the frame.
         * 
         */
        void CreateAndPlaceComponents();

        wxDECLARE_EVENT_TABLE();
};