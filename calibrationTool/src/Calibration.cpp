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
    // First valid image
    int firstRead = -1;
    // Amount of valid images
    int nbValid = 0;
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }
        if (firstRead == -1) {
            firstRead = i;
        }
        ++nbValid;
        objectPoints.push_back(dataCalib->IOcalib[i].CornersCoord3D);
        imagePoints.push_back(dataCalib->IOcalib[i].CornersCoord2D);
    }

    if (nbValid < MIN_VALID_IMAGES) {
        wxMessageBox("Calibration requires at least 3 valid images.", "Calibration", wxOK);
        return -1;
    }
  
    cv::Mat K(3, 3, CV_64F);        // Intrinsics parameters 
    cv::Mat D;                      // Distortion coefficients
    std::vector<cv::Mat> rVecs;     // Rotation vectors
    std::vector<cv::Mat> tVecs;     // Translation vectors
    int flags = 0;                  // Flags
    // Used to get the size on an image
    cv::Mat img = cv::imread(dataCalib->IOcalib[firstRead].image_name, cv::IMREAD_COLOR);

    double error = cv::calibrateCamera(objectPoints, imagePoints, img.size(), K, D, rVecs, tVecs, flags);

    /* PRINTING RESULTS
    // Mean error
    std::cout << "Mean error : " << error << std::endl;
    // Focal length
    double focalLengthX = K.at<double>(0, 0);
    double focalLengthY = K.at<double>(1, 1);
    // Principal point
    double prPointX = K.at<double>(0, 2);
    double prPointY = K.at<double>(1, 2);
    // Skew
    double skew = K.at<double>(0, 1);
    std::cout << "focal length : " << focalLengthX << " " << focalLengthY << std::endl;
    std::cout << "principal point : " << prPointX << " " << prPointY << std::endl;
    std::cout << "skew : " << skew << std::endl;
    // Distorsions
    std::cout << "Distorsions : ";
    for (int j = 0; j < D.cols; ++j) {
        std::cout << D.at<double>(0, j) << " ";
    }
    std::cout << std::endl;
    */

    // Save intrinsics parameters and mean error 
    dataCalib->intrinsics = K;
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