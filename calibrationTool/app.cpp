#include "appFrame.hpp"


class App : public wxApp {
    public:
        virtual bool OnInit();
};


wxIMPLEMENT_APP(App);

bool App::OnInit() {
    AppFrame *frame = new AppFrame("Camera calibration tool", wxPoint(-1, -1), wxSize(300, 500));
    frame->Show(true);
    return true;
}