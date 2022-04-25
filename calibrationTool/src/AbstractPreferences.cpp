#include "../headers/AbstractPreferences.hpp"


AbstractPreferences::AbstractPreferences(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib, AppFrame *parent)
    : wxFrame(parent, 0, title, pos, size, style) {
    dataCalib = calib;
    flags = dataCalib->pref.parameters_flags;
    searchWindowSize = calib->pref.search_window_size;
    renderWindowSize = calib->pref.render_size;
    ignoreFocal = (flags & cv::CALIB_FIX_FOCAL_LENGTH);
    ignorePoint = (flags & cv::CALIB_FIX_PRINCIPAL_POINT);

}


void AbstractPreferences::OnExitCancel(wxCommandEvent& evt) {
    Close(true);
}


void AbstractPreferences::SetLastSearch(wxCommandEvent& evt) {
    for (int i = 0; i < arrLength<const int>(Pref::searchSizes); ++i) {
        if (Pref::searchId[i] == evt.GetId()) {
            searchWindowSize = Pref::searchSizes[i];
            break;
        }
    }
}


void AbstractPreferences::SetLastRender(wxCommandEvent& evt) {
    for (int i = 0; i < arrLength<const cv::Size>(Pref::renderSizes); ++i) {
        if (Pref::renderId[i] == evt.GetId()) {
            renderWindowSize = Pref::renderSizes[i];
            break;
        }
    }
}


void AbstractPreferences::GenericOnExitOk() {
    long n;
    double m;
    // Number of squares along X
    wxString text = nbX->GetValue();
    if (!text.ToLong(&n) || n <= 3) {
        wxMessageBox("Invalid number of squares along X.", "Preferences saving", wxICON_ERROR);
        return;
    }
    dataCalib->calibPattern.nbSquareX = (int) n;

    // Number of squares along Y
    text = nbY->GetValue();
    if (!text.ToLong(&n) || n <= 3) {
        wxMessageBox("Invalid number of squares along Y.", "Preferences saving", wxICON_ERROR);
        return;
    }
    dataCalib->calibPattern.nbSquareY = (int) n;

    // Size of each square along X
    text = sizeX->GetValue();
    if (!text.ToDouble(&m)) {
        wxMessageBox("Invalid size of each square along X.", "Preferences saving", wxICON_ERROR);
        return;
    }
    dataCalib->calibPattern.sizeSquareX = m;

    // Size of each square along Y
    text = sizeY->GetValue();
    if (!text.ToDouble(&m)) {
        wxMessageBox("Invalid size of each square along Y.", "Preferences saving", wxICON_ERROR);
        return;
    }
    dataCalib->calibPattern.sizeSquareY = m;

    dataCalib->pref.render_size = renderWindowSize;
    dataCalib->pref.search_window_size = searchWindowSize;

    // Saving user defined intrinsics parameters if provided

    // Focal Length
    if (ignoreFocal) {
        flags |= cv::CALIB_FIX_FOCAL_LENGTH;
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
    } else {
        flags &= ~(cv::CALIB_FIX_FOCAL_LENGTH);
        dataCalib->intrinsics.at<double>(0, 0) = 0.0;
        dataCalib->intrinsics.at<double>(1, 1) = 0.0;
    }

    // Principal point
    if (ignorePoint) {
        flags |= cv::CALIB_FIX_PRINCIPAL_POINT;
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
}


int AbstractPreferences::GenericSetOkState(wxButton* b) {
    if (ignorePoint) {
        wxTextCtrl* u0 = (wxTextCtrl*) FindWindowById(U0);
        wxTextCtrl* v0 = (wxTextCtrl*) FindWindowById(V0);
        if (u0->GetLineLength(0) == 0 || v0->GetLineLength(0) == 0) {
            b->Enable(false);
            return -1;
        }
    }
    if (ignoreFocal) {
        wxTextCtrl* gu = (wxTextCtrl*) FindWindowById(GU);
        wxTextCtrl* gv = (wxTextCtrl*) FindWindowById(GV);
        if (gu->GetLineLength(0) == 0 || gv->GetLineLength(0) == 0) {
            b->Enable(false);
            return -1;
        }
    }
    if (nbX->GetLineLength(0) == 0 || nbY->GetLineLength(0) == 0 ||
        sizeX->GetLineLength(0) == 0 || sizeY->GetLineLength(0) == 0) {
        b->Enable(false);
            return -1;
    }
    return 0;
}



// --- Utils ---

template<typename T, int size>
int arrLength(T(&)[size]) {
    return size;
}