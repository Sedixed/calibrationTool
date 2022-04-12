#include "../headers/Preferences.hpp"
#include "../headers/ButtonsUtils.hpp"
#include <opencv2/opencv.hpp>
#include <wx/valnum.h>

// ID's for wxTextCtrl
#define NB_X 1
#define NB_Y 2 
#define SIZE_SQUARE_X 3 
#define SIZE_SQUARE_Y 4

/**
 * Calculates the amount of elements of an array. 
 * 
 * @tparam T    Type of elements in the array.
 * @tparam size Amount of elements of the array.
 * @return int  The amount of elements in the array.
 */
template<typename T, int size>
int arrLength(T(&)[size]);


/**
 * Fills a wx[Flex]GridSizer with n blanks added to sizer, disposed
 * on panel.
 * 
 * @param sizer The sizer to which we add the blank element.
 * @param panel The panel on which we create the blank element.
 * @param n     How many blanks are to put.
 */
void fillWithVoid(wxSizer* sizer, wxPanel* panel, int n);

// Event table used by the preferences frame.
//  Makes each button corresponds its onclick function
wxBEGIN_EVENT_TABLE(PreferencesFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT_OK,                 PreferencesFrame::OnExitOk)
    EVT_BUTTON(Btn::ID_EXIT_CANCEL,             PreferencesFrame::OnExitCancel)
    EVT_RADIOBUTTON(Pref::searchId[0],                  PreferencesFrame::SetLastSearch)
    EVT_RADIOBUTTON(Pref::searchId[1],                  PreferencesFrame::SetLastSearch)
    EVT_RADIOBUTTON(Pref::searchId[2],                  PreferencesFrame::SetLastSearch)
    EVT_RADIOBUTTON(Pref::searchId[3],                  PreferencesFrame::SetLastSearch)
    EVT_RADIOBUTTON(Pref::searchId[4],                  PreferencesFrame::SetLastSearch)
    EVT_RADIOBUTTON(Pref::renderId[0],                  PreferencesFrame::SetLastRender)
    EVT_RADIOBUTTON(Pref::renderId[1],                  PreferencesFrame::SetLastRender)
    EVT_RADIOBUTTON(Pref::renderId[2],                  PreferencesFrame::SetLastRender)
    EVT_RADIOBUTTON(Pref::renderId[3],                  PreferencesFrame::SetLastRender)
wxEND_EVENT_TABLE()

PreferencesFrame::PreferencesFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib)
    : wxFrame(NULL, 0, title, pos, size, style) {
    
    dataCalib = calib;
    searchWindowSize = Pref::searchSizes[Pref::DEFAULT_SEARCH_INDEX];
    renderWindowSize = Pref::renderSizes[Pref::DEFAULT_RENDER_INDEX];
    panel = new wxPanel(this);
    CreateAndPlaceComponents();

}

void PreferencesFrame::OnExitOk(wxCommandEvent& evt) {
    // Saving
    long n;
    double m;
    // Number of squares along X
    wxString text = nbX->GetValue();
    if (!text.ToLong(&n)) {
        wxMessageBox("Couldn't save number of squares along X.", "Preferences saving", wxOK);
    }
    dataCalib->calibPattern.nbSquareX = (int) n;
    // Number of squares along Y
    text = nbY->GetValue();
    if (!text.ToLong(&n)) {
        wxMessageBox("Couldn't save number of squares along Y.", "Preferences saving", wxOK);
    }
    dataCalib->calibPattern.nbSquareY = (int) n;
    // Size of each square along X
    text = sizeX->GetValue();
    if (!text.ToDouble(&m)) {
        wxMessageBox("Couldn't save size of each square along X.", "Preferences saving", wxOK);
    }
    dataCalib->calibPattern.sizeSquareX = m;
    // Size of each square along Y
    text = sizeY->GetValue();
    if (!text.ToDouble(&m)) {
        wxMessageBox("Couldn't save size of each square along Y.", "Preferences saving", wxOK);
    }
    dataCalib->calibPattern.sizeSquareY = m;

    dataCalib->pref.render_size = renderWindowSize;
    dataCalib->pref.search_window_size = searchWindowSize;



    Close(true);
}

