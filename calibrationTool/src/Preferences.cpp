#include "../headers/Preferences.hpp"
#include "../headers/ButtonsUtils.hpp"
#include <opencv2/opencv.hpp>
#include <wx/valnum.h>

// IDs for wxTextCtrl
#define NB_X 1
#define NB_Y 2 
#define SIZE_SQUARE_X 3 
#define SIZE_SQUARE_Y 4
#define GU 80
#define GV 81
#define U0 82
#define V0 83

// Number of K parameters for distorsion
#define NB_OF_K_PARAM 5


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
//  Makes each button corresponds its onclick function, and 
//  same for radioButtons and text updates.
wxBEGIN_EVENT_TABLE(PreferencesFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT_OK,                         PreferencesFrame::OnExitOk)
    EVT_BUTTON(Btn::ID_EXIT_CANCEL,                     PreferencesFrame::OnExitCancel)
    EVT_COMMAND_RANGE(Pref::RENDER_MIN_ID, Pref::RENDER_MIN_ID + Pref::RENDER_NB_ID - 1,
            wxEVT_RADIOBUTTON, PreferencesFrame::SetLastRender)
    EVT_COMMAND_RANGE(Pref::SEARCH_MIN_ID, Pref::SEARCH_MIN_ID + Pref::SEARCH_NB_ID - 1,
            wxEVT_RADIOBUTTON, PreferencesFrame::SetLastSearch)
    EVT_COMMAND_RANGE(Pref::ID_FOCAL, Pref::ID_K1 + NB_OF_K_PARAM - 1,
            wxEVT_CHECKBOX, PreferencesFrame::UpdateFlags)
    EVT_COMMAND_RANGE(GU, V0, wxEVT_TEXT, PreferencesFrame::SetOkState)
    EVT_COMMAND_RANGE(NB_X, SIZE_SQUARE_Y, wxEVT_TEXT, PreferencesFrame::SetOkState)
wxEND_EVENT_TABLE()


