#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"
#include "AbstractPreferences.hpp"
#include <wx/gbsizer.h>


// ============================================================================
// ---- Contains PreferencesPerspectiveFrame class declaration and Pref    ----
// ---- namespace completion. It also contains the base ID used by         ----
// ---- checkBoxes of the frame.                                           ----
// ============================================================================


// Base ID for checkBoxes
#define CHECKBOX_BASE_ID 60

// Focal length input IDs
#define GU 80               // ID of the Gu wxTextCtrl
#define GV 81               // ID of the Gv wxTextCtrl


// ============================================================================
// ----------------------------------------------------------------------------
// ------------------------------ Pref namespace ------------------------------
// ----------------------------------------------------------------------------
// @Desc : Completes the definition of the Pref namespace in the 
//  AbstractPreferences header. It adds the followings :
//      - an inner namespace called Perspective that contains the IDs 
//          for the checkBoxes of the frame
//      - the calibration method's button ID
// ============================================================================
namespace Pref {

    namespace Perspective {

        // IDs used by the different checkbox for parameters to estimate
        enum CheckBoxId : int {
            ID_FOCAL = CHECKBOX_BASE_ID,
            ID_POINT,
            ID_K1,
            ID_K2,
            ID_P1P2,
            ID_K3
        };
    }

    // ID of the button used to set calibration method used
    const int CALIB_METHOD_ID = 100;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ------------- PreferencesPerspectiveFrame class declaration ----------------
// ----------------------------------------------------------------------------
// @Desc : PreferencesPerspectiveFrame is a derived class of 
//  AbstractPreferences for setting preferences of the current perspective 
//  calibration. It contains the definition of the abstract methods of its
//  super class, and some methods that are proper to the perspective
//  calibration settings.
// ----------------------------------------------------------------------------
// @Cons :
//      - title      : Title of the frame
//      - pos        : Default position of the frame on the screen
//      - size       : Default size of the frame
//      - style      : Flags used to define frame's behaviour
//      - calib      : Pointer of Calib used for storing settings
//      - parent     : Pointer of AppFrame used to close the window if the
//                      AppFrame one is closed
//      - allEnabled : True if the user has loaded images, false otherwise
// ============================================================================
class PreferencesPerspectiveFrame : public AbstractPreferences {
    // Attributes
    private:
        // 1 if we want to use the RO method, 0 otherwise
        int iFixedPoint;
        // Define if the user provides the focal length
        bool ignoreFocal;

    public:
        PreferencesPerspectiveFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib,
            AppFrame *parent, bool allEnabled);

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