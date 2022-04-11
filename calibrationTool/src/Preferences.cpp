#include "../headers/Preferences.hpp"
#include "../headers/ButtonsUtils.hpp"
#include <opencv2/opencv.hpp>


// Event table used by the preferences frame.
//  Makes each button corresponds its onclick function
wxBEGIN_EVENT_TABLE(PreferencesFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT_OK,                 PreferencesFrame::OnExitOk)
    EVT_BUTTON(Btn::ID_EXIT_CANCEL,             PreferencesFrame::OnExitCancel)
wxEND_EVENT_TABLE()

PreferencesFrame::PreferencesFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(NULL, 0, title, pos, size, style) {
    
    panel = new wxPanel(this);
    CreateAndPlaceComponents();

}

void PreferencesFrame::OnExitOk(wxCommandEvent& evt) {
    // save
    Close(true);
}

void PreferencesFrame::OnExitCancel(wxCommandEvent& evt) {
    Close(true);
}

void PreferencesFrame::CreateAndPlaceComponents() {
    std::vector<wxStaticText*> mainLabels;
    for (int i = 0; i < Pref::ID_CALIB_PROPERTIES; ++i) {
        mainLabels.push_back(new wxStaticText(panel, i, Pref::labels[i]));
    }

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(vbox);

    wxPanel* parameters = new wxPanel(panel);
    wxPanel* search = new wxPanel(panel);
    wxPanel* render = new wxPanel(panel);
    wxPanel* properties = new wxPanel(panel);
    /*
    // Search window size
    wxBoxSizer* hboxSearch = new wxBoxSizer(wxHORIZONTAL);
    search->SetSizer(hboxSearch);
    wxArrayString* searchLabels = new wxArrayString();
    int size = 0;
    for (int i : Pref::searchSizes) {
        ++size;
        wxString label = _(i + "x" + i);
        searchLabels->Add(label);
        wxRadioButton* b = new wxRadioButton(search, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    }
    wxRadioBox* rbox = new wxRadioBox(search, wxID_ANY, _("searchRadioBox"), 
                                    wxDefaultPosition, wxDefaultSize, 
                                    size, &searchLabels);

    
    */
}