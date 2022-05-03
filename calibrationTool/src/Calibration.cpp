#include "../headers/Calibration.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/ccalib/omnidir.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// Minimal amount of valid images
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
        // If the user declared extraction successful but it wasn't 
        // (not all corners detected on at least one view)
        if (dataCalib->IOcalib[i].CornersCoord3D.size() != dataCalib->IOcalib[i].CornersCoord2D.size()) {
            wxMessageBox(std::string("There is at least one image whose corners weren't all extracted.\n") +
                    std::string("Error detected on image :\n") + 
                    std::string(dataCalib->IOcalib[i].image_name),
                    "Extraction error", wxICON_ERROR);
            return -1;
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
        
            // Calibration
            try {
                error = cv::calibrateCameraRO(objectPoints, imagePoints, img.size(), 
                    dataCalib->pref.fixed_point == 1 ? dataCalib->calibPattern.nbSquareX - 1 : 0,
                    dataCalib->intrinsics, dataCalib->distCoeffs, rVecs, tVecs, newObjPoints, 
                    cv::noArray(), cv::noArray(), cv::noArray(), perViewError, flags);
            } catch (std::exception& e) {
                wxMessageBox("An unknown error occurred.", "Error", wxICON_ERROR);
                return -1;
            }

            // Save mean error
            dataCalib->error = error;
            // Save extrinsics parameters
            int cpt = 0;
            for (int i = 0; i < dataCalib->nb_images; ++i) {
                if (!dataCalib->IOcalib[i].active_image) {
                    continue;
                }
                dataCalib->IOcalib[i].rotationMat = rVecs[cpt];
                dataCalib->IOcalib[i].translationMat = tVecs[cpt];
                dataCalib->IOcalib[i].errorView = perViewError[cpt];
                ++cpt;
            }
            wxMessageBox("Calibration succeeded !", "Calibration", wxOK);
            break;
        }

        // ----------------------------------
        // --- Spherical calibration case ---
        // ----------------------------------
        case SPHERICAL_TYPE:
        {   
            cv::Mat Xi(1, 1, CV_64F);
            Xi.at<double>(0, 0) = dataCalib->Xi;
            cv::TermCriteria crit(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 300, 0.0001);

            // Calibration
            try {
                error = cv::omnidir::calibrate(objectPoints, imagePoints, img.size(),
                    dataCalib->intrinsics, Xi, dataCalib->distCoeffs, rVecs, tVecs,
                    flags, crit);
            } catch (std::exception& e) {
                wxMessageBox("An unknown error occurred.", "Error", wxICON_ERROR);
                return -1;
            }
            // Save mean error and Xi
            dataCalib->Xi = Xi.at<double>(0, 0);
            dataCalib->error = error;
            // Save extrinsics parameters
            int cpt = 0;
            for (int i = 0; i < dataCalib->nb_images; ++i) {
                if (!dataCalib->IOcalib[i].active_image) {
                    continue;
                }
                try {
                    if ((size_t) cpt < rVecs.size()) {
                        dataCalib->IOcalib[i].rotationMat = rVecs[cpt];
                        dataCalib->IOcalib[i].translationMat = tVecs[cpt];
                        ++cpt;
                    } else {
                        wxMessageBox("Calibration failed : please ensure that the grid corners extraction was correct.", "Calibration", wxICON_ERROR);
                        img.release();
                        return -1;
                    }
                } catch (std::exception& e) {
                    wxMessageBox("Calibration failed : please ensure that the grid corners extraction was correct.", "Calibration", wxICON_ERROR);
                    img.release();
                    return -1;
                }
            }
            wxMessageBox("Calibration succeeded !", "Calibration", wxOK);
            break;
        }

        default:
            wxMessageBox("Error : unknown calibration type.", "Error", wxICON_ERROR);
    }

    img.release();
    return 0;
}