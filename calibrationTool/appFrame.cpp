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
    
    panel = new wxPanel(this);

    // Base buttons declaration
    wxButton *perspective = Btn::btnFromId(Btn::ID_PERSPECTIVE, panel);
    wxButton* spherical = Btn::btnFromId(Btn::ID_SPHERICAL, panel);
    wxButton* help = Btn::btnFromId(Btn::ID_HELP, panel);
    wxButton* exit = Btn::btnFromId(Btn::ID_EXIT, panel);
    
    buttons = {perspective, spherical, help, exit};

    placeButtons(exit);
    //panel->SetSizer(vbox);

    Centre();
}


void AppFrame::OnExit(wxCommandEvent& evt) {
    Close(true);
}


void AppFrame::OnHelp(wxCommandEvent& evt) {
    wxLogMessage("Camera calibration tool. Please first select either spherical or perspective camera to start.");
}

// todo : déclarer les boutons séparément
// peut être créer des tableaux constants dans buttonsUtils.hpp/cpp
// base / persp / spher
void AppFrame::OnPerspectiveSelection(wxCommandEvent& evt) {
    resetButtons();

    // Buttons replacement
    for (int i = Btn::ID_LOAD_IMG; i <= Btn::ID_PREFERENCES; ++i) {
        wxButton *b = Btn::btnFromId(static_cast<Btn::ButtonsId>(i), panel);
        buttons.push_back(b);
    }
    wxButton* exit = Btn::btnFromId(Btn::ID_EXIT, panel);
    buttons.push_back(exit);

    placeButtons(exit);
}


void AppFrame::OnSphericalSelection(wxCommandEvent& evt) {
    std::cout << "spher\n";
}


void AppFrame::resetButtons() {
    for (wxButton *b : buttons) {
        b->Destroy();
    }
    buttons.erase(buttons.begin(), buttons.end());
}


void AppFrame::placeButtons(wxButton* last) {
    vbox = new wxBoxSizer(wxVERTICAL);

    wxSize* actSize = new wxSize();
    getMaxSizeOfBtns(buttons, actSize);

    int i = 0;
    vbox->AddStretchSpacer(1);
    for (wxButton* btn : buttons) {
        btn->SetSize(actSize->GetWidth(), btn->GetSize().GetHeight());
        vbox->Add(btn, 0, wxEXPAND | wxLEFT | wxRIGHT, 70);
        if (last != NULL && btn != last) {
            vbox->AddSpacer(40);
        }
        ++i;
    }
    vbox->AddStretchSpacer(1);

    panel->SetSizer(vbox);
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

