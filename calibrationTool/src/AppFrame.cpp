#include "../headers/AppFrame.hpp"
#include "../headers/ButtonsUtils.hpp"
#include "../headers/LoadImages.hpp"
#include "../headers/ExtractGridCorners.hpp"
#include "../headers/PreferencesPerspective.hpp"
#include "../headers/Calibration.hpp"
#include "../headers/ComputeViewsError.hpp"
#include "../headers/Save.hpp"
#include "../headers/LoadFile.hpp"
#include <iostream>

// Spacing between two buttons in the base menu
#define BASE_SPACING 40

// Spacing between two buttons in the perspective menu
#define PERSPECTIVE_SPACING 30

// Spacing between two buttons in the spherical menu
// à définir plus tard
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


AppFrame::AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(NULL, 0, title, pos, size, style) {
    
    SetDefaultPreferences();

    // UPDATE
    // On a le choix entre un vecteur dans calibrateCameraRO et les reprojections via projectCorners
    //  pour obtenir les erreurs par image : lequel est le meilleur ? jsp
    // si celui de la fonction, revoir tout le code de computeviewserror
    // De même, on a le choix entre les coordonnées brutes en 3D et celles recalculées par RO (pareil)

    // A VOIR
    // peut être supprimer le active_image (refaire toutes les images donc, à voir)

    // EN COURS
    // pb du flac CALIB_USE_INTRINSIC_GUESS : si point défini par user mais pas la focal length, problème

    //  EN COURS
    // adapter pour windows à un moment
    // installé mais linkage marche pas

    // commencé spherical : calib fix (pas de pb de taille de matrice mais de findchessboard,
    //      flag cb_fast à désactiver -> donne un résultat
    //      pas besoin de redimensionner en passant par des vecs chelou

    // IDEE
    // Ajouter btn pour fermer toutes les images opencv ouvertes
    
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
    // Disabling buttons 
    int borne =  Btn::ID_PREFERENCES + 1 - Btn::ID_EXTRACT_GRID_CORNERS + 1;
    // 1 because we keep the first button active
    for (int i = 1; i < borne; ++i) {
        if (i != Btn::ID_LOAD_FILE - Btn::ID_LOAD_IMG) {
            buttons[i]->Enable(false);
        }
    }

    placeButtons(PERSPECTIVE_SPACING);
}


void AppFrame::OnSphericalSelection(wxCommandEvent& evt) {
    resetButtons();
    dataCalib.type = SPHERICAL_TYPE;
    buttons = Btn::sphericalButtons(panel);
    SetSize(wxSize(300, 650));
    // Disabling buttons 
    int borne =  Btn::ID_PREFERENCES + 1 - Btn::ID_EXTRACT_GRID_CORNERS + 1;
    // 1 because we keep the first button active
    for (int i = 1; i < borne; ++i) {
        if (i != Btn::ID_LOAD_FILE - Btn::ID_LOAD_IMG) {
            buttons[i]->Enable(false);
        }
    }

    placeButtons(SPHERICAL_SPACING);
}


void AppFrame::OnLoadImages(wxCommandEvent& evt) {
    int r = LoadImages(&dataCalib, this);
    if (r == 0) {
        buttons[Btn::ID_EXTRACT_GRID_CORNERS - Btn::ID_LOAD_IMG]->Enable(true);
        buttons[Btn::ID_PREFERENCES - Btn::ID_LOAD_IMG]->Enable(true);
    }
}


void AppFrame::OnExtractGridCorners(wxCommandEvent& evt) {
    int r = ExtractGridCorners(&dataCalib);
    if (r == 0) {
        buttons[Btn::ID_CALIB - Btn::ID_LOAD_IMG]->Enable(true);
    }
}


void AppFrame::OnCalibration(wxCommandEvent& evt) {
    int r = Calibration(&dataCalib);
    if (r == 0) {
        buttons[Btn::ID_SHOW_CORNERS_PROJ - Btn::ID_LOAD_IMG]->Enable(true);
        buttons[Btn::ID_CALIB_RESULTS - Btn::ID_LOAD_IMG]->Enable(true);
    }
}


void AppFrame::OnShowReprojection(wxCommandEvent& evt) {
    ComputeViewsError(&dataCalib, this, 1);
}


void AppFrame::OnCalibResults(wxCommandEvent& evt) {
    int r = ComputeViewsError(&dataCalib);
    if (r == 0) {
        buttons[Btn::ID_SAVE - Btn::ID_LOAD_IMG]->Enable(true);
    }
}


void AppFrame::OnSave(wxCommandEvent& evt) {
    Save(&dataCalib);
}


void AppFrame::OnLoadFile(wxCommandEvent& evt) {
    int r = LoadFile(&dataCalib);
    if (r == 0) {
        buttons[Btn::ID_EXTRACT_GRID_CORNERS - Btn::ID_LOAD_IMG]->Enable(true);
        buttons[Btn::ID_CALIB - Btn::ID_LOAD_IMG]->Enable(true);
        buttons[Btn::ID_SHOW_CORNERS_PROJ - Btn::ID_LOAD_IMG]->Enable(true);
        buttons[Btn::ID_CALIB_RESULTS - Btn::ID_LOAD_IMG]->Enable(true);
        buttons[Btn::ID_SAVE - Btn::ID_LOAD_IMG]->Enable(true);
        buttons[Btn::ID_PREFERENCES - Btn::ID_LOAD_IMG]->Enable(true);
    }
}


void AppFrame::OnPreferences(wxCommandEvent& evt) {
    long style = wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX);
    PreferencesPerspectiveFrame* frame = new PreferencesPerspectiveFrame("Preferences", wxPoint(-1, -1), wxSize(550, 710), style, &dataCalib, this);
    frame->Show(true);
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

