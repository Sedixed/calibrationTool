#include "../headers/PreferencesPerspective.hpp"
#include "../headers/ButtonsUtils.hpp"
#include <opencv2/opencv.hpp>
#include <wx/valnum.h>


// Number of parameters for distorsion
#define NB_OF_DIST_PARAM 4


// Event table used by the preferences frame.
//  Makes each button corresponds its onclick function, and 
//  same for radioButtons and text updates.
wxBEGIN_EVENT_TABLE(PreferencesPerspectiveFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT_OK,                         PreferencesPerspectiveFrame::OnExitOk)
    EVT_BUTTON(Btn::ID_EXIT_CANCEL,                     PreferencesPerspectiveFrame::OnExitCancel)
    EVT_BUTTON(Pref::CALIB_METHOD_ID,                   PreferencesPerspectiveFrame::ToggleROMode)
    EVT_COMMAND_RANGE(GU, GV, wxEVT_TEXT,               PreferencesPerspectiveFrame::SetOkState)
    EVT_COMMAND_RANGE(NB_X, SIZE_SQUARE_Y, wxEVT_TEXT,  PreferencesPerspectiveFrame::SetOkState)

    EVT_COMMAND_RANGE(Pref::RENDER_MIN_ID, Pref::RENDER_MIN_ID + Pref::RENDER_NB_ID - 1,
        wxEVT_RADIOBUTTON, PreferencesPerspectiveFrame::SetLastRender)

    EVT_COMMAND_RANGE(Pref::SEARCH_MIN_ID, Pref::SEARCH_MIN_ID + Pref::SEARCH_NB_ID - 1,
        wxEVT_RADIOBUTTON, PreferencesPerspectiveFrame::SetLastSearch)

    EVT_COMMAND_RANGE(Pref::Perspective::ID_FOCAL, Pref::Perspective::ID_K1 + NB_OF_DIST_PARAM - 1,
        wxEVT_CHECKBOX, PreferencesPerspectiveFrame::UpdateFlags)
wxEND_EVENT_TABLE()


PreferencesPerspectiveFrame::PreferencesPerspectiveFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib,
        AppFrame* parent, bool allEnabled) : AbstractPreferences(title, pos, size, style, calib, parent, allEnabled) {
    
    // Setting the base values
    iFixedPoint = dataCalib->pref.fixed_point;
    ignoreFocal = (flags & cv::CALIB_FIX_FOCAL_LENGTH);
    ignorePoint = (flags & cv::CALIB_FIX_PRINCIPAL_POINT);
    panel = new wxPanel(this);
    CreateAndPlaceComponents();
}


void PreferencesPerspectiveFrame::OnExitOk(wxCommandEvent& evt) {
    // Saving
    GenericOnExitOk();

    // Calibration type
    dataCalib->pref.fixed_point = iFixedPoint;

    // Focal Length
    if (ignoreFocal) {
        wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
        wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
        double fx;
        if (!gu->GetLineText(0).ToDouble(&fx)) {
            wxMessageBox("Couldn't save Gu.", "Preferences saving", wxICON_ERROR);
            return;
        }
        double fy;
        if (!gv->GetLineText(0).ToDouble(&fy)) {
            wxMessageBox("Couldn't save Gv.", "Preferences saving", wxICON_ERROR);
            return;
        }
        dataCalib->intrinsics.at<double>(0, 0) = fx;
        dataCalib->intrinsics.at<double>(1, 1) = fy;
         // To fix principal point
        cv::Mat img = cv::imread(dataCalib->IOcalib[0].image_name, cv::IMREAD_COLOR);
        dataCalib->intrinsics.at<double>(0, 2) = img.size().width / 2;
        dataCalib->intrinsics.at<double>(1, 2) = img.size().height / 2;
        img.release();
    } else {
        dataCalib->intrinsics.at<double>(0, 0) = 0.0;
        dataCalib->intrinsics.at<double>(1, 1) = 0.0;
    }

    dataCalib->pref.parameters_flags = flags;
    Close(true);
}


