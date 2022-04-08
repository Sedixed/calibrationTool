#include "../headers/Preferences.hpp"
#include "../headers/ButtonsUtils.hpp"
#include <opencv2/opencv.hpp>

// Size of the window for the corners detection
#define SEARCH_WINDOW_SIZE_1 3
#define SEARCH_WINDOW_SIZE_2 5
#define SEARCH_WINDOW_SIZE_3 9
#define SEARCH_WINDOW_SIZE_4 15
#define SEARCH_WINDOW_SIZE_5 21

// Render window size
#define RDR_WINDOW_SIZE_1 cv::Size(800, 600)
#define RDR_WINDOW_SIZE_2 cv::Size(1024, 768)
#define RDR_WINDOW_SIZE_3 cv::Size(1152, 864)
#define RDR_WINDOW_SIZE_4 cv::Size(1280, 1024)


// Event table used by the preferences frame.
//  Makes each button corresponds its onclick function
wxBEGIN_EVENT_TABLE(PreferencesFrame, wxFrame)
    EVT_BUTTON(Btn::ID_EXIT_OK,                 PreferencesFrame::OnExitOk)
    EVT_BUTTON(Btn::ID_EXIT_CANCEL,             PreferencesFrame::OnExitCancel)
wxEND_EVENT_TABLE()

PreferencesFrame::PreferencesFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(NULL, 0, title, pos, size, style) {

    

}

void PreferencesFrame::OnExitOk(wxCommandEvent& evt) {
    Close(true);
}

void PreferencesFrame::OnExitCancel(wxCommandEvent& evt) {
    Close(true);
}