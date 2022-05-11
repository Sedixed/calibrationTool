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
#include <wx/mimetype.h>
#include <iostream>
#include <wx/filename.h>


// Spacing between two buttons in the base menu
#define BASE_SPACING 40

// Spacing between two buttons in the perspective menu
#define PERSPECTIVE_SPACING 30

// Spacing between two buttons in the spherical menu
#define SPHERICAL_SPACING 30

// Adapt size for both Linux and Windows widgets sizes
#ifdef WINDOWS  
    #define MENU_SIZE wxSize(300, 550)
    #define PREF_SPHER_SIZE wxSize(425, 580)
    #define PREF_PERSP_SIZE wxSize(470, 590)
#else
    #define MENU_SIZE wxSize(300, 650)
    #define PREF_SPHER_SIZE wxSize(550, 720)
    #define PREF_PERSP_SIZE wxSize(550, 710)
#endif


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

    // todo :
    // version release/debug ?

    // fait :
    // manuel user (reste que generalised focal length, voir schémas qu'il a envoyé)
    // help permet d'ouvrir le pdf user -> gérer les espaces sous windows car fonctionne pas

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
    wxMessageDialog* msgBox = new wxMessageDialog(this, 
        "Camera calibration tool. Please first select either spherical or perspective camera to start.",
        "Help", wxHELP | wxICON_INFORMATION);
    msgBox->SetHelpLabel("View documentation");

    if (msgBox->ShowModal() == wxID_HELP) {
        wxMimeTypesManager mgr;
        wxFileType* pdfType = mgr.GetFileTypeFromExtension("pdf");
        wxString path = wxGetCwd();
        #ifdef WINDOWS
            path += _("\\docs\\user_manual.pdf");
        #else
            path += _("/docs/user_manual.pdf");
        #endif
        
        auto command = pdfType->GetOpenCommand(path);
        
        std::string arg = "--single-argument";
        size_t pos = command.find(arg);
        
        if (pos != std::string::npos) {
            pos += arg.length();
            if (command[pos + 1] == '\"') {
                command.erase(pos + 1, 1);
                command.erase(command.length() - 1, 1);
            }
        }
        if (command == wxEmptyString) {
            delete pdfType;
            const char *errMsg = "Could not open the manual file. Try to open the file from the folder where you installed the software.";
            wxLogError(errMsg);
            return;
        }
        wxExecute(command);
        delete pdfType;
    }
}


void AppFrame::OnPerspectiveSelection(wxCommandEvent& evt) {
    resetButtons();
    dataCalib.type = PERSPECTIVE_TYPE;
    buttons = Btn::perspectiveButtons(panel);
    SetSize(MENU_SIZE);
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
    SetSize(MENU_SIZE);
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
    setButtonsState(std::vector<Btn::ButtonsId> {
        Btn::ID_SHOW_CORNERS_PROJ, 
        Btn::ID_CALIB_RESULTS,
        Btn::ID_SAVE}, false
    );
    int r = ExtractGridCorners(&dataCalib);
    if (r == 0) {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_CALIB}, true
        );
        
    } else {
        setButtonsState(std::vector<Btn::ButtonsId> {
            Btn::ID_CALIB}, false
        );
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
    ShowReprojection(&dataCalib, this);

}


void AppFrame::OnCalibResults(wxCommandEvent& evt) {
    int r = CalibrationResults(&dataCalib, this);
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
                    PREF_PERSP_SIZE, style, &dataCalib, this, true);
            } else {
                frame = new PreferencesPerspectiveFrame("Preferences", wxPoint(-1, -1), 
                   PREF_PERSP_SIZE, style, &dataCalib, this, false);
            }
            frame->Show(true);
            break;
        }
        case SPHERICAL_TYPE:
        {   
            PreferencesSphericalFrame* frame;
            if (dataCalib.nb_images > 0) {
                frame = new PreferencesSphericalFrame("Preferences", wxPoint(-1, -1), 
                   PREF_SPHER_SIZE, style, &dataCalib, this, true);
            } else {
                frame = new PreferencesSphericalFrame("Preferences", wxPoint(-1, -1), 
                   PREF_SPHER_SIZE, style, &dataCalib, this, false);
            }
            frame->Show(true);
            break;
        }
        default:
        {
            wxMessageBox("Error : unknown calibration type.", "Error", wxICON_ERROR);
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
    for (size_t i = 1; i < btns.size(); ++i) {
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