PreferencesFrame::PreferencesFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib)
    : wxFrame(NULL, 0, title, pos, size, style) {
    
    // Setting the base values
    dataCalib = calib;
    flags = dataCalib->pref.parameters_flags;
    searchWindowSize = calib->pref.search_window_size;
    renderWindowSize = calib->pref.render_size;
    ignoreFocal = calib->intrinsics.at<double>(0, 0) > 0. || calib->intrinsics.at<double>(1, 1) > 0.;
    ignorePoint = calib->intrinsics.at<double>(0, 2) > 0. || calib->intrinsics.at<double>(1, 2) > 0.;
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

    // Saving user defined intrinsics parameters if provided

    // Focal Length
    if (ignoreFocal) {
        flags ^= cv::CALIB_FIX_FOCAL_LENGTH;
        wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
        wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
        double fx;
        if (!gu->GetLineText(0).ToDouble(&fx)) {
            wxMessageBox("Couldn't save Gu.", "Preferences saving", wxOK);
        }
        double fy;
        if (!gv->GetLineText(0).ToDouble(&fy)) {
            wxMessageBox("Couldn't save Gv.", "Preferences saving", wxOK);
        }
        dataCalib->intrinsics.at<double>(0, 0) = fx;
        dataCalib->intrinsics.at<double>(1, 1) = fy;
    } else {
        dataCalib->intrinsics.at<double>(0, 0) = 0;
        dataCalib->intrinsics.at<double>(1, 1) = 0;
    }

    // Principal point
    if (ignorePoint) {
        flags ^= cv::CALIB_FIX_PRINCIPAL_POINT;
        wxTextCtrl* u0 = (wxTextCtrl*) FindWindowById(U0);
        wxTextCtrl* v0 = (wxTextCtrl*) FindWindowById(V0);
        double cx;
        if (!u0->GetLineText(0).ToDouble(&cx)) {
            wxMessageBox("Couldn't save u0.", "Preferences saving", wxOK);
        }
        double cy;
        if (!v0->GetLineText(0).ToDouble(&cy)) {
            wxMessageBox("Couldn't save v0.", "Preferences saving", wxOK);
        }
        dataCalib->intrinsics.at<double>(0, 2) = cx;
        dataCalib->intrinsics.at<double>(1, 2) = cy;
    } else {
        dataCalib->intrinsics.at<double>(0, 2) = 0;
        dataCalib->intrinsics.at<double>(1, 2) = 0;
    }

    // Calibration flags
    dataCalib->pref.parameters_flags = flags;
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
    parameters = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    vbox->Add(new wxStaticText(panel, Pref::ID_PARAMETERS, "\t" + Pref::labels[Pref::ID_PARAMETERS]), 
            wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(10);

    /* 1 + NB_OF_K_PARAM because there is 1 column for parameter name and 
     *   NB_OF_K_PARAM because that is at the moment the largest amount 
     *   of columns required.
     */
    fgboxParameters = new wxFlexGridSizer(3, 1 + NB_OF_K_PARAM, 10, 10);
    parameters->SetSizer(fgboxParameters);

    // Focal length
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Generalised focal length :"));
    wxCheckBox* g = new wxCheckBox(parameters, Pref::ID_FOCAL, "G");
    g->SetValue(!ignoreFocal);
    fgboxParameters->Add(g);

    // Validator for focal length / principal point input
    wxFloatingPointValidator<float> valFocal(3, NULL, wxNUM_VAL_ZERO_AS_BLANK);
    valFocal.SetMin(0);


    wxTextCtrl *gu = new wxTextCtrl(parameters, GU, 
            ignoreFocal ? _(std::to_string(dataCalib->intrinsics.at<double>(0, 0))) : _(""), 
            wxDefaultPosition, wxDefaultSize, 0, valFocal);
    gu->SetHint("Gu");
    gu->Show(ignoreFocal);
    fgboxParameters->Add(gu);
    wxTextCtrl *gv = new wxTextCtrl(parameters, GV, 
            ignoreFocal ? _(std::to_string(dataCalib->intrinsics.at<double>(1, 1))) : _(""), 
            wxDefaultPosition, wxDefaultSize, 0, valFocal);
    gv->SetHint("Gv");
    gv->Show(ignoreFocal);
    fgboxParameters->Add(gv);

    fillWithVoid(fgboxParameters, parameters, 2);

    // Principal point 
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Principal point :"));
    wxCheckBox* p = new wxCheckBox(parameters, Pref::ID_POINT, "P");
    p->SetValue(!ignorePoint);
    fgboxParameters->Add(p);

    wxTextCtrl *u0 = new wxTextCtrl(parameters, U0, 
            ignorePoint ? _(std::to_string(dataCalib->intrinsics.at<double>(0, 2))) : _(""),
            wxDefaultPosition, wxDefaultSize, 0, valFocal);
    u0->SetHint("u0");
    u0->Show(ignorePoint);
    fgboxParameters->Add(u0);
    wxTextCtrl *v0 = new wxTextCtrl(parameters, V0,
            ignorePoint ? _(std::to_string(dataCalib->intrinsics.at<double>(1, 2))) : _(""),
            wxDefaultPosition, wxDefaultSize, 0, valFocal);
    v0->SetHint("v0");
    v0->Show(ignorePoint);
    fgboxParameters->Add(v0);

    fillWithVoid(fgboxParameters, parameters, 2);

    // Distorsions
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Distorsions :"));
    for (int i = 0; i < NB_OF_K_PARAM; ++i) {
        std::string label = "k" + std::to_string(i + 1);
        wxCheckBox* k = new wxCheckBox(parameters, Pref::ID_K1 + i, label);
        k->SetValue(true);
        fgboxParameters->Add(k);
    }
    vbox->Add(parameters, wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(30);
    
    // Search window size
    wxPanel* search = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    vbox->Add(new wxStaticText(panel, Pref::ID_SEARCH_WINDOW_SIZE, "\t" + Pref::labels[Pref::ID_SEARCH_WINDOW_SIZE]),
            wxEXPAND | wxLEFT | wxRIGHT, 60);

    vbox->AddSpacer(10);
    wxBoxSizer* hboxSearch = new wxBoxSizer(wxHORIZONTAL);
    search->SetSizer(hboxSearch);

    // Getting the number of radioButtons to place
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
    vbox->Add(new wxStaticText(panel, Pref::ID_RENDER_WINDOW_SIZE, "\t" + Pref::labels[Pref::ID_RENDER_WINDOW_SIZE]),
            wxEXPAND | wxLEFT | wxRIGHT, 60);
    vbox->AddSpacer(10);
    wxBoxSizer* hboxRender = new wxBoxSizer(wxHORIZONTAL);
    render->SetSizer(hboxRender);
    
    // Getting the number of radioButtons to place
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
    vbox->Add(new wxStaticText(panel, Pref::ID_CALIB_PROPERTIES, "\t" + Pref::labels[Pref::ID_CALIB_PROPERTIES]),
            wxEXPAND | wxLEFT | wxRIGHT, 60);

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


void PreferencesFrame::UpdateFlags(wxCommandEvent& evt) {
    switch(evt.GetId()) {
        case Pref::ID_FOCAL:
            {   
                ignoreFocal = !ignoreFocal;
                SetOkState(evt);
                wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
                gu->Show(ignoreFocal);
                wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
                gv->Show(ignoreFocal);
                fgboxParameters->Layout();
                parameters->Layout();
                panel->Layout();
                Layout();
                break;
            }
        case Pref::ID_POINT:
            {
                ignorePoint = !ignorePoint;
                SetOkState(evt);
                wxTextCtrl* u0 = (wxTextCtrl*) FindWindowById(U0);
                u0->Show(ignorePoint);
                wxTextCtrl* v0 = (wxTextCtrl*) FindWindowById(V0);
                v0->Show(ignorePoint);
                fgboxParameters->Layout();
                parameters->Layout();
                panel->Layout();
                Layout();
                break;
            }
        case Pref::ID_K1:
            flags ^= cv::CALIB_FIX_K1;
            break;
        case Pref::ID_K2:
            flags ^= cv::CALIB_FIX_K2;
            break;
        case Pref::ID_K3:
            flags ^= cv::CALIB_FIX_K3;
            break;
        case Pref::ID_K4:
            flags ^= cv::CALIB_FIX_K4;
            break;
        case Pref::ID_K5:
            flags ^= cv::CALIB_FIX_K5;
            break;
        case Pref::ID_K6:
            flags ^= cv::CALIB_FIX_K6;
            break;
        default:
            break;
    }
}


void PreferencesFrame::SetOkState(wxCommandEvent& evt) {
    wxButton *b = (wxButton*) FindWindowById(Btn::ID_EXIT_OK);
    if (ignorePoint) {
        wxTextCtrl* u0 = (wxTextCtrl*) FindWindowById(U0);
        wxTextCtrl* v0 = (wxTextCtrl*) FindWindowById(V0);
        if (u0->GetLineText(0).Len() == 0 || v0->GetLineText(0).Len() == 0) {
            b->Enable(false);
            return;
        }
    }
    if (ignoreFocal) {
        wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
        wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
        if (gu->GetLineText(0).Len() == 0 || gv->GetLineText(0).Len() == 0) {
            b->Enable(false);
            return;
        }
    }
    if (nbX->GetLineText(0).Len() == 0 || nbY->GetLineText(0).Len() == 0 ||
        sizeX->GetLineText(0).Len() == 0 || sizeY->GetLineText(0).Len() == 0) {
        b->Enable(false);
            return;
    }
    if (!b->IsEnabled()) {
        b->Enable(true);
    }
}

// --- Utils ---


template<typename T, int size>
int arrLength(T(&)[size]) {
    return size;
}


void fillWithVoid(wxSizer* sizer, wxPanel* panel, int n) {
    for (int i = 0; i < n; ++i) {
        sizer->Add(new wxStaticText(panel, wxID_ANY, ""));
    }
}