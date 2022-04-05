#include "appFrame.hpp"
#include "buttonsUtils.hpp"
#include <iostream>


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


AppFrame::AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, 0, title, pos, size) {
    
    wxPanel* panel = new wxPanel(this);

    // Base buttons declaration
    wxButton *perspective = Btn::btnFromId(Btn::ID_PERSPECTIVE, panel);
    wxButton* spherical = Btn::btnFromId(Btn::ID_SPHERICAL, panel);
    wxButton* help = Btn::btnFromId(Btn::ID_HELP, panel);
    wxButton* exit = Btn::btnFromId(Btn::ID_EXIT, panel);
    
    buttons = {perspective, spherical, help, exit};
    
    // get the largest button's size
    wxSize* actSize = new wxSize();
    getMaxSizeOfBtns(buttons, actSize);

    // place components and resize buttons to match the largest's one
    vbox = new wxBoxSizer(wxVERTICAL);

    vbox->AddStretchSpacer(1);
    for (wxButton* btn : buttons) {
        btn->SetSize(actSize->GetWidth(), btn->GetSize().GetHeight());
        vbox->Add(btn, 0, wxEXPAND | wxLEFT | wxRIGHT, 70);
        if (btn != exit) {
            vbox->AddSpacer(40);
        }
    }
    vbox->AddStretchSpacer(1);

    panel->SetSizer(vbox);

    Centre();
}


void AppFrame::OnExit(wxCommandEvent& evt) {
    Close(true);
}


void AppFrame::OnHelp(wxCommandEvent& evt) {
    wxLogMessage("Camera calibration tool. Please first select either spherical or perspective camera to start.");
}


void AppFrame::OnPerspectiveSelection(wxCommandEvent& evt) {
    std::cout << "persp\n";

    for (wxButton *b : buttons) {
        b->Destroy();
    }
    buttons.erase(buttons.begin(), buttons.end());


}


void AppFrame::OnSphericalSelection(wxCommandEvent& evt) {
    std::cout << "spher\n";
}


void getMaxSizeOfBtns(std::vector<wxButton*>& btns, wxSize* retSize) {
    *retSize = btns.at(0)->GetSize();
    for (int i = 1; i < btns.size(); ++i) {
        if (retSize->GetWidth() < btns.at(i)->GetSize().GetWidth()) {
            *retSize = btns.at(i)->GetSize();
        }
    }
}