#include "../headers/CalibrationResults.hpp"
#include <iomanip>
#include <opencv2/ccalib/omnidir.hpp>
#include <wx/gbsizer.h>


#ifdef WINDOWS
    #define SIZE wxSize(660, 500)
#else
    #define SIZE wxSize(640, 500)
#endif

#define GREEN wxColour(57, 240, 119)
#define RED wxColour(237, 67, 55)


int CalibrationResults(Calib *dataCalib, wxWindow* parent) {
    cv::Mat cameraMatrix = dataCalib->intrinsics;
    cv::Mat distCoeffs = dataCalib->distCoeffs;

    // Step useless if it is perspective type
    if (dataCalib->type != PERSPECTIVE_TYPE) {
        for (int i = 0; i < dataCalib->nb_images; ++i) {
            if (!dataCalib->IOcalib[i].active_image) {
                continue;
            }
            std::vector<cv::Point3f> objPoints = dataCalib->IOcalib[i].CornersCoord3D; // 3D Points
            cv::Mat rVec = dataCalib->IOcalib[i].rotationMat;                          // Rotation matrix          
            cv::Mat tVec = dataCalib->IOcalib[i].translationMat;                       // Translation matrix
            std::vector<cv::Point2f> imgPointsOutput;                                  // Output 2D points

            switch(dataCalib->type) {
                case SPHERICAL_TYPE:
                {   
                    cv::omnidir::projectPoints(objPoints, imgPointsOutput, rVec, tVec, cameraMatrix, dataCalib->Xi, dataCalib->distCoeffs);
                    // Error per view calculus
                    double err = cv::norm(cv::Mat(dataCalib->IOcalib[i].CornersCoord2D), cv::Mat(imgPointsOutput), cv::NORM_L2);
                    int n = (int) objPoints.size();
                    dataCalib->IOcalib[i].errorView = (float) std::sqrt(err * err / n);
                    break;
                }
                default:
                    wxMessageBox("Error : unknown calibration type.", "Error", wxICON_ERROR);
                    return -1;
            }
        }
    }
    

    // Results display
    long style = wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX);

    wxFrame* resultsFrame = new wxFrame(parent, wxID_ANY, _("Calibration results"), wxPoint(-1, -1), SIZE, style);
    wxScrolledWindow* panel = new wxScrolledWindow(resultsFrame);
    panel->SetScrollbars(0, 10, 0, 1000);
    wxGridBagSizer* box = new wxGridBagSizer(10, 10);
    box->Add(new wxStaticText(panel, wxID_ANY, "Calibration summary"), wxGBPosition(0, 0), wxDefaultSpan,
        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 16);

    wxPanel *summaryPanel = new wxPanel(panel, -1, wxDefaultPosition, 
        wxDefaultSize, wxSUNKEN_BORDER);
    wxGridBagSizer* sumBox = new wxGridBagSizer(10, 10);

    // Focal length
    double fx = dataCalib->intrinsics.at<double>(0, 0);
    double fy = dataCalib->intrinsics.at<double>(1, 1);
    std::string fl = dataCalib->type == PERSPECTIVE_TYPE ? "Focal length :" : "Generalised focal length :";
    sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
        _(fl)), wxGBPosition(0, 0), wxDefaultSpan,
        wxALL | wxEXPAND, 8);
    sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY,
        _(std::to_string(fx) + "  " + std::to_string(fy))), wxGBPosition(0, 1), wxDefaultSpan,
        wxALL | wxEXPAND, 8);

    // Principal point
    double cx = dataCalib->intrinsics.at<double>(0, 2);
    double cy = dataCalib->intrinsics.at<double>(1, 2);
    sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
        _("Principal point :")), wxGBPosition(1, 0), wxDefaultSpan,
        wxALL | wxEXPAND, 8);
    sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
        _(std::to_string(cx) + "  " + std::to_string(cy))), wxGBPosition(1, 1), wxDefaultSpan,
        wxALL | wxEXPAND, 8);
    
    int nextRow;
    switch(dataCalib->type) {
        // ------------------------------------
        // --- Perspective calibration case ---
        // ------------------------------------
        case PERSPECTIVE_TYPE:
        {   
            // Distorsions
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _("Distorsions :")), wxGBPosition(2, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            std::string distCoeffs = "";
            for (int j = 0; j < dataCalib->distCoeffs.cols; ++j) {
                distCoeffs += std::to_string(dataCalib->distCoeffs.at<double>(0, j)) + "  ";
            }
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _(distCoeffs)), wxGBPosition(2, 1), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            
            // Method used
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _("Method used :")), wxGBPosition(3, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            std::string method = dataCalib->pref.fixed_point == 0 ? "calibrateCamera" : "calibrateCameraRO";
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _(method)), wxGBPosition(3, 1), wxDefaultSpan,
            wxALL | wxEXPAND, 8);

            nextRow = 4;
            break;
        }
        // ----------------------------------
        // --- Spherical calibration case ---
        // ----------------------------------
        case SPHERICAL_TYPE:
        {   
            // Skew
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _("Skew :")), wxGBPosition(2, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _(std::to_string(dataCalib->intrinsics.at<double>(0, 1)))), wxGBPosition(2, 1), wxDefaultSpan,
            wxALL | wxEXPAND, 8);

            // Xi
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _("Xi :")), wxGBPosition(3, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _(std::to_string(dataCalib->Xi))), wxGBPosition(3, 1), wxDefaultSpan,
            wxALL | wxEXPAND, 8);

            // Distorsions
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _("Distorsions :")), wxGBPosition(4, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            std::string distCoeffs = "";
            for (int j = 0; j < dataCalib->distCoeffs.cols; ++j) {
                distCoeffs += std::to_string(dataCalib->distCoeffs.at<double>(0, j)) + "  ";
            }
            sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _(distCoeffs)), wxGBPosition(4, 1), wxDefaultSpan,
            wxALL | wxEXPAND, 8);

            nextRow = 5;
            break;
        }
        default:
            wxMessageBox("Error : unknown calibration type.", "Error", wxICON_ERROR);
            return -1;
    }

    // Mean error panel

    sumBox->Add(new wxStaticText(summaryPanel, wxID_ANY, 
            _("Mean error :")), wxGBPosition(nextRow, 0), wxDefaultSpan,
            wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 8);

    wxPanel* innerErrPanel = new wxPanel(summaryPanel, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);

    innerErrPanel->SetBackgroundColour(dataCalib->error >= 1 ? RED : GREEN);
    innerErrPanel->SetForegroundColour((wxColour) * wxBLACK);
    wxGridBagSizer* innerErrSizer = new wxGridBagSizer(10, 10);
    // Standard deviation + mean error display
    double stdd = standardDeviation(dataCalib);
    innerErrSizer->Add(new wxStaticText(innerErrPanel, wxID_ANY, 
            std::to_wstring(dataCalib->error) + L" \u00b1 " + std::to_wstring(stdd)), 
            wxGBPosition(0, 0), wxDefaultSpan, wxALL | wxALIGN_CENTER | wxEXPAND, 8);
    innerErrSizer->SetSizeHints(innerErrPanel);
    innerErrPanel->SetSizer(innerErrSizer);

    sumBox->Add(innerErrPanel, wxGBPosition(nextRow, 1), wxDefaultSpan,
            wxALL | wxEXPAND | wxALIGN_CENTER, 8);

    sumBox->SetSizeHints(summaryPanel);
    summaryPanel->SetSizer(sumBox);
    box->Add(summaryPanel, wxGBPosition(1, 0), wxDefaultSpan, wxLEFT | wxRIGHT | wxEXPAND, 8);


    // Views
    box->Add(new wxStaticText(panel, wxID_ANY, "Views summary"), wxGBPosition(2, 0), wxDefaultSpan,
        wxLEFT | wxTOP | wxBOTTOM | wxRIGHT | wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 8);

    // Table
    wxGridStringTable *gt = new wxGridStringTable(dataCalib->nb_images, 3);
    gt->SetColLabelValue(0, "Path");
    gt->SetColLabelValue(1, "Used");
    gt->SetColLabelValue(2, "Error");
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        gt->SetValue(i, 0, dataCalib->IOcalib[i].image_name);
        gt->SetValue(i, 1, dataCalib->IOcalib[i].active_image ? "Yes" : "No");
        gt->SetValue(i, 2, std::to_string(dataCalib->IOcalib[i].errorView));
    }

    wxGrid* grid = new wxGrid(panel, wxID_ANY);
    grid->SetColLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
    grid->EnableDragCell(false);
    grid->EnableEditing(false);
    grid->EnableCellEditControl(false);
    grid->DisableDragColMove();
    grid->DisableColResize(1);
    grid->DisableColResize(2);
    grid->DisableDragRowSize();
    grid->SetTable(gt);
    grid->AutoSizeRow(1);
    grid->AutoSizeRow(2);
    grid->SetColSize(0, 400);

    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            grid->SetCellBackgroundColour(i, 2, (wxColour) * wxBLACK);
        }
        else if (dataCalib->IOcalib[i].errorView >= 1) {
            grid->SetCellBackgroundColour(i, 2, RED);
        } else {
            grid->SetCellBackgroundColour(i, 2, GREEN);
        }


        grid->SetCellTextColour(i, 2, (wxColour) *wxBLACK);
        grid->SetCellAlignment(i, 1, wxALIGN_CENTER, wxALIGN_CENTER);
        grid->SetCellAlignment(i, 2, wxALIGN_CENTER, wxALIGN_CENTER);
    }
    
    box->Add(grid, wxGBPosition(3, 0), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

    box->SetSizeHints(panel);
    panel->SetSizer(box);
    resultsFrame->Show();
    return 0;
}

double standardDeviation(Calib* dataCalib) {
    double sum = 0;
    double stdd = 0;
    int nbValid = 0;
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }
        ++nbValid;
        sum += dataCalib->IOcalib[i].errorView;
    }
    double mean = sum / nbValid;
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }
        stdd =+ pow(dataCalib->IOcalib[i].errorView - mean, 2);
    }
    return sqrt(stdd / nbValid);
}