void PreferencesPerspectiveFrame::CreateAndPlaceComponents() {
    wxGridBagSizer* box = new wxGridBagSizer(10, 10);

    // Parameters
    box->Add(new wxStaticText(panel, Pref::ID_PARAMETERS, Pref::labels[Pref::ID_PARAMETERS]),
            wxGBPosition(0, 0), wxDefaultSpan, wxEXPAND | wxLEFT | wxTOP, 15); 

    parameters = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
    fgboxParameters = new wxGridBagSizer(10, 10);

    // Focal length
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Generalised focal length :"),
            wxGBPosition(0, 0), wxDefaultSpan, wxALL | wxALIGN_CENTER_VERTICAL, 8);
    wxCheckBox* g = new wxCheckBox(parameters, Pref::Perspective::ID_FOCAL, "G");
    g->SetValue(!ignoreFocal);
    g->Enable(allEnabled);
    fgboxParameters->Add(g, wxGBPosition(0, 1), wxDefaultSpan, wxLEFT | wxTOP | wxALIGN_CENTER_VERTICAL, 8);

    // Validator for focal length input
    wxFloatingPointValidator<float> valParam(3, NULL, wxNUM_VAL_ZERO_AS_BLANK);
    valParam.SetMin(0);


    wxTextCtrl *gu = new wxTextCtrl(parameters, GU, 
            ignoreFocal ? _(std::to_string(dataCalib->intrinsics.at<double>(0, 0))) : _(""), 
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    gu->SetHint("Gu");
    gu->Enable(allEnabled);
    gu->Show(ignoreFocal);
    fgboxParameters->Add(gu, wxGBPosition(0, 2), wxGBSpan(1, 2), wxTOP | wxRIGHT | wxALIGN_CENTER_VERTICAL, 8);

    wxTextCtrl *gv = new wxTextCtrl(parameters, GV, 
            ignoreFocal ? _(std::to_string(dataCalib->intrinsics.at<double>(1, 1))) : _(""), 
            wxDefaultPosition, wxDefaultSize, 0, valParam);
    gv->SetHint("Gv");
    gv->Enable(allEnabled);
    gv->Show(ignoreFocal);
    fgboxParameters->Add(gv, wxGBPosition(0, 4), wxGBSpan(1, 2), wxTOP | wxRIGHT | wxALIGN_CENTER_VERTICAL, 8);


    // Principal point 
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Principal point :"),
            wxGBPosition(1, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL, 8);
    wxCheckBox* p = new wxCheckBox(parameters, Pref::Perspective::ID_POINT, "P");
    p->SetValue(!ignorePoint);
    p->Enable(allEnabled);
    fgboxParameters->Add(p, wxGBPosition(1, 1), wxDefaultSpan, wxLEFT, 8);

    // Distorsions
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Distorsions :"),
            wxGBPosition(2, 0), wxDefaultSpan, wxLEFT | wxALIGN_CENTER_VERTICAL, 8);
    
    const int KFixId[] = {cv::CALIB_FIX_K1,
                          cv::CALIB_FIX_K2,
                          cv::CALIB_ZERO_TANGENT_DIST,
                          cv::CALIB_FIX_K3};
    for (int i = 0; i < NB_OF_DIST_PARAM; ++i) {
        std::string label;
        if (i == NB_OF_DIST_PARAM - 2) {
            label = "p1, p2";
        } else {
            label = "k" + std::to_string(i == NB_OF_DIST_PARAM - 1 ? i : i + 1);
        }
        wxCheckBox* k = new wxCheckBox(parameters, Pref::Perspective::ID_K1 + i, label);
        k->SetValue(!(dataCalib->pref.parameters_flags & KFixId[i]));
        k->Enable(allEnabled);
        int flag = (i == 0) ? wxLEFT : 0;
        fgboxParameters->Add(k, wxGBPosition(2, (i + 1)), wxDefaultSpan, wxRIGHT | wxALIGN_CENTER_VERTICAL | flag, 8);
    }

    // Calibration method
    fgboxParameters->Add(new wxStaticText(parameters, wxID_ANY, "Calibration mehod :"),
            wxGBPosition(3, 0), wxDefaultSpan, wxLEFT | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 8);
    std::string label = (iFixedPoint == 0) ? "Default" : "RO";
    wxButton* tgb = new wxButton(parameters, Pref::CALIB_METHOD_ID, _(label));
    tgb->Enable(allEnabled);
    fgboxParameters->Add(tgb, wxGBPosition(3, 1), wxGBSpan(1, 2), wxSHAPED | wxLEFT | wxBOTTOM, 8);

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


void PreferencesPerspectiveFrame::UpdateFlags(wxCommandEvent& evt) {
    switch(evt.GetId()) {
        case Pref::Perspective::ID_FOCAL:
            {   
                ignoreFocal = !ignoreFocal;
                flags ^= cv::CALIB_FIX_FOCAL_LENGTH;
                flags ^= cv::CALIB_USE_INTRINSIC_GUESS;
                SetOkState(evt);
                wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
                gu->Show(ignoreFocal);
                wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
                gv->Show(ignoreFocal);
                fgboxParameters->SetSizeHints(parameters);
                panel->Layout();
                break;
            }
        case Pref::Perspective::ID_POINT:
            {   
                ignorePoint = !ignorePoint;
                flags ^= cv::CALIB_FIX_PRINCIPAL_POINT;
                break;
            }
        case Pref::Perspective::ID_K1:
            flags ^= cv::CALIB_FIX_K1;
            break;
        case Pref::Perspective::ID_K2:
            flags ^= cv::CALIB_FIX_K2;
            break;
        case Pref::Perspective::ID_K3:
            flags ^= cv::CALIB_FIX_K3;
            break;
        case Pref::Perspective::ID_P1P2:
            flags ^= cv::CALIB_ZERO_TANGENT_DIST;
            break;
        default:
            break;
    }
}


void PreferencesPerspectiveFrame::SetOkState(wxCommandEvent& evt) {
    wxButton *b = (wxButton*) FindWindowById(Btn::ID_EXIT_OK);
    // Common to all calibration types
    if (GenericSetOkState(b) != 0) {
        return;
    }

    if (ignoreFocal) {
        wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
        wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
        if (gu->GetLineLength(0) == 0 || gv->GetLineLength(0) == 0) {
            b->Enable(false);
            return;
        }
    }

    if (!b->IsEnabled()) {
        b->Enable(true);
    }
}


void PreferencesPerspectiveFrame::ToggleROMode(wxCommandEvent& evt) {
    wxButton *b = (wxButton*) FindWindowById(Pref::CALIB_METHOD_ID);
    iFixedPoint = 1 - iFixedPoint;
    if (b != NULL) {
        b->SetLabel(iFixedPoint == 0 ? "Default" : "RO");
    }
}


