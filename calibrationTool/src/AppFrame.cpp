#include "../headers/AppFrame.hpp"
#include "../headers/ButtonsUtils.hpp"
#include "../headers/LoadImages.hpp"
#include "../headers/ExtractGridCorners.hpp"
#include "../headers/PreferencesPerspective.hpp"
#include "../headers/PreferencesSpherical.hpp"
#include "../headers/Calibration.hpp"
#include "../headers/ShowReprojection.hpp"
#include "../headers/CalibrationResults.hpp"
#include "../headers/Save.hpp"
#include "../headers/LoadFile.hpp"
#include <iostream>


// Spacing between two buttons in the base menu
#define BASE_SPACING 40

// Spacing between two buttons in the perspective menu
#define PERSPECTIVE_SPACING 30

// Spacing between two buttons in the spherical menu
#define SPHERICAL_SPACING 30


// Event table used by the frame.
//  Makes each button corresponds its onclick function
wxBEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT,                 AppFrame::OnExit)
    EVT_BUTTON(Btn::ID_HELP,                 AppFrame::OnHelp)
    EVT_BUTTON(Btn::ID_PERSPECTIVE,          AppFrame::OnPerspectiveSelection)
    EVT_BUTTON(Btn::ID_SPHERICAL,            AppFrame::OnSphericalSelection)
    EVT_BUTTON(Btn::ID_LOAD_IMG,             AppFrame::OnLoadImages)
    EVT_BUTTON(Btn::ID_EXTRACT_GRID_CORNERS, AppFrame::OnExtractGridCorners)
    EVT_BUTTON(Btn::ID_CALIB,                AppFrame::OnCalibration)
    EVT_BUTTON(Btn::ID_SHOW_CORNERS_PROJ,    AppFrame::OnShowReprojection)
    EVT_BUTTON(Btn::ID_CALIB_RESULTS,        AppFrame::OnCalibResults)
    EVT_BUTTON(Btn::ID_SAVE,                 AppFrame::OnSave)
    EVT_BUTTON(Btn::ID_LOAD_FILE,            AppFrame::OnLoadFile)
    EVT_BUTTON(Btn::ID_PREFERENCES,          AppFrame::OnPreferences)
wxEND_EVENT_TABLE()


/**
 * Puts in retSize the size of the wxButton having the highest width.
 * 
 * @param btns      vector of pointers of wxButtons to iterate over.
 * @param retSize   the wxSize object that will contain the result value.
 */
void getMaxSizeOfBtns(std::vector<wxButton*>& btns, wxSize *retSize);


/**
 * Change the state of the buttons whose IDs are in ids.
 * 
 * @param ids   Vector of button IDs used to change its buttons state.
 * @param state true if we want the button enabled, false otherwise.
 */
void setButtonsState(std::vector<Btn::ButtonsId> ids, bool state);


AppFrame::AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(NULL, 0, title, pos, size, style) {
    SetDefaultPreferences();

    // A VOIR
    // peut être supprimer le active_image (refaire toutes les images donc, à voir)
    //on a le choix entre les coordonnées brutes en 3D et celles recalculées par RO (pareil)


    //  EN COURS
    // makefile à améliorer
    // voir si on peut ignorer des img (si pas détecté)


    // FAIT
    // +- fini fenêtre de préférences omni
    // windows ok
    // tests faits : high fonctionne, weak quasi pas, low 4/26 pas détecté


    panel = new wxPanel(this);
    buttons = Btn::baseButtons(panel);
    placeButtons(BASE_SPACING);
    Centre();
}


void AppFrame::OnExit(wxCommandEvent& evt) {
    cv::destroyAllWindows();
    Close(true);
}


void AppFrame::OnHelp(wxCommandEvent& evt) {
    wxLogMessage("Camera calibration tool. Please first select either spherical or perspective camera to start.");
}


void AppFrame::OnPerspectiveSelection(wxCommandEvent& evt) {
    resetButtons();
    dataCalib.type = PERSPECTIVE_TYPE;
    buttons = Btn::perspectiveButtons(panel);
    SetSize(wxSize(300, 650));
    SetTitle("Perspective calibration");
    // Disabling buttons 
    setButtonsState(std::vector<Btn::ButtonsId> {
        Btn::ID_EXTRACT_GRID_CORNERS,
        Btn::ID_CALIB, 
        Btn::ID_SHOW_CORNERS_PROJ, 
        Btn::ID_CALIB_RESULTS,
        Btn::ID_SAVE}, false
    );
    placeButtons(PERSPECTIVE_SPACING);
}


void AppFrame::OnSphericalSelection(wxCommandEvent& evt) {
    resetButtons();
    dataCalib.type = SPHERICAL_TYPE;
    buttons = Btn::sphericalButtons(panel);
    SetSize(wxSize(300, 650));
    SetTitle("Spherical calibration");
    // Disabling buttons 
    setButtonsState(std::vector<Btn::ButtonsId> {
        Btn::ID_EXTRACT_GRID_CORNERS,
        Btn::ID_CALIB, 
        Btn::ID_SHOW_CORNERS_PROJ, 
        Btn::ID_CALIB_RESULTS,
        Btn::ID_SAVE}, false
    );
    placeButtons(SPHERICAL_SPACING);
}


void AppFrame::OnLoadImages(wxCommandEvent& evt) {
    int r = LoadImages(&dataCalib, this);
    if (r == 0) {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_EXTRACT_GRID_CORNERS,
            Btn::ID_PREFERENCES}, true
        );
        // In case it is not the first calibration during execution
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_CALIB, 
            Btn::ID_SHOW_CORNERS_PROJ, 
            Btn::ID_CALIB_RESULTS,
            Btn::ID_SAVE}, false
        );
    }
}


