#include "../headers/Calibration.hpp"

#include <opencv2/opencv.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// Nombre minimal d'images valides
#define MIN_VALID_IMAGES 3

int Calibration(Calib* dataCalib) {
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
        wxMessageBox("Calibration requires at least 3 valid images.", "Calibration", wxOK);
        return -1;
    }

    std::vector<cv::Point3f> newObjPoints;                         // New object points
    cv::Mat D;                                                     // Distortion coefficients
    std::vector<cv::Mat> rVecs;                                    // Rotation vectors
    std::vector<cv::Mat> tVecs;                                    // Translation vectors
    int flags = dataCalib->pref.parameters_flags | cv::CALIB_RATIONAL_MODEL;                  // Flags

    // Used to get the size of an image
    cv::Mat img = cv::imread(dataCalib->IOcalib[0].image_name, cv::IMREAD_COLOR);

    // Calibration
    double error = cv::calibrateCameraRO(objectPoints, imagePoints, img.size(), 
            dataCalib->pref.fixed_point == 1 ? dataCalib->calibPattern.nbSquareX - 1 : 0,
            dataCalib->intrinsics, D, rVecs, tVecs, newObjPoints, flags);
 
    // Mean error
    std::cout << "----------------------------------\n";
    std::cout << "Mean error : " << error << std::endl;
    // Focal length
    double focalLengthX = dataCalib->intrinsics.at<double>(0, 0);
    double focalLengthY = dataCalib->intrinsics.at<double>(1, 1);
    // Principal point
    double prPointX = dataCalib->intrinsics.at<double>(0, 2);
    double prPointY = dataCalib->intrinsics.at<double>(1, 2);
    std::cout << "focal length : " << focalLengthX << " " << focalLengthY << std::endl;
    std::cout << "principal point : " << prPointX << " " << prPointY << std::endl;
    // Distorsions
    std::cout << "Distorsions : ";
    for (int j = 0; j < D.cols; ++j) {
        std::cout << D.at<double>(0, j) << " ";
    }
    std::cout << std::endl;
    

    // Save intrinsics parameters and mean error
    dataCalib->error = error;

    // Save extrinsics parameters
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }
        dataCalib->IOcalib[i].rotationMat = rVecs[i];
        dataCalib->IOcalib[i].translationMat = tVecs[i];
    }

    wxMessageBox("Calibration succeeded !", "Calibration", wxOK);

    img.release();
    return 0;
}