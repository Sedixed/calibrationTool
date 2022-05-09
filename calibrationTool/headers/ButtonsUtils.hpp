#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// ============================================================================
// ------------------- Contains Btn namespace declaration.  -------------------
// ============================================================================



// ============================================================================
// ----------------------------------------------------------------------------
// ------------------------------ Btn namespace -------------------------------
// ----------------------------------------------------------------------------
// @Desc : Btn namespace is used for storing buttons IDs, labels, and some
//  utilitaries functions to retrieve one or more of these buttons.
//  It especially contains : 
//      - IDs of the application's buttons and their labels
//      - Functions to retrieve a vector of buttons
//      - Function to get a button from its ID
// ============================================================================
namespace Btn {

    // IDs of the differents buttons of the app
    enum ButtonsId : int {
        ID_PERSPECTIVE = 0,
        ID_SPHERICAL,
        ID_HELP,
        ID_EXIT,
        ID_LOAD_IMG,
        ID_EXTRACT_GRID_CORNERS,
        ID_CALIB,
        ID_SHOW_CORNERS_PROJ,
        ID_CALIB_RESULTS,
        ID_SAVE,
        ID_LOAD_FILE,
        ID_PREFERENCES,
        ID_EXIT_OK,
        ID_EXIT_CANCEL
    };


    // Labels associated to the buttons of the app
    const std::string mainLabels[] = {
        "Perspective camera",
        "Spherical camera",
        "Help",
        "Exit",
        "Load images",
        "Extract grid corners",
        "Calibration",
        "Show corners projection",
        "Calibration results",
        "Save",
        "Load YML file",
        "Preferences",
        "OK",
        "Cancel"
    };


    /**
     * Returns a vector of pointers of wxButtons that are the base buttons of the main frame.
     * 
     * @param parent                        the wxWindow parent of the buttons.
     * @return const std::vector<wxButton*> vector of pointers of wxButtons, base buttons of the main frame.
     */
    const std::vector<wxButton*> baseButtons(wxWindow* parent);


    /**
     * Returns a vector of pointers of wxButtons that are the buttons of the main frame, when
     * the button "Perspective camera" has been clicked by the user.
     * 
     * @param parent                        the wxWindow parent of the buttons.
     * @return const std::vector<wxButton*> vector of pointers of wxButtons, buttons displayed
     *                                      when "Perspective camera" button is clicked.
     */
    const std::vector<wxButton*> perspectiveButtons(wxWindow* parent);


    /**
     * Returns a vector of pointers of wxButtons that are the buttons of the main frame, when
     * the button "Spherical camera" has been clicked by the user.
     * 
     * @param parent                        the wxWindow parent of the buttons.
     * @return const std::vector<wxButton*> vector of pointers of wxButtons, buttons displayed
     *                                      when "Spherical camera" button is clicked.
     */
    const std::vector<wxButton*> sphericalButtons(wxWindow* parent);


    /**
     * Returns a vector of pointers of wxButtons that are the buttons of the preferences frame.
     * 
     * @param parent                        the wxWindow parent of the buttons.
     * @return const sd::vector<wxButton*>  vector of pointers of wxButtons, buttons displayed
     *                                      on the preferences frame.
     */
    const std::vector<wxButton*> preferencesButtons(wxWindow* parent);


    /**
     * Returns a pointer of wxButton with an appropriate label depending on id.
     * This wxButton will be displayed on the wxWindow pointed by parent.
     * 
     * @param id            identifier of a wxButton, from ButtonsId.
     * @param parent        the wxWindow the wxButton result will be displayed on.
     * @return wxButton*    the wxButton resulting from the id given.
     */
    wxButton* btnFromId(ButtonsId id, wxWindow* parent);
}