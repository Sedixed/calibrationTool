#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"
#include "AbstractPreferences.hpp"
#include <wx/gbsizer.h>


// Base ID for checkBoxes
#define CHECKBOX_BASE_ID 60


// Used for storing perspective specific constants, enums and constant arrays.
namespace Pref {

    namespace Perspective {

        // IDs used by the different checkbox for parameters to estimate
        enum CheckBoxId : int {
            ID_FOCAL = CHECKBOX_BASE_ID,
            ID_POINT,
            ID_K1,
            ID_K2,
            ID_K3,
            ID_K4,
            ID_K5,
            ID_K6 // Not used at the moment
        };
    }

    // ID of the button used to set calibration method used
    const int CALIB_METHOD_ID = 100;
}

/**
 * Class for perspective preferences frame.
 * 
 */
class PreferencesPerspectiveFrame : public AbstractPreferences {
    // Attributes
    private:
        // 1 if we want to use the RO method, 0 otherwise
        int iFixedPoint;


    public:
        PreferencesPerspectiveFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib, AppFrame *parent);

    private:
        void OnExitOk(wxCommandEvent& evt);

        void UpdateFlags(wxCommandEvent& evt);

        void SetOkState(wxCommandEvent& evt);


        /**
         * Toggle the calibration method (basic / RO).
         * 
         * @param evt wxCommandEvent associated to the event.
         */
        void ToggleROMode(wxCommandEvent& evt);


        void CreateAndPlaceComponents();

        wxDECLARE_EVENT_TABLE();
};