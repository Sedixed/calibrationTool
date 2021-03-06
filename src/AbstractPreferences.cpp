#include "../headers/AbstractPreferences.hpp"


AbstractPreferences::AbstractPreferences(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib, 
        AppFrame *parent, bool allEnabled)
    : wxFrame(parent, 0, title, pos, size, style) {
    dataCalib = calib;
    flags = dataCalib->pref.parameters_flags;
    searchWindowSize = calib->pref.search_window_size;
    renderWindowSize = calib->pref.render_size;
    this->allEnabled = allEnabled;
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
}


int AbstractPreferences::GenericSetOkState(wxButton* b) {    
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