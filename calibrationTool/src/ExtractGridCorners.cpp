#include "../headers/ExtractGridCorners.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define IMG_NAME "Corners extraction"

int ExtractGridCorners(Calib *dataCalib) {
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        /*if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }*/

        cv::Mat src = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);

        // Corners detection

        std::vector<cv::Point2f> corners;
        int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK;

        cv::Size patternSize = cv::Size(dataCalib->calibPattern.nbSquareX - 1, dataCalib->calibPattern.nbSquareY - 1);
        bool found = cv::findChessboardCorners(src, patternSize, corners, chessBoardFlags);

        if (found) {
            cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.0001);
            cv::Mat viewGray;
            cv::cvtColor(src, viewGray, cv::COLOR_BGR2GRAY);
            cv::cornerSubPix(viewGray, corners, cv::Size(dataCalib->pref.search_window_size, dataCalib->pref.search_window_size),
                                cv::Size(-1,-1), criteria);
            cv::drawChessboardCorners(src, patternSize, cv::Mat(corners), found);
            viewGray.release();
        }

        // 3D coordinates of the points
        std::vector<cv::Point3f> corners3D;
        for (int j = 0; j < dataCalib->calibPattern.nbSquareY - 1; ++j) {
            for (int k = 0; k < dataCalib->calibPattern.nbSquareX - 1; ++k) {
                corners3D.push_back(cv::Point3f(
                                    k * dataCalib->calibPattern.sizeSquareX,
                                    j * dataCalib->calibPattern.sizeSquareY, 
                                    0));
            }
        }

        cv::imshow(IMG_NAME, src);

        int result = wxMessageBox("Was the extraction successful ?", "Corners extraction", wxYES_NO | wxICON_QUESTION);
        if (result == wxYES) {
            // Saving corners positions in 2D and 3D
            dataCalib->IOcalib[i].CornersCoord3D = corners3D;
            dataCalib->IOcalib[i].CornersCoord2D = corners;
        } else {
            dataCalib->IOcalib[i].active_image = false;
        }
        cv::destroyWindow(IMG_NAME);
        src.release();
    }
    return 0;
}