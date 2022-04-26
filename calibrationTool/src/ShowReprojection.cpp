#include "../headers/ShowReprojection.hpp"
#include <opencv2/ccalib/omnidir.hpp>

#define IMG_NAME "Corners Reprojection :"


int ShowReprojection(Calib *dataCalib, wxWindow* parent) {
    cv::Mat cameraMatrix = dataCalib->intrinsics;
    cv::Mat distCoeffs = dataCalib->distCoeffs;

    int totalPoints = 0;
    double totalErr = 0;

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
                std::cout << "Unknown calibration type\n";
                return -1;
        }
    }

    // For drawChessboardCorners function
    cv::Size patternSize = cv::Size(dataCalib->calibPattern.nbSquareX - 1, dataCalib->calibPattern.nbSquareY - 1);
    
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }

        bool clickClosed = false;
        std::vector<cv::Point2f> imgPointsOutput; // Output 2D points
        projectPoints(dataCalib, i, imgPointsOutput);

        // Showing image
        cv::Mat img = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);
        cv::drawChessboardCorners(img, patternSize, cv::Mat(imgPointsOutput), true);
        std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
        std::string errText = "View error : " + std::to_string(dataCalib->IOcalib[i].errorView);
        cv::putText(img, errText, cv::Point(15, img.rows - 15), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 255), 1, 8, false);
        cv::resize(img, img, dataCalib->pref.render_size, cv::INTER_LINEAR);
        cv::imshow(title, img);
        cv::setMouseCallback(title, callbackClickSR, &clickClosed);
        img.release();
        while(!clickClosed && cv::waitKey(1) == -1);
        if (cv::getWindowProperty(title, cv::WND_PROP_AUTOSIZE) == -1) {
            return -1;
        }   
        cv::destroyWindow(title);
    }

    if (parent != NULL) {
        parent->Raise();
    }
    return 0;
}


void callbackClickSR(int evt, int x, int y, int flags, void* data) {
    bool* clickClosed = (bool*) data;
    if (evt == cv::EVENT_LBUTTONDOWN) {
        *clickClosed = true;
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
            std::cout << "Unknown calibration type\n";
            return -1;
    }
}