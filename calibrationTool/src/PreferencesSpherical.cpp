#include "../headers/PreferencesSpherical.hpp"
#include "../headers/ButtonsUtils.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/ccalib/omnidir.hpp>
#include <wx/valnum.h>


// IDs for wxTextCtrl (specific to spherical calibration)
#define A 84
#define XI 85

// Number of K parameters for distorsion
#define NB_OF_K_PARAM 5


// Event table used by the preferences frame.
//  Makes each button corresponds its onclick function, and 
//  same for radioButtons and text updates.
wxBEGIN_EVENT_TABLE(PreferencesSphericalFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT_OK,                         PreferencesSphericalFrame::OnExitOk)
    EVT_BUTTON(Btn::ID_EXIT_CANCEL,                     PreferencesSphericalFrame::OnExitCancel)
    EVT_COMMAND_RANGE(GU, XI, wxEVT_TEXT,               PreferencesSphericalFrame::SetOkState)
    EVT_COMMAND_RANGE(NB_X, SIZE_SQUARE_Y, wxEVT_TEXT,  PreferencesSphericalFrame::SetOkState)

    EVT_COMMAND_RANGE(Pref::RENDER_MIN_ID, Pref::RENDER_MIN_ID + Pref::RENDER_NB_ID - 1,
        wxEVT_RADIOBUTTON, PreferencesSphericalFrame::SetLastRender)

    EVT_COMMAND_RANGE(Pref::SEARCH_MIN_ID, Pref::SEARCH_MIN_ID + Pref::SEARCH_NB_ID - 1,
        wxEVT_RADIOBUTTON, PreferencesSphericalFrame::SetLastSearch)

    EVT_COMMAND_RANGE(Pref::Spherical::ID_FOCAL, Pref::Spherical::ID_K1 + NB_OF_K_PARAM - 1,
        wxEVT_CHECKBOX, PreferencesSphericalFrame::UpdateFlags)
wxEND_EVENT_TABLE()


PreferencesSphericalFrame::PreferencesSphericalFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib, AppFrame* parent)
    : AbstractPreferences(title, pos, size, style, calib, parent) {
    
    // Setting the base values
    ignoreSkew = (flags & cv::omnidir::CALIB_FIX_SKEW);
    ignoreXi = (flags & cv::omnidir::CALIB_FIX_XI);
    ignorePoint = (flags & cv::omnidir::CALIB_FIX_CENTER);
    panel = new wxPanel(this);
    CreateAndPlaceComponents();
}


void PreferencesSphericalFrame::OnExitOk(wxCommandEvent& evt) {
    // Saving
    GenericOnExitOk();

    if (ignoreSkew) {
        flags |= cv::omnidir::CALIB_FIX_SKEW;
        wxTextCtrl *skew = (wxTextCtrl*) FindWindowById(A);
        double a;
        if (!skew->GetLineText(0).ToDouble(&a)) {
            wxMessageBox("Couldn't save a.", "Preferences saving", wxICON_ERROR);
            return;
        }
        dataCalib->intrinsics.at<double>(0, 1) = a;
    } else {
        flags &= ~(cv::omnidir::CALIB_FIX_SKEW);
        dataCalib->intrinsics.at<double>(0, 1) = 0;
    }

    if (ignoreXi) {
        flags |= cv::omnidir::CALIB_FIX_XI;
        wxTextCtrl *xiText = (wxTextCtrl*) FindWindowById(XI);
        double xi;
        if (!xiText->GetLineText(0).ToDouble(&xi)) {
            wxMessageBox("Couldn't save Xi.", "Preferences saving", wxICON_ERROR);
            return;
        }
        dataCalib->Xi = xi;
    } else {
        flags &= ~(cv::omnidir::CALIB_FIX_XI);
    }

    // Principal point
    if (ignorePoint) {
        flags |= cv::omnidir::CALIB_FIX_CENTER;
        wxTextCtrl* u0 = (wxTextCtrl*) FindWindowById(U0);
        wxTextCtrl* v0 = (wxTextCtrl*) FindWindowById(V0);
        double cx;
        if (!u0->GetLineText(0).ToDouble(&cx)) {
            wxMessageBox("Couldn't save u0.", "Preferences saving", wxICON_ERROR);
            return;
        }
        double cy;
        if (!v0->GetLineText(0).ToDouble(&cy)) {
            wxMessageBox("Couldn't save v0.", "Preferences saving", wxICON_ERROR);
            return;
        }
        dataCalib->intrinsics.at<double>(0, 2) = cx;
        dataCalib->intrinsics.at<double>(1, 2) = cy;
    } else {
        // Center of the image
        cv::Mat img = cv::imread(dataCalib->IOcalib[0].image_name, cv::IMREAD_COLOR);
        flags &= ~(cv::CALIB_FIX_PRINCIPAL_POINT);
        dataCalib->intrinsics.at<double>(0, 2) = img.size().width / 2;
        dataCalib->intrinsics.at<double>(1, 2) = img.size().height / 2;
        img.release();
    }


    // Calibration flags
    // maybe un &&, peut être contraindre de fixer les 2 ou 0
    if (ignorePoint || ignoreFocal) {
        // eq. to cv::omnidir::CALIB_USE_GUESS
        flags |= cv::CALIB_USE_INTRINSIC_GUESS;
    } else {
        flags &= ~(cv::CALIB_USE_INTRINSIC_GUESS);
    }
    

    dataCalib->pref.parameters_flags = flags;
    Close(true);
}


