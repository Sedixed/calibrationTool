#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"
#include "AbstractPreferences.hpp"
#include <wx/gbsizer.h>


// ============================================================================
// ---- Contains PreferencesSphericalFrame class declaration and Pref      ----
// ---- namespace completion. It also contains the base ID used by         ----
// ---- checkBoxes of the frame.                                           ----
// ============================================================================


// Base ID for checkBoxes
#define CHECKBOX_BASE_ID 60


// ============================================================================
// ----------------------------------------------------------------------------
// ------------------------------ Pref namespace ------------------------------
// ----------------------------------------------------------------------------
// @Desc : Completes the definition of the Pref namespace in the 
//  AbstractPreferences header. It adds the followings :
//      - an inner namespace called Spherical that contains the IDs 
//          for the checkBoxes of the frame
// ============================================================================
namespace Pref {

    namespace Spherical {

        // IDs used by the different checkbox for parameters to estimate
        enum CheckBoxId : int {
            ID_FOCAL = CHECKBOX_BASE_ID,
            ID_POINT,
            ID_SKEW,
            ID_XI,
            ID_K1,
            ID_K2,
            ID_K3,
            ID_K4,
            ID_K5,
            ID_K6 // Not used at the moment
        };
    }
}


// ============================================================================
// ----------------------------------------------------------------------------
// -------------- PreferencesSphericalFrame class declaration -----------------
// ----------------------------------------------------------------------------
// @Desc : PreferencesSphericalFrame is a derived class of 
//  AbstractPreferences for setting preferences of the current spherical 
//  calibration. It contains the definition of the abstract methods of its
//  super class.
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
class PreferencesSphericalFrame : public AbstractPreferences {
    // Attributes
    private:
        // Define if the user provides the skew
        bool ignoreSkew;
        // Define if the user provides the Xi parameter
        bool ignoreXi;

    public:
        PreferencesSphericalFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib,
            AppFrame *parent, bool allEnabled);

    private:
        void OnExitOk(wxCommandEvent& evt);

        void UpdateFlags(wxCommandEvent& evt);

        void SetOkState(wxCommandEvent& evt);

        void CreateAndPlaceComponents();

        wxDECLARE_EVENT_TABLE();
};