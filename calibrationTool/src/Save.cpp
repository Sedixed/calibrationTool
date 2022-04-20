#include "../headers/Save.hpp"
#include <opencv2/opencv.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

int Save(Calib* dataCalib) {
    wxFileDialog dialog(NULL, _("Save to YML file"), "", "calibration_results",
                       "YML files (*.yml)|*.yml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    int r = dialog.ShowModal();
    if (r == wxID_CANCEL) {
        return -1;
    }

    cv::FileStorage fs(std::string(dialog.GetPath().mb_str()), cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML);
    fs << "Title" << "Calibration results";
    fs << "Type" << "Perspective";
    fs << "Number of images" << dataCalib->nb_images;
    // Mire data
    fs << "Mire used";
    fs << "{" << "Number of squares along X" << dataCalib->calibPattern.nbSquareX;
    fs << "Number of squares along Y" << dataCalib->calibPattern.nbSquareY;
    fs << "Size of a square along X" << dataCalib->calibPattern.sizeSquareX;
    fs << "Size of a square along Y" << dataCalib->calibPattern.sizeSquareY << "}";
    // Mean error
    fs << "Mean error" <<  dataCalib->error;
    // Preferences
    fs << "Method" << (dataCalib->pref.fixed_point == 0 ? "calibrateCamera" : "calibrateCameraRO");
    fs << "Render size" << "{";
    fs << "Width" << dataCalib->pref.render_size.width;
    fs << "Height" << dataCalib->pref.render_size.height << "}";
    fs << "Search window size" << dataCalib->pref.search_window_size;
    // Intrinsics parameters
    fs << "Intrinsics parameters" << dataCalib->intrinsics;
    // Distorsions
    fs << "Distorsion coefficients" << dataCalib->distCoeffs;
    // Flags
    fs << "Flags" << dataCalib->pref.parameters_flags;

    // Views
    fs << "Views data" << "{";
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        fs << "View " + std::to_string(i + 1) << "{";
        fs << "Path" << dataCalib->IOcalib[i].image_name;
        fs << "View error" << dataCalib->IOcalib[i].errorView;
        fs << "Rotation vector" << dataCalib->IOcalib[i].rotationMat;
        fs << "Translation vector" << dataCalib->IOcalib[i].translationMat;
        fs << "Image points" << VecPoint2FToMat(dataCalib->IOcalib[i].CornersCoord2D);
        fs << "Object points" << VecPoint3FToMat(dataCalib->IOcalib[i].CornersCoord3D) << "}";

    }
    fs << "}";
    fs.release();
    return 0;
}


cv::Mat VecPoint2FToMat(std::vector<cv::Point2f>& v) {
    cv::Mat m(2, v.size(), CV_32FC1);
    for (size_t i = 0; i < v.size(); ++i) {
        m.at<float>(0, i) = v[i].x;
        m.at<float>(1, i) = v[i].y;
    }
    return m;
}


cv::Mat VecPoint3FToMat(std::vector<cv::Point3f>& v) {
    cv::Mat m(3, v.size(), CV_32FC1);
    for (size_t i = 0; i < v.size(); ++i) {
        m.at<float>(0, i) = v[i].x;
        m.at<float>(1, i) = v[i].y;
        m.at<float>(2, i) = v[i].z;
    }
    return m;
}