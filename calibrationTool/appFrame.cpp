#include "appFrame.hpp"
#include <iostream>

wxBEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_BUTTON(ID_EXIT,        AppFrame::OnExit)
    EVT_BUTTON(ID_HELP,        AppFrame::OnHelp)
    EVT_BUTTON(ID_PERSPECTIVE, AppFrame::OnPerspectiveSelection)
    EVT_BUTTON(ID_SPHERICAL,   AppFrame::OnSphericalSelection)
wxEND_EVENT_TABLE()


void getMaxSizeBtn(std::vector<wxButton*>& btns, wxSize *retSize);

AppFrame::AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, 0, title, pos, size) {
    
    wxPanel* panel = new wxPanel(this);
   
    wxButton* perspective = new wxButton(panel, ID_PERSPECTIVE, "Perspective camera");
    wxButton* spherical = new wxButton(panel, ID_SPHERICAL, "Spherical camera");
    wxButton* help = new wxButton(panel, ID_HELP, "Help");
    wxButton* exit = new wxButton(panel, ID_EXIT, "Exit");
    std::vector<wxButton*> btns = {perspective, spherical, help, exit};
    
    wxSize* actSize = new wxSize();
    getMaxSizeBtn(btns, actSize);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    vbox->AddStretchSpacer(1);
    for (wxButton* btn : btns) {
        btn->SetSize(actSize->GetWidth(), btn->GetSize().GetHeight());
        vbox->Add(btn, 0, wxEXPAND | wxLEFT | wxRIGHT, 70);
        vbox->AddSpacer(40);
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
}

void AppFrame::OnSphericalSelection(wxCommandEvent& evt) {
    std::cout << "spher\n";
}

// getMaxSizeBtn : puts into the wxSize pointed by retSize the button contained in btns
//      that has the highest width
void getMaxSizeBtn(std::vector<wxButton*>& btns, wxSize* retSize) {
    *retSize = btns.at(0)->GetSize();
    for (int i = 1; i < btns.size(); ++i) {
        if (retSize->GetWidth() < btns.at(i)->GetSize().GetWidth()) {
            *retSize = btns.at(i)->GetSize();
        }
    }
}