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


// Used for storing spherical specific constants, enums and constant arrays.
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


/**
 * Class for spherical preferences frame.
 * 
 */
class PreferencesSphericalFrame : public AbstractPreferences {
    // Attributes
    private:
        // Define if the user provides the skew
        bool ignoreSkew;
        // Define if the user provides the Xi parameter
        bool ignoreXi;

    public:
        PreferencesSphericalFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib, AppFrame *parent);

    private:
        void OnExitOk(wxCommandEvent& evt);

        void UpdateFlags(wxCommandEvent& evt);

        void SetOkState(wxCommandEvent& evt);

        void CreateAndPlaceComponents();

        wxDECLARE_EVENT_TABLE();
};