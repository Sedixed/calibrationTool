#include "../headers/Calibration.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/ccalib/omnidir.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// Nombre minimal d'images valides
#define MIN_VALID_IMAGES 3

int Calibration(Calib* dataCalib) {
    // ---------------------------------------
    // --- Common to all calibration types ---
    // ---------------------------------------

    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    // Amount of valid images
    int nbValid = 0;
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }
        ++nbValid;
        objectPoints.push_back(dataCalib->IOcalib[i].CornersCoord3D);
        imagePoints.push_back(dataCalib->IOcalib[i].CornersCoord2D);
    }

    // Error if not enough images
    if (nbValid < MIN_VALID_IMAGES) {
        wxMessageBox("Calibration requires at least 3 valid images.", "Calibration", wxICON_ERROR);
        return -1;
    }

    std::vector<cv::Mat> rVecs;                                    // Rotation vectors
    std::vector<cv::Mat> tVecs;                                    // Translation vectors
    int flags = dataCalib->pref.parameters_flags;                  // Flags

    // Used to get the size of an image
    cv::Mat img = cv::imread(dataCalib->IOcalib[0].image_name, cv::IMREAD_COLOR);

    // Future result of calibration
    double error;
    

    switch(dataCalib->type) {
        // ------------------------------------
        // --- Perspective calibration case ---
        // ------------------------------------
        case PERSPECTIVE_TYPE:
        {
            std::vector<double> perViewError;                              // Error per view vector
            std::vector<cv::Point3f> newObjPoints;                         // New object points
            if (!(flags & cv::CALIB_FIX_K3 && flags & cv::CALIB_FIX_K4 && flags & cv::CALIB_FIX_K5)) {
                flags |= cv::CALIB_RATIONAL_MODEL; 
            } 

            // Calibration
            try {
                error = cv::calibrateCameraRO(objectPoints, imagePoints, img.size(), 
                    dataCalib->pref.fixed_point == 1 ? dataCalib->calibPattern.nbSquareX - 1 : 0,
                    dataCalib->intrinsics, dataCalib->distCoeffs, rVecs, tVecs, newObjPoints, 
                    cv::noArray(), cv::noArray(), cv::noArray(), perViewError, flags);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                return -1;
            }

            /*
            // Approximativement égal à ObjectPoints
            std::cout << "\n--new (ro only) --\n";
            for (int i = 0; i < newObjPoints.size(); ++i) {
                std::cout << newObjPoints[i].x << " " << newObjPoints[i].y << " " << newObjPoints[i].z << std::endl;
            }
            */

            // Save mean error
            dataCalib->error = error;
            // Save extrinsics parameters
            for (int i = 0; i < dataCalib->nb_images; ++i) {
                if (!dataCalib->IOcalib[i].active_image) {
                    continue;
                }
                dataCalib->IOcalib[i].rotationMat = rVecs[i];
                dataCalib->IOcalib[i].translationMat = tVecs[i];
                dataCalib->IOcalib[i].errorView = perViewError[i];
            }
            wxMessageBox("Calibration succeeded !", "Calibration", wxOK);
            break;
        }

        // ----------------------------------
        // --- Spherical calibration case ---
        // ----------------------------------
        case SPHERICAL_TYPE:
        {
            cv::Mat Xi;     // Xi parameter for CMei's model (mirror shape)
        
            // Calibration
            cv::TermCriteria crit(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 300, 0.0001);

            try {
                error = cv::omnidir::calibrate(objectPoints, imagePoints, img.size(),
                    dataCalib->intrinsics, Xi, dataCalib->distCoeffs, rVecs, tVecs,
                    flags, crit);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                return -1;
            }

            std::cout << error << std::endl;
        }

        
    }


    img.release();
    return 0;
}