void AppFrame::OnExtractGridCorners(wxCommandEvent& evt) {
    try {
    int r = ExtractGridCorners(&dataCalib);
    if (r == 0) {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_CALIB}, true
        );
    } else {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_CALIB, 
            Btn::ID_SHOW_CORNERS_PROJ, 
            Btn::ID_CALIB_RESULTS,
            Btn::ID_SAVE}, false
        );
    }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}


void AppFrame::OnCalibration(wxCommandEvent& evt) {
    int r = Calibration(&dataCalib);
    if (r == 0) {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_SHOW_CORNERS_PROJ,
            Btn::ID_CALIB_RESULTS}, true
        );
    }
}


void AppFrame::OnShowReprojection(wxCommandEvent& evt) {
    int r = ShowReprojection(&dataCalib, this);
    if (r == 0) {
        //
    }
}


void AppFrame::OnCalibResults(wxCommandEvent& evt) {
    int r = CalibrationResults(&dataCalib);
    if (r == 0) {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_SAVE}, true
        );
    }
}


void AppFrame::OnSave(wxCommandEvent& evt) {
    Save(&dataCalib);
}


void AppFrame::OnLoadFile(wxCommandEvent& evt) {
    int r = LoadFile(&dataCalib);
    if (r == 0) {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_EXTRACT_GRID_CORNERS,
            Btn::ID_CALIB,
            Btn::ID_SHOW_CORNERS_PROJ,
            Btn::ID_CALIB_RESULTS,
            Btn::ID_SAVE,
            Btn::ID_PREFERENCES}, true
        );
    }
}


void AppFrame::OnPreferences(wxCommandEvent& evt) {
    long style = wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX);
    switch(dataCalib.type) {
        case PERSPECTIVE_TYPE:
        {      
            PreferencesPerspectiveFrame* frame;
            if (dataCalib.nb_images > 0) {
                frame = new PreferencesPerspectiveFrame("Preferences", wxPoint(-1, -1), 
                    wxSize(550, 710), style, &dataCalib, this, true);
            } else {
                frame = new PreferencesPerspectiveFrame("Preferences", wxPoint(-1, -1), 
                    wxSize(550, 710), style, &dataCalib, this, false);
            }
            frame->Show(true);
            break;
        }
        case SPHERICAL_TYPE:
        {   
            PreferencesSphericalFrame* frame;
            if (dataCalib.nb_images > 0) {
                frame = new PreferencesSphericalFrame("Preferences", wxPoint(-1, -1), 
                    wxSize(550, 720), style, &dataCalib, this, true);
            } else {
                frame = new PreferencesSphericalFrame("Preferences", wxPoint(-1, -1), 
                    wxSize(550, 720), style, &dataCalib, this, false);
            }
            frame->Show(true);
            break;
        }
        default:
        {
            std::cout << "Unknown type\n";
        }
    }
    
}


void AppFrame::resetButtons() {
    for (wxButton *b : buttons) {
        vbox->Detach(b);
        b->Destroy();
    }
}


void AppFrame::placeButtons(int spacing) {
    // Get the max size for buttons
    wxSize* actSize = new wxSize();
    getMaxSizeOfBtns(buttons, actSize);

    vbox = new wxBoxSizer(wxVERTICAL);

    // Placing buttons and spaces
    vbox->AddStretchSpacer(1);
    for (wxButton* btn : buttons) {
        btn->SetSize(actSize->GetWidth(), btn->GetSize().GetHeight());
        vbox->Add(btn, 0, wxEXPAND | wxLEFT | wxRIGHT, 60);
        if (btn != buttons.back()) {
            vbox->AddSpacer(spacing);
        }
    }
    
    // Refreshing
    vbox->AddStretchSpacer(1);
    vbox->Layout();
    panel->SetSizer(vbox);
    panel->Layout();
}


void AppFrame::SetDefaultPreferences() {
    dataCalib.pref.render_size = Pref::renderSizes[Pref::DEFAULT_RENDER_INDEX]; 
    dataCalib.calibPattern.nbSquareX = Pref::DEFAULT_NB_X;
    dataCalib.calibPattern.nbSquareY = Pref::DEFAULT_NB_Y;
    dataCalib.calibPattern.sizeSquareX = Pref::DEFAULT_SIZE_X;
    dataCalib.calibPattern.sizeSquareY = Pref::DEFAULT_SIZE_Y;
    dataCalib.pref.search_window_size = Pref::searchSizes[Pref::DEFAULT_SEARCH_INDEX];
    dataCalib.pref.parameters_flags = 0;
    dataCalib.intrinsics = cv::Mat::zeros(3, 3, CV_64F);
    dataCalib.intrinsics.at<double>(2,2) = 1;
    dataCalib.pref.fixed_point = 0;
}


// --- Utils ---


void getMaxSizeOfBtns(std::vector<wxButton*>& btns, wxSize* retSize) {
    *retSize = btns.at(0)->GetSize();
    for (int i = 1; i < btns.size(); ++i) {
        if (retSize->GetWidth() < btns.at(i)->GetSize().GetWidth()) {
            *retSize = btns.at(i)->GetSize();
        }
    }
}


void setButtonsState(std::vector<Btn::ButtonsId> ids, bool state) {
    for (Btn::ButtonsId id : ids) {
        wxButton* b = (wxButton*) wxWindow::FindWindowById(id);
        b->Enable(state);
    }
}