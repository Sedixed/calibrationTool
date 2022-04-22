#include "../headers/AbstractPreferences.hpp"


AbstractPreferences::AbstractPreferences(const wxString& title, const wxPoint& pos, const wxSize& size, long style, Calib* calib, AppFrame *parent)
    : wxFrame(parent, 0, title, pos, size, style) {
    dataCalib = calib;
    flags = dataCalib->pref.parameters_flags;
    searchWindowSize = calib->pref.search_window_size;
    renderWindowSize = calib->pref.render_size;

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


// --- Utils ---

template<typename T, int size>
int arrLength(T(&)[size]) {
    return size;
}