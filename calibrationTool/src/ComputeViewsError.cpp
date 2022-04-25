#include "../headers/ComputeViewsError.hpp"
#include <iomanip>
#include <opencv2/ccalib/omnidir.hpp>

#define IMG_NAME "Corners Reprojection :"

int ComputeViewsError(Calib *dataCalib, wxWindow* parent, bool show, bool perViewErrorCalculated) {
    // ---------------------------------------
    // --- Common to all calibration types ---
    // ---------------------------------------

    cv::Mat cameraMatrix = dataCalib->intrinsics;
    cv::Mat distCoeffs = dataCalib->distCoeffs;

    // For spherical error calculus
    int totalPoints = 0;
    double totalErr = 0;

    // Used for showing images
    cv::Size patternSize = cv::Size(dataCalib->calibPattern.nbSquareX - 1, dataCalib->calibPattern.nbSquareY - 1);
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }

        std::vector<cv::Point3f> objPoints = dataCalib->IOcalib[i].CornersCoord3D; // 3D Points
        cv::Mat rVec = dataCalib->IOcalib[i].rotationMat;                          // Rotation matrix          
        cv::Mat tVec = dataCalib->IOcalib[i].translationMat;                       // Translation matrix
        std::vector<cv::Point2f> imgPointsOutput;                                  // Output 2D points

        switch (dataCalib->type) {
            // ------------------------------------
            // --- Perspective calibration case ---
            // ------------------------------------
            case PERSPECTIVE_TYPE:
            {   
                cv::projectPoints(objPoints, rVec, tVec, cameraMatrix, distCoeffs, imgPointsOutput);

                // Showing image if necessary
                if (show) {
                    double err = dataCalib->IOcalib[i].errorView;
                    cv::Mat img = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);
                    cv::drawChessboardCorners(img, patternSize, cv::Mat(imgPointsOutput), true);
                    std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
                    std::string errText = "View error : " + std::to_string(err);
                    cv::putText(img, errText, cv::Point(15, img.rows - 15), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 255), 1, 8, false);
                    cv::imshow(title, img);
                    img.release();
                    while(cv::waitKey(1) == -1);
                    if (cv::getWindowProperty(title, cv::WND_PROP_AUTOSIZE) == -1) {
                        return -1;
                    }   
                    cv::destroyWindow(title);
                }
                break;
            }
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
                totalErr += err * err;
                totalPoints += n;

                // Showing image if necessary
                if (show) {
                    cv::Mat img = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);
                    cv::drawChessboardCorners(img, patternSize, cv::Mat(imgPointsOutput), true);
                    std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
                    std::string errText = "View error : " + std::to_string(dataCalib->IOcalib[i].errorView);
                    cv::putText(img, errText, cv::Point(15, img.rows - 15), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 255), 1, 8, false);
                    cv::imshow(title, img);
                    img.release();
                    while(cv::waitKey(1) == -1);
                    if (cv::getWindowProperty(title, cv::WND_PROP_AUTOSIZE) == -1) {
                        return -1;
                    }   
                    cv::destroyWindow(title);
                }
                break;
            }
        }
    }

    if (show) {
        parent->Raise();
    } else {
        // To manipulate values precision
        std::ostringstream streamObj;
        streamObj << std::fixed;
        streamObj << std::setprecision(3);
        std::string res;
        res += "Calibration results :\n\n";
        // Focal length
        double fx = dataCalib->intrinsics.at<double>(0, 0);
        double fy = dataCalib->intrinsics.at<double>(1, 1);
        streamObj << "Generalised focal length :\t" << fx << "  " << fy  << "\n";
        res += streamObj.str();
        streamObj.str("");
        // Principal point
        double cx = dataCalib->intrinsics.at<double>(0, 2);
        double cy = dataCalib->intrinsics.at<double>(1, 2);
        streamObj << "Principal point :\t\t\t\t" << cx << "  " << cy  << "\n";
        res += streamObj.str();
        streamObj.str("");
        // Distorsions
        res += "Distorsions : \t\t\t\t";
        for (int j = 0; j < dataCalib->distCoeffs.cols; ++j) {
            if (j == 0 || j == 1 || j == 4 || j == 5 || j == 6) {
                streamObj << dataCalib->distCoeffs.at<double>(0, j) << "  ";
            }
        }
        res += streamObj.str() + "\n";
        streamObj.str("");

        // Method used
        res += "Method used : \t\t\t\t";
        res += std::string(dataCalib->pref.fixed_point == 0 ? "calibrateCamera\n" : "calibrateCameraRO\n");
        res += "\nMean error : \t\t\t\t" + std::to_string(dataCalib->error) + "\n";
        wxMessageBox(res, "Calibration results", wxOK | wxICON_INFORMATION);
    }
    return 0;
}