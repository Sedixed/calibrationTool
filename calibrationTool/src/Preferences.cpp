#include "../headers/Preferences.hpp"
#include "../headers/ButtonsUtils.hpp"
#include <opencv2/opencv.hpp>
#include <iterator>

template<typename T, int size>
int arrLength(T(&)[size]);

void fillWithVoid(wxSizer* sizer, wxPanel* panel, int n);

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
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(vbox);
    vbox->AddStretchSpacer(1);

    // Parameters
    wxPanel* parameters = new wxPanel(panel);
    vbox->Add(new wxStaticText(panel, Pref::ID_PARAMETERS, Pref::labels[Pref::ID_PARAMETERS]));
    wxFlexGridSizer *fgboxParameters = new wxFlexGridSizer(4, 6, 10, 10);
    parameters->SetSizer(fgboxParameters);

    // Focal length
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Generalised focal length :"));
    fgboxParameters->Add(new wxCheckBox(parameters, wxID_ANY, "Gu"));
    fgboxParameters->Add(new wxCheckBox(parameters, wxID_ANY, "Gv"));
    fillWithVoid(fgboxParameters, parameters, 3);
    // Principal point 
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Principal point :"));
    fgboxParameters->Add(new wxCheckBox(parameters, wxID_ANY, "u0"));
    fgboxParameters->Add(new wxCheckBox(parameters, wxID_ANY, "v0"));
    fillWithVoid(fgboxParameters, parameters, 3);
    // Skewskew
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Skew :"));
    fgboxParameters->Add(new wxCheckBox(parameters, wxID_ANY, "a"));
    fillWithVoid(fgboxParameters, parameters, 4);
    // Distorsions
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Distorsions :"));
    for (int i = 0; i < 5; ++i) {
        std::string label = "k" + std::to_string(i + 1);
        fgboxParameters->Add(new wxCheckBox(parameters, wxID_ANY, label));
    }
    vbox->Add(parameters, wxTOP | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 5);
    vbox->AddSpacer(30);
    
    // Search window size
    wxPanel* search = new wxPanel(panel);
    vbox->Add(new wxStaticText(panel, Pref::ID_SEARCH_WINDOW_SIZE, Pref::labels[Pref::ID_SEARCH_WINDOW_SIZE]));
    wxBoxSizer* hboxSearch = new wxBoxSizer(wxHORIZONTAL);
    search->SetSizer(hboxSearch);
    int size = arrLength<const int>(Pref::searchSizes);
    for (int i = 0; i < size; ++i) {
        int value = Pref::searchSizes[i];
        std::string label = std::to_string(value) + "x" + std::to_string(value);
        hboxSearch->Add(new wxRadioButton(search, wxID_ANY, _(label), 
                    wxDefaultPosition, wxDefaultSize, i == 0 ? wxRB_GROUP : 0));
        hboxSearch->AddStretchSpacer(1);
    }
    vbox->Add(search, wxTOP | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 5);
    vbox->AddStretchSpacer(1);

    // Render window size
    wxPanel* render = new wxPanel(panel);
    vbox->Add(new wxStaticText(panel, Pref::ID_RENDER_WINDOW_SIZE, Pref::labels[Pref::ID_RENDER_WINDOW_SIZE]));
    wxBoxSizer* hboxRender = new wxBoxSizer(wxHORIZONTAL);
    render->SetSizer(hboxRender);
    size = arrLength<const cv::Size>(Pref::renderSizes);
    for (int i = 0; i < size; ++i) {
        int w = Pref::renderSizes[i].width;
        int h = Pref::renderSizes[i].height;
        std::string label = std::to_string(w) + "x" + std::to_string(h);
        hboxRender->Add(new wxRadioButton(render, wxID_ANY, _(label), 
                    wxDefaultPosition, wxDefaultSize, i == 0 ? wxRB_GROUP : 0));
        hboxRender->AddStretchSpacer(1);
    }
    vbox->Add(render, wxTOP | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 5);
    vbox->AddStretchSpacer(1);

    // Calibration pattern properties
    wxPanel* properties = new wxPanel(panel);
    vbox->Add(new wxStaticText(panel, Pref::ID_CALIB_PROPERTIES, Pref::labels[Pref::ID_CALIB_PROPERTIES]));
    wxFlexGridSizer *fgboxProp = new wxFlexGridSizer(4, 2, 10, 10);
    properties->SetSizer(fgboxProp);
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Number of squares along X :"));
    fgboxProp->Add(new wxTextCtrl(properties, wxID_ANY, "20.0"));
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Number of squares along Y :"));
    fgboxProp->Add(new wxTextCtrl(properties, wxID_ANY, "20.0"));
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Size of each square along X :"));
    fgboxProp->Add(new wxTextCtrl(properties, wxID_ANY, "7"));
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Size of each square along Y :"));
    fgboxProp->Add(new wxTextCtrl(properties, wxID_ANY, "5"));
    vbox->Add(properties, wxTOP | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 5);
    vbox->AddSpacer(30);

    // Exit buttons
    wxPanel* btns = new wxPanel(panel);
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    btns->SetSizer(hbox);
    hbox->Add(Btn::btnFromId(Btn::ID_EXIT_CANCEL, btns));
    hbox->Add(Btn::btnFromId(Btn::ID_EXIT_OK, btns));
    vbox->Add(btns, wxTOP | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 5);
    vbox->AddStretchSpacer(1);

}

template<typename T, int size>
int arrLength(T(&)[size]) {
    return size;
}

void fillWithVoid(wxSizer* sizer, wxPanel* panel, int n) {
    for (int i = 0; i < n; ++i) {
        sizer->Add(new wxStaticText(panel, wxID_ANY, ""));
    }
}