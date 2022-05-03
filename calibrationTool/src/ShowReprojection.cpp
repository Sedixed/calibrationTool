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

        // Base points
        drawChessboardCorners(img, patternSize, cv::Mat(dataCalib->IOcalib[i].CornersCoord2D), true, cv::Scalar(0, 0, 255, 0), 0);
        // Recalculated points
        drawChessboardCorners(img, patternSize, cv::Mat(imgPointsOutput), true, cv::Scalar(255, 0, 0, 0), 1);
        std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
        cv::setWindowTitle(windowID, title);
        std::string errText = "View error : " + std::to_string(dataCalib->IOcalib[i].errorView);
        drawCross(img, 25, img.rows - 85, 10, 0, 1, cv::Scalar(0, 0, 255, 0));
        drawCross(img, 25, img.rows - 54, 8, 1, 1, cv::Scalar(255, 0, 0, 0));
        cv::putText(img, errText, cv::Point(15, img.rows - 15), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 255), 1, 8, false);
        cv::putText(img, "Extracted points", cv::Point(45, img.rows - 75), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 255), 1, 8, false);
        cv::putText(img, "Calculated points",cv::Point(45, img.rows - 45), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 0, 0), 1, 8, false);
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


void drawCross(cv::InputOutputArray _image, double x, double y, int size, int type, int thickness, cv::Scalar color) {
    using namespace cv;

    // Get header for input arrays
    Mat image = _image.getMat();
    
    if( type == 0 )
    {
        line( image, Point((int)(x-size), (int)(y)), Point((int)(x+size), (int)(y)), color, thickness );
        line( image, Point((int)(x), (int)(y-size)), Point((int)(x), (int)(y+size)), color, thickness );
    }
    else
    {
        line( image, Point((int)(x-size), (int)(y-size)), Point((int)(x+size), (int)(y+size)), color, thickness, LINE_AA );
        line( image, Point((int)(x-size), (int)(y+size)), Point((int)(x+size), (int)(y-size)), color, thickness, LINE_AA );
    }
}


void drawChessboardCorners(cv::InputOutputArray image, cv::Size patternSize, cv::InputArray _corners, bool patternWasFound, cv::Scalar color, int type) {
    using namespace cv;

    int t = image.type();
    int cn = CV_MAT_CN(t);
    CV_CheckType(t, cn == 1 || cn == 3 || cn == 4,
            "Number of channels must be 1, 3 or 4" );

    int depth = CV_MAT_DEPTH(t);
    CV_CheckType(t, depth == CV_8U || depth == CV_16U || depth == CV_32F,
            "Only 8-bit, 16-bit or floating-point 32-bit images are supported");

    if (_corners.empty()) {
        return;
    }
    Mat corners = _corners.getMat();
    const Point2f* corners_data = corners.ptr<Point2f>(0);
    CV_DbgAssert(corners_data);
    int nelems = corners.checkVector(2, CV_32F, true);
    CV_Assert(nelems >= 0);

    const int shift = 0;

    if (!patternWasFound) {
        Scalar errColor(0,0,255,0);

        for (int i = 0; i < nelems; ++i) {
            cv::Point2i pt(
                    cvRound(corners_data[i].x*(1 << shift)),
                    cvRound(corners_data[i].y*(1 << shift))
            );
            drawCross(image, pt.x, pt.y, 4, 8, 1, errColor);
        }
    } else {
        cv::Point2i prev_pt;
        for (int y = 0, i = 0; y < patternSize.height; ++y) {
            for (int x = 0; x < patternSize.width; x++, i++) {
                cv::Point2i pt(
                        cvRound(corners_data[i].x*(1 << shift)),
                        cvRound(corners_data[i].y*(1 << shift))
                );

                if (i != 0) {
                    drawCross(image, pt.x, pt.y, 4, type, 1, color);
                }
                prev_pt = pt;
            }
        }
    }
}
