#include "../headers/AppFrame.hpp"
#include "../headers/ButtonsUtils.hpp"
#include "../headers/LoadImages.hpp"
#include "../headers/ExtractGridCorners.hpp"
#include <iostream>

// Spacing between two buttons in the base menu
#define BASE_SPACING 40

// Spacing between two buttons in the perspective menu
#define PERSPECTIVE_SPACING 30

// Spacing between two buttons in the spherical menu
// à définir plus tard
#define SPHERICAL_SPACING -1

#define PREF_WIDTH 1152
#define PREF_HEIGHT 864

#define PREF_PPR 7
#define PREF_PPC 5


// Event table used by the frame.
//  Makes each button corresponds its onclick function
wxBEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT,                 AppFrame::OnExit)
    EVT_BUTTON(Btn::ID_HELP,                 AppFrame::OnHelp)
    EVT_BUTTON(Btn::ID_PERSPECTIVE,          AppFrame::OnPerspectiveSelection)
    EVT_BUTTON(Btn::ID_SPHERICAL,            AppFrame::OnSphericalSelection)
    EVT_BUTTON(Btn::ID_LOAD_IMG,             AppFrame::OnLoadImages)
    EVT_BUTTON(Btn::ID_EXTRACT_GRID_CORNERS, AppFrame::OnExtractGridCorners)
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

    dataCalib.pref.render_size = cv::Size(PREF_WIDTH, PREF_HEIGHT);
    dataCalib.pref.pattern_size = cv::Size(PREF_PPR, PREF_PPC);
    
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
    SetSize(wxSize(300, 600));
    // disabling buttons 
    int base = Btn::ID_EXTRACT_GRID_CORNERS - Btn::ID_LOAD_IMG;
    int borne =  Btn::ID_PREFERENCES - Btn::ID_EXTRACT_GRID_CORNERS + 1;
    for (int i = base; i < borne; ++i) {
        buttons[i]->Enable(false);
    }

    placeButtons(PERSPECTIVE_SPACING);
}


void AppFrame::OnSphericalSelection(wxCommandEvent& evt) {
    std::cout << "spher\n";
}


void AppFrame::OnLoadImages(wxCommandEvent& evt) {
    int r = LoadImages(&dataCalib);
    if (r == 0) {
        buttons[Btn::ID_EXTRACT_GRID_CORNERS - Btn::ID_LOAD_IMG]->Enable(true);
        
    }

    /* tests
    for (IOCalibration ioc : dataCalib.IOcalib) {
        if (ioc.image_name[0] == '\0') {
            break;
        }
        std::cout << ioc.image_name << "\n";
        std::cout << (ioc.active_image ? "active\n" : "not active\n");
    }*/
}

void AppFrame::OnExtractGridCorners(wxCommandEvent& evt) {
    int r = ExtractGridCorners(&dataCalib);
    if (r == 0) {

    }
}


void AppFrame::resetButtons() {
    for (wxButton *b : buttons) {
        vbox->Detach(b);
        b->Destroy();
    }
}


void AppFrame::placeButtons(int spacing) {
    // get the max size for buttons
    wxSize* actSize = new wxSize();
    getMaxSizeOfBtns(buttons, actSize);

    vbox = new wxBoxSizer(wxVERTICAL);

    // placing buttons and spaces
    vbox->AddStretchSpacer(1);
    for (wxButton* btn : buttons) {
        btn->SetSize(actSize->GetWidth(), btn->GetSize().GetHeight());
        vbox->Add(btn, 0, wxEXPAND | wxLEFT | wxRIGHT, 60);
        if (btn != buttons.back()) {
            // au choix, à voir
            vbox->AddSpacer(spacing);
            //vbox->AddStretchSpacer(1);
        }
    }
    
    // refreshing
    vbox->AddStretchSpacer(1);
    vbox->Layout();

    panel->SetSizer(vbox);
    panel->Layout();
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

