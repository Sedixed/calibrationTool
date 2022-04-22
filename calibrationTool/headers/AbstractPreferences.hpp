#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"
#include "AppFrame.hpp"
#include <wx/gbsizer.h>


/**
 * Calculates the number of elements of an array. 
 * 
 * @tparam T    Type of elements in the array.
 * @tparam size Amount of elements of the array.
 * @return int  The amount of elements in the array.
 */
template<typename T, int size>
int arrLength(T(&)[size]);


// Base ID for search radioButtons
#define SEARCH_BASE_ID 20


// Used for storing commons constants, enums and constant arrays.
namespace Pref {

    // IDs of the differents labels of the Perspective preferences frame
    enum LabelsId : int {
        ID_PARAMETERS = 0,
        ID_SEARCH_WINDOW_SIZE,
        ID_RENDER_WINDOW_SIZE,
        ID_CALIB_PROPERTIES
    };

    // Labels of the Perspective preferences frame
    const std::string labels[] = {
        "Parameters to estimate :",
        "Size of the window for the corners detection :",
        "Render window size :",
        "Calibration pattern properties :"
    };

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


    // Minimal ID for searching radiobuttons
    const int SEARCH_MIN_ID = SEARCH_BASE_ID;

    // Size of the window for the corners detection
    const int searchSizes[] = {3, 5, 9, 15, 21};

    // IDs corresponding to the window sizes above
    const int searchId[] = {SEARCH_MIN_ID,
                            SEARCH_MIN_ID + 1,
                            SEARCH_MIN_ID + 2,
                            SEARCH_MIN_ID + 3, 
                            SEARCH_MIN_ID + 4
                           };

    // How many ID's there are for searching radiobuttons
    const int SEARCH_NB_ID = arrLength<const int>(searchId);

    // Render window sizes
    const cv::Size renderSizes[] = {
        cv::Size(800, 600),
        cv::Size(1024, 768),
        cv::Size(1152, 864),
        cv::Size(1280, 1024)
    };

    // Minimal ID for render radiobuttons
    const int RENDER_MIN_ID = SEARCH_MIN_ID + SEARCH_NB_ID + 1;

    //IDs corresponding to the render sizes above
    const int renderId[] = {RENDER_MIN_ID,
                            RENDER_MIN_ID + 1,
                            RENDER_MIN_ID + 2,
                            RENDER_MIN_ID + 3
                           };

    // How many ID's there are for render radiobuttons
    const int RENDER_NB_ID = arrLength<const int>(renderId);
}


/**
 * Generic abstract class for preferences frame.
 * 
 */
class AbstractPreferences : public wxFrame {
    // Attributes
    protected:
        // Define if the user provides the focal length
        bool ignoreFocal;
        // Define if the user provides the principal point
        bool ignorePoint;
        // The main panel of the frame
        wxPanel* panel;
        // The panel used for displaying intrinsics parameters
        wxPanel* parameters;
        // The Sizer used by parameters
        wxGridBagSizer *fgboxParameters;
        // The size of the currently selected search window size
        int searchWindowSize;
        // The size of the currently selected render window size
        cv::Size renderWindowSize;
        // The current flags for calibration
        int flags;
        // Calib structure associated to the running instance of the program using this frame
        Calib* dataCalib;

        // Text controllers for Mire input
        wxTextCtrl* nbX;
        wxTextCtrl* nbY;
        wxTextCtrl* sizeX;
        wxTextCtrl* sizeY;

    public:
        AbstractPreferences(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib, AppFrame *parent);

    protected:
        // Called when a user click on the "Cancel" labeled button.
        void OnExitCancel(wxCommandEvent& evt);


        // Called when a user click on the "OK" labeled button.
        virtual void OnExitOk(wxCommandEvent& evt) = 0;


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
         * Update the value of flags according to the checkbox which have been
         * clicked to toggle/untoggle the selected parameter.
         * 
         * @param evt wxCommandEvent associated to the event.
         */
        virtual void UpdateFlags(wxCommandEvent& evt) = 0;


        /**
         * Toggles the "Ok" button with conditions. If one of the active wxTextCtrl
         * is blank, button is not active, else it is.
         * 
         * @param evt wxCommandEvent associated to the event.
         */
        virtual void SetOkState(wxCommandEvent& evt) = 0;


        /**
         * Create all the required components for the frame, and places them
         * on the frame.
         * 
         */
        virtual void CreateAndPlaceComponents() = 0;
};