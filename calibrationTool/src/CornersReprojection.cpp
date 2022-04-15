#include "../headers/CornersReprojection.hpp"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <iomanip>

#define IMG_NAME "Corners Reprojection :"

int CornersReprojection(Calib *dataCalib, int show) {
    cv::Mat cameraMatrix = dataCalib->intrinsics;
    cv::Mat distCoeffs = dataCalib->distCoeffs;
    // For results
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

        cv::projectPoints(objPoints, rVec, tVec, cameraMatrix, distCoeffs, imgPointsOutput);

        // Showing image
        if (show == 1) {
            cv::Mat img = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);
            cv::drawChessboardCorners(img, patternSize, cv::Mat(imgPointsOutput), true);
            std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
            cv::imshow(title, img);
            img.release();
        // Saving current result
        } else {
            double err = cv::norm(cv::Mat(dataCalib->IOcalib[i].CornersCoord2D), cv::Mat(imgPointsOutput), cv::NORM_L2);
            int n = (int)objPoints.size();
            dataCalib->IOcalib[i].errorView = (float) std::sqrt(err * err / n);
            std::cout << "Erreur image " << (i + 1) << " : " <<  dataCalib->IOcalib[i].errorView << std::endl;
            totalErr += err * err;
            totalPoints += n;
        }
    }
    if (show == 0) {
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
        // Error per view
        // à voir plutôt pour l'afficher sur les images direct car là c'est dégueu

        /*res += "Error per view : \t\t";

        int n_col = (int)(floor(sqrt((double)(dataCalib->nb_images))));
        std::cout << "ncol" << n_col << std::endl;;
        for (int i = 0; i < dataCalib->nb_images; ++i) {
            float err = dataCalib->IOcalib[i].errorView;
            streamObj << "View " << (i + 1) << " : " << err;
            if ((i + 1) % (n_col) == 0 || i == dataCalib->nb_images - 1) {
                streamObj << "\n";
            } else {
                streamObj << "  ";
            }
        }
        res += streamObj.str();
        streamObj.str("");*/

        // Method used
        res += "Method used : \t\t\t\t";
        res += std::string(dataCalib->pref.fixed_point == 0 ? "calibrateCamera\n" : "calibrateCameraRO\n");
        res += "\nMean error : " + std::to_string(std::sqrt(totalErr/totalPoints)) + "\n";
        wxMessageBox(res, "Calibration results", wxOK | wxICON_INFORMATION);

    }
    return 0;
}