#include "../headers/AppFrame.hpp"

#ifdef WINDOWS  
    #define SIZE wxSize(300, 340)
#else
    #define SIZE wxSize(300, 500)
#endif


class App : public wxApp {
    public:
        virtual bool OnInit();
};

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    long style = wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX);
    AppFrame *frame = new AppFrame("Camera calibration tool", wxPoint(-1, -1), SIZE, style);
    frame->Show(true);
    return true;
}