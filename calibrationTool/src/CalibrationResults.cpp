#include "../headers/CalibrationResults.hpp"
#include <iomanip>
#include <opencv2/ccalib/omnidir.hpp>
#include <wx/gbsizer.h>
#include <wx/grid.h>



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

    wxFrame* resultsFrame = new wxFrame(parent, wxID_ANY, _("Calibration results"), wxPoint(-1, -1), wxSize(640, 500), style);
    wxScrolledWindow* panel = new wxScrolledWindow(resultsFrame);
    panel->SetScrollbars(0, 10, 0, 1000);
    wxGridBagSizer* box = new wxGridBagSizer(10, 10);
    box->Add(new wxStaticText(panel, wxID_ANY, "Calibration summary"), wxGBPosition(0, 0), wxDefaultSpan,
        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 8);

    // Focal length
    double fx = dataCalib->intrinsics.at<double>(0, 0);
    double fy = dataCalib->intrinsics.at<double>(1, 1);
    box->Add(new wxStaticText(panel, wxID_ANY, 
        _("Generalised focal length :\t" + std::to_string(fx) + "  " + std::to_string(fy))), wxGBPosition(1, 0), wxDefaultSpan,
        wxALL | wxEXPAND, 8);

    // Principal point
    double cx = dataCalib->intrinsics.at<double>(0, 2);
    double cy = dataCalib->intrinsics.at<double>(1, 2);
    box->Add(new wxStaticText(panel, wxID_ANY, 
        _("Principal point :\t\t\t\t" + std::to_string(cx) + "  " + std::to_string(cy))), wxGBPosition(2, 0), wxDefaultSpan,
        wxALL | wxEXPAND, 8);
    
    int nextRow;
    switch(dataCalib->type) {
        // ------------------------------------
        // --- Perspective calibration case ---
        // ------------------------------------
        case PERSPECTIVE_TYPE:
        {   
            // Distorsions
            std::string dist = "Distorsions : \t\t\t\t";
            for (int j = 0; j < dataCalib->distCoeffs.cols; ++j) {
                if (j == 0 || j == 1 || j == 4 || j == 5 || j == 6) {
                    dist += std::to_string(dataCalib->distCoeffs.at<double>(0, j)) + "  ";
                }
            }
            box->Add(new wxStaticText(panel, wxID_ANY, 
            _(dist)), wxGBPosition(3, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            

            // Method used
            std::string method = dataCalib->pref.fixed_point == 0 ? "calibrateCamera" : "calibrateCameraRO";
            box->Add(new wxStaticText(panel, wxID_ANY, 
            _("Method used : \t\t\t\t" + method)), wxGBPosition(4, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);

            nextRow = 5;
            break;
        }
        // ----------------------------------
        // --- Spherical calibration case ---
        // ----------------------------------
        case SPHERICAL_TYPE:
        {   
            // Skew
            box->Add(new wxStaticText(panel, wxID_ANY, 
            _("Skew : \t\t\t\t\t\t" + std::to_string(dataCalib->intrinsics.at<double>(0, 1)))), wxGBPosition(3, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            // Xi
            box->Add(new wxStaticText(panel, wxID_ANY, 
            _("Xi : \t\t\t\t\t\t\t" + std::to_string(dataCalib->Xi))), wxGBPosition(4, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
            // Distorsions
            std::string dist = "Distorsions : \t\t\t\t";
            for (int j = 0; j < dataCalib->distCoeffs.cols; ++j) {
                dist += std::to_string(dataCalib->distCoeffs.at<double>(0, j)) + "  ";
            }
            box->Add(new wxStaticText(panel, wxID_ANY, 
            _(dist)), wxGBPosition(5, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);

            nextRow = 6;
            break;
        }
        default:
             wxMessageBox("Error : unknown calibration type.", "Error", wxICON_ERROR);
            return -1;
    }

    // Mean error
    box->Add(new wxStaticText(panel, wxID_ANY, 
            _("Mean error : \t\t\t\t" + std::to_string(dataCalib->error))), wxGBPosition(nextRow, 0), wxDefaultSpan,
            wxALL | wxEXPAND, 8);
    ++nextRow;

    // Views
    box->Add(new wxStaticText(panel, wxID_ANY, "Views summary"), wxGBPosition(nextRow, 0), wxDefaultSpan,
        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 8);
    ++nextRow;

    wxGridStringTable* gt = new wxGridStringTable(dataCalib->nb_images + 1, 3);
    gt->SetValue(0, 0, _("View"));
    gt->SetValue(0, 1, _("Path"));
    gt->SetValue(0, 2, _("Error"));
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        gt->SetValue(i + 1, 0, std::to_string(i + 1));
        gt->SetValue(i + 1, 1, dataCalib->IOcalib[i].image_name);
        gt->SetValue(i + 1, 2, std::to_string(dataCalib->IOcalib[i].errorView));
    }

    wxGrid* grid = new wxGrid(panel, wxID_ANY);
    grid->SetTable(gt);

    
    box->Add(grid, wxGBPosition(nextRow, 0), wxDefaultSpan);

    box->SetSizeHints(panel);
    panel->SetSizer(box);
    resultsFrame->Show();
    return 0;
}



    