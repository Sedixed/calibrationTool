#include "AppFrame.hpp"
#include "ButtonsUtils.hpp"
#include <iostream>

#define BASE_SPACING 40
#define PERSPECTIVE_SPACING 30
#define SPHERICAL_SPACING 2

// Event table used by the frame.
//  Makes each button corresponds its onclick function
wxBEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT,        AppFrame::OnExit)
    EVT_BUTTON(Btn::ID_HELP,        AppFrame::OnHelp)
    EVT_BUTTON(Btn::ID_PERSPECTIVE, AppFrame::OnPerspectiveSelection)
    EVT_BUTTON(Btn::ID_SPHERICAL,   AppFrame::OnSphericalSelection)
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
    placeButtons(PERSPECTIVE_SPACING);
    SetSize(wxSize(300, 600));
}


void AppFrame::OnSphericalSelection(wxCommandEvent& evt) {
    std::cout << "spher\n";
}


void AppFrame::resetButtons() {
    for (wxButton *b : buttons) {
        vbox->Detach(b);
        b->Destroy();
    }
}


void AppFrame::placeButtons(int spacing) {
    wxSize* actSize = new wxSize();
    getMaxSizeOfBtns(buttons, actSize);

    vbox = new wxBoxSizer(wxVERTICAL);

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