void PreferencesFrame::OnExitCancel(wxCommandEvent& evt) {
    Close(true);
}

void PreferencesFrame::CreateAndPlaceComponents() {
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(vbox);
    
    vbox->AddSpacer(15);
    // Parameters
    wxPanel* parameters = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    vbox->Add(new wxStaticText(panel, Pref::ID_PARAMETERS, "\t" + Pref::labels[Pref::ID_PARAMETERS]), wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(10);
    wxFlexGridSizer *fgboxParameters = new wxFlexGridSizer(4, 6, 10, 10);
    parameters->SetSizer(fgboxParameters);

    // Focal length
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Generalised focal length :"));
    wxCheckBox* gu = new wxCheckBox(parameters, wxID_ANY, "Gu");
    gu->SetValue(true);
    fgboxParameters->Add(gu);
    wxCheckBox* gv = new wxCheckBox(parameters, wxID_ANY, "Gv");
    gv->SetValue(true);
    fgboxParameters->Add(gv);
    fillWithVoid(fgboxParameters, parameters, 3);
    // Principal point 
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Principal point :"));
    wxCheckBox* u0 = new wxCheckBox(parameters, wxID_ANY, "u0");
    u0->SetValue(true);
    fgboxParameters->Add(u0);
    wxCheckBox* v0 = new wxCheckBox(parameters, wxID_ANY, "v0");
    v0->SetValue(true);
    fgboxParameters->Add(v0);
    fillWithVoid(fgboxParameters, parameters, 3);
    // Skewskew
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Skew :"));
     wxCheckBox* a = new wxCheckBox(parameters, wxID_ANY, "a");
    a->SetValue(true);
    fgboxParameters->Add(a);
    fillWithVoid(fgboxParameters, parameters, 4);
    // Distorsions
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Distorsions :"));
    for (int i = 0; i < 5; ++i) {
        std::string label = "k" + std::to_string(i + 1);
        wxCheckBox* k = new wxCheckBox(parameters, wxID_ANY, label);
        k->SetValue(true);
        fgboxParameters->Add(k);
    }
    vbox->Add(parameters, wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(30);
    
    // Search window size
    wxPanel* search = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    vbox->Add(new wxStaticText(panel, Pref::ID_SEARCH_WINDOW_SIZE, "\t" + Pref::labels[Pref::ID_SEARCH_WINDOW_SIZE]), wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(10);
    wxBoxSizer* hboxSearch = new wxBoxSizer(wxHORIZONTAL);
    search->SetSizer(hboxSearch);
    int size = arrLength<const int>(Pref::searchSizes);
    for (int i = 0; i < size; ++i) {
        int value = Pref::searchSizes[i];
        std::string label = std::to_string(value) + "x" + std::to_string(value);
        wxRadioButton *b = new wxRadioButton(search, Pref::searchId[i], _(label), 
                    wxDefaultPosition, wxDefaultSize, i == 0 ? wxRB_GROUP : 0);
        if (value == dataCalib->pref.search_window_size) {
            b->SetValue(true);
        }
        hboxSearch->Add(b);
        hboxSearch->AddStretchSpacer(1);
    }
    vbox->Add(search, wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(30);

    // Render window size
    wxPanel* render = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    vbox->Add(new wxStaticText(panel, Pref::ID_RENDER_WINDOW_SIZE, "\t" + Pref::labels[Pref::ID_RENDER_WINDOW_SIZE]), wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(10);
    wxBoxSizer* hboxRender = new wxBoxSizer(wxHORIZONTAL);
    render->SetSizer(hboxRender);
    size = arrLength<const cv::Size>(Pref::renderSizes);
    for (int i = 0; i < size; ++i) {
        int w = Pref::renderSizes[i].width;
        int h = Pref::renderSizes[i].height;
        std::string label = std::to_string(w) + "x" + std::to_string(h);
        wxRadioButton *b = new wxRadioButton(render, Pref::renderId[i], _(label), 
                    wxDefaultPosition, wxDefaultSize, i == 0 ? wxRB_GROUP : 0);
        
        cv::Size size = dataCalib->pref.render_size;
        if (w == size.width && h == size.height) {
            b->SetValue(true);
        }
        hboxRender->Add(b);
        hboxRender->AddStretchSpacer(1);
    }
    vbox->Add(render, wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(30);

    // Calibration pattern properties
    wxFloatingPointValidator<float> valFloat(2, NULL, wxNUM_VAL_ZERO_AS_BLANK);
    valFloat.SetRange(0, 100);

    wxIntegerValidator<int> valInt(NULL);
    valInt.SetRange(0, 100);

    wxPanel* properties = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    vbox->Add(new wxStaticText(panel, Pref::ID_CALIB_PROPERTIES, "\t" + Pref::labels[Pref::ID_CALIB_PROPERTIES]), wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(10);
    wxFlexGridSizer *fgboxProp = new wxFlexGridSizer(4, 2, 10, 10);
    properties->SetSizer(fgboxProp);
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Number of squares along X :"));
    nbX = new wxTextCtrl(properties, NB_X, _(std::to_string(dataCalib->calibPattern.nbSquareX)), 
                        wxDefaultPosition, wxDefaultSize, 0, valInt);
    fgboxProp->Add(nbX);
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Number of squares along Y :"));
    nbY = new wxTextCtrl(properties, NB_Y, _(std::to_string(dataCalib->calibPattern.nbSquareY)),
                        wxDefaultPosition, wxDefaultSize, 0, valInt);
    fgboxProp->Add(nbY);

    std::string ssx = std::to_string(dataCalib->calibPattern.sizeSquareX);
    ssx = ssx.substr(0, ssx.find(".,") + 6);
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Size of each square along X (mm) :"));
    sizeX = new wxTextCtrl(properties, wxID_ANY, _(ssx), wxDefaultPosition, wxDefaultSize, 0, valFloat);
    fgboxProp->Add(sizeX);

    std::string ssy = std::to_string(dataCalib->calibPattern.sizeSquareY);
    ssy = ssy.substr(0, ssy.find(".,") + 6);
    fgboxProp->Add(new wxStaticText(properties, wxID_ANY, "Size of each square along Y (mm) :"));
    sizeY = new wxTextCtrl(properties, wxID_ANY, _(ssy), wxDefaultPosition, wxDefaultSize, 0, valFloat);
    fgboxProp->Add(sizeY);
    vbox->Add(properties, wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(30);

    // Exit buttons
    wxPanel* btns = new wxPanel(panel);
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    btns->SetSizer(hbox);
    hbox->Add(Btn::btnFromId(Btn::ID_EXIT_CANCEL, btns));
    hbox->AddSpacer(20);
    hbox->Add(Btn::btnFromId(Btn::ID_EXIT_OK, btns));
    vbox->Add(btns, wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(15);
}


void PreferencesFrame::SetLastSearch(wxCommandEvent& evt) {
    for (int i = 0; i < arrLength<const int>(Pref::searchSizes); ++i) {
        if (Pref::searchId[i] == evt.GetId()) {
            searchWindowSize = Pref::searchSizes[i];
            break;
        }
    }
}


void PreferencesFrame::SetLastRender(wxCommandEvent& evt) {
    for (int i = 0; i < arrLength<const cv::Size>(Pref::renderSizes); ++i) {
        if (Pref::renderId[i] == evt.GetId()) {
            renderWindowSize = Pref::renderSizes[i];
            break;
        }
    }
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