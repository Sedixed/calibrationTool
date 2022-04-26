#include "../headers/CalibrationResults.hpp"
#include <iomanip>
#include <opencv2/ccalib/omnidir.hpp>


int CalibrationResults(Calib *dataCalib) {
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
                std::cout << "Unknown calibration type\n";
            return -1;
        }
    }

    // Results display

    // --- Common results ---

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

    // --- Specific results ---
    switch(dataCalib->type) {
        // ------------------------------------
        // --- Perspective calibration case ---
        // ------------------------------------
        case PERSPECTIVE_TYPE:
        {
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
            break;
        }
        // ----------------------------------
        // --- Spherical calibration case ---
        // ----------------------------------
        case SPHERICAL_TYPE:
        {   
            // Skew
            res += "Skew : \t\t\t\t\t\t" + std::to_string(dataCalib->intrinsics.at<double>(0, 1)) + "\n";
            // Xi
            res += "Xi : \t\t\t\t\t\t\t" + std::to_string(dataCalib->Xi) + "\n";
            // Distorsions
            res += "Distorsions : \t\t\t\t";
            for (int j = 0; j < dataCalib->distCoeffs.cols; ++j) {
                streamObj << dataCalib->distCoeffs.at<double>(0, j) << "  ";
            }
            res += streamObj.str() + "\n";
            streamObj.str("");
            // Mean error
            res += "\nMean error : \t\t\t\t" + std::to_string(dataCalib->error) + "\n";
            wxMessageBox(res, "Calibration results", wxOK | wxICON_INFORMATION);
            break;
        }
        default:
            std::cout << "Unknown calibration type\n";
            return -1;
    }


    return 0;
}



    