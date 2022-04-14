#include "../headers/AppFrame.hpp"
#include "../headers/ButtonsUtils.hpp"
#include "../headers/LoadImages.hpp"
#include "../headers/ExtractGridCorners.hpp"
#include "../headers/Preferences.hpp"
#include "../headers/Calibration.hpp"
#include <iostream>

// Spacing between two buttons in the base menu
#define BASE_SPACING 40

// Spacing between two buttons in the perspective menu
#define PERSPECTIVE_SPACING 30

// Spacing between two buttons in the spherical menu
// à définir plus tard
#define SPHERICAL_SPACING -1

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

    // TODO: gérer fermeture alors que des images sont encore ouvertes (segfault parfois)
    // enlever le spam click pour les images -> cooldown entre chaque et demander à la fin (en cours)
    //      tout afficher e le laisser fermer 
    // possibilité de load un fichier (btn ajouté)
    // choisir calibrate / calibrateRO (côté calib à faire, pltôt bien géré avec intrinsic guess aussi
    //      mais change tjr les valeurs "fixées"
    // tester les distorsions / focal / point etc
    // dans calibration results, afficher résultats (calculer erreur par mire avec des reprojections)
    // adapter pour windows à un moment     // dans calibration results, afficher résultats 
    //      (calculer erreur par mire avec des reprojections)


    
    panel = new wxPanel(this);
    buttons = Btn::baseButtons(panel);
    placeButtons(BASE_SPACING);
    Centre();
}


void AppFrame::OnExit(wxCommandEvent& evt) {
    Close(true);
}


void AppFrame::OnHelp(wxCommandEvent& evt) {
    wxLogMessage("Camera calibration tool. Please first select either spherical or perspective camera to start.");
}


void AppFrame::OnPerspectiveSelection(wxCommandEvent& evt) {
    resetButtons();
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
    std::cout << "TODO\n";
}


void AppFrame::OnLoadImages(wxCommandEvent& evt) {
    int r = LoadImages(&dataCalib);
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
        buttons[Btn::ID_SAVE - Btn::ID_LOAD_IMG]->Enable(true);
    }
}


void AppFrame::OnShowReprojection(wxCommandEvent& evt) {
    std::cout << "reproj\n";
}


void AppFrame::OnCalibResults(wxCommandEvent& evt) {
    std::cout << "results\n";
}


void AppFrame::OnSave(wxCommandEvent& evt) {
    std::cout << "save\n";
}


void AppFrame::OnLoadFile(wxCommandEvent& evt) {
    std::cout << "load file\n";
}


void AppFrame::OnPreferences(wxCommandEvent& evt) {
    long style = wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX);
    PreferencesFrame* frame = new PreferencesFrame("Preferences", wxPoint(-1, -1), wxSize(550, 730), style, &dataCalib);
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

