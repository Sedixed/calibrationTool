#include <opencv2/opencv.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MyApp: public wxApp {
    public:
        virtual bool OnInit();
};

class MyFrame: public wxFrame {
    wxButton* btn;
    
    public:
        MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    private:
        void OnButtonClick(wxCommandEvent& event);
        wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Load = 1,
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(ID_Load,   MyFrame::OnButtonClick)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("Image Loader", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, 0, title, pos, size) {
    btn = new wxButton(this, ID_Load, "Load");
}

void MyFrame::OnButtonClick(wxCommandEvent& event) {
    std::string wildcard = "PNG and JPG files (*.png;*.jpg)|*.png;*.jpg|BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif";

    wxFileDialog* askImage = new wxFileDialog(this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
                                wxString(wildcard), wxFD_OPEN, wxDefaultPosition);
    if (askImage->ShowModal() == wxID_OK) {
        std::string path = std::string(askImage->GetPath().mb_str());
        cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
        int screen_w = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
        int img_w = img.cols;
        if (img_w > screen_w) {
            float ratio = screen_w / (float) img_w;
            cv::resize(img, img, cv::Size(img.cols * ratio, img.rows * ratio), 0, 0);
        }
        cv::imshow(std::string("Image chosen :"), img);
    }
}