void PreferencesSphericalFrame::CreateAndPlaceComponents() {
    wxGridBagSizer* box = new wxGridBagSizer(10, 10);

    // Parameters
    box->Add(new wxStaticText(panel, Pref::ID_PARAMETERS, Pref::labels[Pref::ID_PARAMETERS]),
            wxGBPosition(0, 0), wxDefaultSpan, wxEXPAND | wxLEFT | wxTOP, 15); 

    parameters = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    fgboxParameters = new wxGridBagSizer(10, 10);

    // Focal length
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Generalised focal length :"),
            wxGBPosition(0, 0), wxDefaultSpan, wxALL | wxALIGN_CENTER_VERTICAL, 8);
    wxCheckBox* g = new wxCheckBox(parameters, Pref::Spherical::ID_FOCAL, "G");
    g->SetValue(!ignoreFocal);
    fgboxParameters->Add(g, wxGBPosition(0, 1), wxDefaultSpan, wxLEFT | wxTOP, 8);

    // Validator for parameters input
    wxFloatingPointValidator<float> valParam(3, NULL, wxNUM_VAL_ZERO_AS_BLANK);
    valParam.SetMin(0);


    wxTextCtrl *gu = new wxTextCtrl(parameters, GU, 
            ignoreFocal ? _(std::to_string(dataCalib->intrinsics.at<double>(0, 0))) : _(""), 
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    gu->SetHint("Gu");
    gu->Show(ignoreFocal);
    fgboxParameters->Add(gu, wxGBPosition(0, 2), wxGBSpan(1, 2), wxTOP | wxRIGHT, 8);

    wxTextCtrl *gv = new wxTextCtrl(parameters, GV, 
            ignoreFocal ? _(std::to_string(dataCalib->intrinsics.at<double>(1, 1))) : _(""), 
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    gv->SetHint("Gv");
    gv->Show(ignoreFocal);
    fgboxParameters->Add(gv, wxGBPosition(0, 4), wxGBSpan(1, 2), wxTOP | wxRIGHT, 8);


    // Principal point 
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Principal point :"),
            wxGBPosition(1, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL, 8);
    wxCheckBox* p = new wxCheckBox(parameters, Pref::Spherical::ID_POINT, "P");
    p->SetValue(!ignorePoint);
    fgboxParameters->Add(p, wxGBPosition(1, 1), wxDefaultSpan, wxLEFT, 8);

    wxTextCtrl* u0 = new wxTextCtrl(parameters, U0, 
            ignorePoint ? _(std::to_string(dataCalib->intrinsics.at<double>(0, 2))) : _(""),
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    u0->SetHint("u0");
    u0->Show(ignorePoint);
    fgboxParameters->Add(u0, wxGBPosition(1, 2), wxGBSpan(1, 2), wxRIGHT, 8);
    wxTextCtrl *v0 = new wxTextCtrl(parameters, V0,
            ignorePoint ? _(std::to_string(dataCalib->intrinsics.at<double>(1, 2))) : _(""),
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    v0->SetHint("v0");
    v0->Show(ignorePoint);
    fgboxParameters->Add(v0, wxGBPosition(1, 4), wxGBSpan(1, 2), wxRIGHT, 8);

    // Skew
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Skew :"),
            wxGBPosition(2, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL, 8);
    wxCheckBox* skew = new wxCheckBox(parameters, Pref::Spherical::ID_SKEW, "a");
    skew->SetValue(!ignoreSkew);
    fgboxParameters->Add(skew, wxGBPosition(2, 1), wxDefaultSpan, wxLEFT, 8);
    wxTextCtrl* a = new wxTextCtrl(parameters, A, 
            ignoreSkew ? _(std::to_string(dataCalib->intrinsics.at<double>(0, 1))) : _(""),
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    a->SetHint("a");
    a->Show(ignoreSkew);
    fgboxParameters->Add(a, wxGBPosition(2, 2), wxGBSpan(1, 2), wxRIGHT, 8);

    // Xi
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Xi :"),
            wxGBPosition(3, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL, 8);
    wxCheckBox* xi = new wxCheckBox(parameters, Pref::Spherical::ID_XI, "Xi");
    xi->SetValue(!ignoreXi);
    fgboxParameters->Add(xi, wxGBPosition(3, 1), wxDefaultSpan, wxLEFT, 8);
    wxTextCtrl* xiText = new wxTextCtrl(parameters, XI, 
            ignoreXi ? _(std::to_string(dataCalib->Xi)) : _(""),
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    xiText->SetHint("Xi");
    xiText->Show(ignoreXi);
    fgboxParameters->Add(xiText, wxGBPosition(3, 2), wxGBSpan(1, 2), wxRIGHT, 8);

    // Distorsions
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Distorsions :"),
            wxGBPosition(4, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL | wxBOTTOM, 8);
    
    const int KFixId[] = {cv::omnidir::CALIB_FIX_K1,
                          cv::omnidir::CALIB_FIX_K2,
                          cv::CALIB_FIX_K3,
                          cv::CALIB_FIX_K4,
                          cv::CALIB_FIX_K5};
    for (int i = 0; i < NB_OF_K_PARAM; ++i) {
        std::string label = "k" + std::to_string(i + 1);
        wxCheckBox* k = new wxCheckBox(parameters, Pref::Spherical::ID_K1 + i, label);
        k->SetValue(!(dataCalib->pref.parameters_flags & KFixId[i]));
        int flag = (i == 0) ? wxLEFT : 0;
        fgboxParameters->Add(k, wxGBPosition(4, (i + 1)), wxDefaultSpan, wxRIGHT | flag | wxBOTTOM, 8);
    }

    fgboxParameters->SetSizeHints(parameters);
    parameters->SetSizer(fgboxParameters);
    box->Add(parameters, wxGBPosition(1, 0), wxDefaultSpan, wxEXPAND | wxLEFT, 20);

    // Search window size
    wxPanel* search = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    box->Add(new wxStaticText(panel, Pref::ID_SEARCH_WINDOW_SIZE, Pref::labels[Pref::ID_SEARCH_WINDOW_SIZE]),
            wxGBPosition(2, 0), wxDefaultSpan, wxEXPAND | wxLEFT, 15); 

    wxGridBagSizer *boxSearch = new wxGridBagSizer(10, 10);
    int size = arrLength<const int>(Pref::searchSizes);
    for (int i = 0; i < size; ++i) {
        int value = Pref::searchSizes[i];
        std::string label = std::to_string(value) + "x" + std::to_string(value);
        wxRadioButton *b = new wxRadioButton(search, Pref::searchId[i], _(label), 
                    wxDefaultPosition, wxDefaultSize, i == 0 ? wxRB_GROUP : 0);
        if (value == dataCalib->pref.search_window_size) {
            b->SetValue(true);
        }
        boxSearch->Add(b, wxGBPosition(0, i), wxDefaultSpan, wxALL, 8);
    }
    boxSearch->SetSizeHints(search);
    search->SetSizer(boxSearch);
    box->Add(search, wxGBPosition(3, 0), wxDefaultSpan, wxEXPAND | wxLEFT, 20);

    // Render window size
    wxPanel* render = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    box->Add(new wxStaticText(panel, Pref::ID_RENDER_WINDOW_SIZE, Pref::labels[Pref::ID_RENDER_WINDOW_SIZE]),
            wxGBPosition(4, 0), wxDefaultSpan, wxEXPAND | wxLEFT, 15);

    wxGridBagSizer* boxRender = new wxGridBagSizer(10, 10);
    
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
        boxRender->Add(b, wxGBPosition(0, i), wxDefaultSpan, wxALL, 8);
    }
    boxRender->SetSizeHints(render);
    render->SetSizer(boxRender);
    box->Add(render, wxGBPosition(5, 0), wxDefaultSpan, wxEXPAND | wxLEFT, 20);

    // Calibration pattern properties
    wxFloatingPointValidator<float> valFloat(2, NULL, wxNUM_VAL_ZERO_AS_BLANK);
    valFloat.SetRange(0, 100);

    wxIntegerValidator<int> valInt(NULL, wxNUM_VAL_ZERO_AS_BLANK);
    valInt.SetRange(1, 100);

    wxPanel* properties = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    box->Add(new wxStaticText(panel, Pref::ID_CALIB_PROPERTIES, Pref::labels[Pref::ID_CALIB_PROPERTIES]),
            wxGBPosition(6, 0), wxDefaultSpan, wxEXPAND | wxLEFT, 15);

    wxGridBagSizer *boxProp = new wxGridBagSizer(10, 10);

    boxProp->Add(new wxStaticText(properties, wxID_ANY, "Number of squares along X :"),
            wxGBPosition(0, 0), wxDefaultSpan, wxLEFT | wxTOP | wxALIGN_CENTER_VERTICAL, 8);
    nbX = new wxTextCtrl(properties, NB_X, _(std::to_string(dataCalib->calibPattern.nbSquareX)), 
                        wxDefaultPosition, wxDefaultSize, 0, valInt);
    boxProp->Add(nbX, wxGBPosition(0, 1), wxDefaultSpan, wxTOP | wxLEFT | wxRIGHT, 8);

    boxProp->Add(new wxStaticText(properties, wxID_ANY, "Number of squares along Y :"),
            wxGBPosition(1, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL, 8);
    nbY = new wxTextCtrl(properties, NB_Y, _(std::to_string(dataCalib->calibPattern.nbSquareY)),
                        wxDefaultPosition, wxDefaultSize, 0, valInt);
    boxProp->Add(nbY, wxGBPosition(1, 1), wxDefaultSpan, wxLEFT | wxRIGHT , 8);

    std::string ssx = std::to_string(dataCalib->calibPattern.sizeSquareX);
    ssx = ssx.substr(0, ssx.find(".,") + 6);
    boxProp->Add(new wxStaticText(properties, wxID_ANY, "Size of each square along X (mm) :"),
            wxGBPosition(2, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL, 8);
    sizeX = new wxTextCtrl(properties, wxID_ANY, _(ssx), wxDefaultPosition, wxDefaultSize, 0, valFloat);
    boxProp->Add(sizeX, wxGBPosition(2, 1), wxDefaultSpan, wxLEFT | wxRIGHT, 8);

    std::string ssy = std::to_string(dataCalib->calibPattern.sizeSquareY);
    ssy = ssy.substr(0, ssy.find(".,") + 6);
    boxProp->Add(new wxStaticText(properties, wxID_ANY, "Size of each square along Y (mm) :"),
            wxGBPosition(3, 0), wxDefaultSpan, wxLEFT | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 8);
    sizeY = new wxTextCtrl(properties, wxID_ANY, _(ssy), wxDefaultPosition, wxDefaultSize, 0, valFloat);
    boxProp->Add(sizeY, wxGBPosition(3, 1), wxDefaultSpan, wxBOTTOM | wxLEFT | wxRIGHT, 8);

    boxProp->SetSizeHints(properties);
    properties->SetSizer(boxProp);
    box->Add(properties, wxGBPosition(7, 0), wxDefaultSpan, wxEXPAND | wxLEFT, 20);

    // Exit buttons
    wxPanel* btns = new wxPanel(panel);
    wxGridBagSizer* btnBox = new wxGridBagSizer(10, 10);
    
    btnBox->Add(Btn::btnFromId(Btn::ID_EXIT_CANCEL, btns), wxGBPosition(0, 0), wxDefaultSpan, wxALL, 10);
    btnBox->Add(Btn::btnFromId(Btn::ID_EXIT_OK, btns), wxGBPosition(0, 1), wxDefaultSpan, wxALL, 10);
    box->Add(btns, wxGBPosition(8, 0), wxDefaultSpan, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 10);

    btnBox->SetSizeHints(btns);
    btns->SetSizer(btnBox);

    box->SetSizeHints(panel);
    panel->SetSizer(box);
}


void PreferencesSphericalFrame::UpdateFlags(wxCommandEvent& evt) {
    switch(evt.GetId()) {
        case Pref::Spherical::ID_FOCAL:
        {   
            ignoreFocal = !ignoreFocal;
            SetOkState(evt);
            wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
            gu->Show(ignoreFocal);
            wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
            gv->Show(ignoreFocal);
            fgboxParameters->SetSizeHints(parameters);
            panel->Layout();
            break;
        }
        case Pref::Spherical::ID_POINT:
        {   
            ignorePoint = !ignorePoint;
            SetOkState(evt);
            wxTextCtrl* u0 = (wxTextCtrl*) FindWindowById(U0);
            u0->Show(ignorePoint);
            wxTextCtrl* v0 = (wxTextCtrl*) FindWindowById(V0);
            v0->Show(ignorePoint);
            fgboxParameters->SetSizeHints(parameters);
            panel->Layout();
            break;
        }
        case Pref::Spherical::ID_SKEW:
        {
            ignoreSkew = !ignoreSkew;
            SetOkState(evt);
            wxTextCtrl* skew = (wxTextCtrl*) FindWindowById(A);
            skew->Show(ignoreSkew);
            fgboxParameters->SetSizeHints(parameters);
            panel->Layout();
            break;
        }
        case Pref::Spherical::ID_XI:
        {
            ignoreXi = !ignoreXi;
            SetOkState(evt);
            wxTextCtrl* xi = (wxTextCtrl*) FindWindowById(XI);
            xi->Show(ignoreXi);
            fgboxParameters->SetSizeHints(parameters);
            panel->Layout();
            break;
        }
        case Pref::Spherical::ID_K1:
            flags ^= cv::omnidir::CALIB_FIX_K1;
            break;
        case Pref::Spherical::ID_K2:
            flags ^= cv::omnidir::CALIB_FIX_K2;
            break;
        case Pref::Spherical::ID_K3:
            flags ^= cv::CALIB_FIX_K3;
            break;
        case Pref::Spherical::ID_K4:
            flags ^= cv::CALIB_FIX_K4;
            break;
        case Pref::Spherical::ID_K5:
            flags ^= cv::CALIB_FIX_K5;
            break;
        case Pref::Spherical::ID_K6:
            flags ^= cv::CALIB_FIX_K6;
            break;
        default:
            break;

    }
    
}


void PreferencesSphericalFrame::SetOkState(wxCommandEvent& evt) {
    wxButton *b = (wxButton*) FindWindowById(Btn::ID_EXIT_OK);
    // Common to all calibration types
    if (GenericSetOkState(b) != 0) {
        return;
    }

    if (ignoreSkew) {
        wxTextCtrl* skew = (wxTextCtrl*) FindWindowById(A);
        if (skew->GetLineText(0).Len() == 0) {
            b->Enable(false);
            return;
        }
    }
    if (ignoreXi) {
        wxTextCtrl* xi = (wxTextCtrl*) FindWindowById(XI);
        if (xi->GetLineText(0).Len() == 0) {
            b->Enable(false);
            return;
        }
    }

    if (!b->IsEnabled()) {
        b->Enable(true);
    }
}