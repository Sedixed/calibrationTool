#include "../headers/ShowReprojection.hpp"
#include <opencv2/ccalib/omnidir.hpp>

#define IMG_NAME "Corners Reprojection :"


int ShowReprojection(Calib *dataCalib, wxWindow* parent) {
    cv::Mat cameraMatrix = dataCalib->intrinsics;
    cv::Mat distCoeffs = dataCalib->distCoeffs;

    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }

        std::vector<cv::Point3f> objPoints = dataCalib->IOcalib[i].CornersCoord3D; // 3D Points
        cv::Mat rVec = dataCalib->IOcalib[i].rotationMat;                          // Rotation matrix          
        cv::Mat tVec = dataCalib->IOcalib[i].translationMat;                       // Translation matrix
        std::vector<cv::Point2f> imgPointsOutput;                                  // Output 2D points

        // We don't handle perspective calibration case because it has 
        //  necessarily been calculated through calibrateCameraRO
        switch (dataCalib->type) {
            // ------------------------------------
            // --- Perspective calibration case ---
            // ------------------------------------
            case PERSPECTIVE_TYPE:
                break;
            // ----------------------------------
            // --- Spherical calibration case ---
            // ----------------------------------
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

    // For drawChessboardCorners function
    cv::Size patternSize = cv::Size(dataCalib->calibPattern.nbSquareX - 1, dataCalib->calibPattern.nbSquareY - 1);
    std::string windowID = "Corners reprojection";
    cv::namedWindow(windowID, 1);
    
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }

        bool clickNext = false;
        std::vector<cv::Point2f> imgPointsOutput; // Output 2D points
        projectPoints(dataCalib, i, imgPointsOutput);
        // Showing image
        cv::Mat img = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);
        cv::drawChessboardCorners(img, patternSize, cv::Mat(imgPointsOutput), true);
        std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
        cv::setWindowTitle(windowID, title);
        std::string errText = "View error : " + std::to_string(dataCalib->IOcalib[i].errorView);
        cv::putText(img, errText, cv::Point(15, img.rows - 15), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 255), 1, 8, false);
        cv::resize(img, img, dataCalib->pref.render_size, cv::INTER_LINEAR);
        cv::imshow(windowID, img);
        cv::setMouseCallback(windowID, callbackClickSR, &clickNext);
        img.release();
        while(!clickNext && cv::waitKey(1) == -1);
        if (cv::getWindowProperty(windowID, cv::WND_PROP_AUTOSIZE) == -1) {
            return -1;
        }   
    }
    cv::destroyWindow(windowID);
    if (parent != NULL) {
        parent->Raise();
    }
    return 0;
}


void callbackClickSR(int evt, int x, int y, int flags, void* data) {
    bool* clickNext = (bool*) data;
    if (evt == cv::EVENT_LBUTTONDOWN) {
        *clickNext = true;
    }
}


int projectPoints(Calib *dataCalib, int index, std::vector<cv::Point2f>& imgPointsOutput) {
    std::vector<cv::Point3f> objPoints = dataCalib->IOcalib[index].CornersCoord3D; // 3D Points
    cv::Mat rVec = dataCalib->IOcalib[index].rotationMat;                          // Rotation matrix          
    cv::Mat tVec = dataCalib->IOcalib[index].translationMat;                       // Translation matrix
    switch (dataCalib->type) {
        // ------------------------------------
        // --- Perspective calibration case ---
        // ------------------------------------
        case PERSPECTIVE_TYPE:
        {   
            cv::projectPoints(objPoints, rVec, tVec, dataCalib->intrinsics, dataCalib->distCoeffs, imgPointsOutput);
            return 0;
        }
        // ----------------------------------
        // --- Spherical calibration case ---
        // ----------------------------------
        case SPHERICAL_TYPE:
        {
            cv::omnidir::projectPoints(objPoints, imgPointsOutput, rVec, tVec, dataCalib->intrinsics, dataCalib->Xi, dataCalib->distCoeffs);
            return 0;
        }
        default:
            wxMessageBox("Error : unknown calibration type.", "Error", wxICON_ERROR);
            return -1;
    }
}