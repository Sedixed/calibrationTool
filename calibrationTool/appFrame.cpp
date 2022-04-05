#include "appFrame.hpp"
#include <iostream>

wxBEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_BUTTON(ID_EXIT,        AppFrame::OnExit)
    EVT_BUTTON(ID_HELP,        AppFrame::OnHelp)
    EVT_BUTTON(ID_PERSPECTIVE, AppFrame::OnPerspectiveSelection)
    EVT_BUTTON(ID_SPHERICAL,   AppFrame::OnSphericalSelection)
wxEND_EVENT_TABLE()


wxSize getMaxSizeBtn(std::vector<wxButton>& btns);

AppFrame::AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, 0, title, pos, size) {
    
    wxPanel* panel = new wxPanel(this);
   
    wxButton* perspective = new wxButton(panel, ID_PERSPECTIVE, "Perspective camera");
    wxButton* spherical = new wxButton(panel, ID_SPHERICAL, "Spherical camera");
    wxButton* help = new wxButton(panel, ID_HELP, "Help");
    wxButton* exit = new wxButton(panel, ID_EXIT, "Exit");
    /*std::vector<wxButton> btns = new std::vector<wxButton>();
    btns.add(perspective);
    btns.add(spherical);
    btns.add(help);
    btns.add(exit);
    wxSize actSize = getMaxSizeBtn(btns);
    std::cout << actSize.GetWidth() << "\n";*/

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    vbox->AddStretchSpacer(1);
    vbox->Add(perspective, 0, wxALIGN_CENTER | wxBOTTOM, 50);
    vbox->Add(spherical, 0, wxALIGN_CENTER | wxBOTTOM, 50);
    vbox->Add(help, 0, wxALIGN_CENTER | wxBOTTOM, 50);
    vbox->Add(exit, 0, wxALIGN_CENTER);
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

wxSize getMaxSizeBtn(std::vector<wxButton>& btns) {
    wxSize max = btns.at(0).GetSize();
    for (int i = 1; i < btns.size(); ++i) {
        if (max.GetWidth() < btns.at(i).GetSize().GetWidth()) {
            max = btns.at(i).GetSize();
        }
    }
    return max;
}