#include "../headers/ExtractGridCorners.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define IMG_NAME "Corners extraction : "

int ExtractGridCorners(Calib *dataCalib) {
    std::vector<std::vector<cv::Point2f>> allCorners;
    std::vector<std::vector<cv::Point3f>> allCorners3D;
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        cv::Mat src = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);

        // Corners detection
        std::vector<cv::Point2f> corners;
        int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;
        // For spherical calibration, it doesn't detect corners as it runs a fast check on the image 
        // and shortcut the call if none is found.
        if (dataCalib->type == PERSPECTIVE_TYPE) {
            chessBoardFlags |= cv::CALIB_CB_FAST_CHECK;
        }

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
        allCorners.push_back(corners);
        allCorners3D.push_back(corners3D);
        std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
        cv::imshow(title, src);
        src.release();
        // Wait for any key input or for user to close image himself.
        // If the user closes the image, followings won't be displayed
        // and he will not be able to perform calibration.
        while (1) {
            if (cv::waitKey(1) != -1) {
                break;
            };
            if (cv::getWindowProperty(title, cv::WND_PROP_AUTOSIZE) == -1) {
                wxMessageBox("All corners weren't extracted properly : you won't be able to perform calibration.",
                "Corners extraction", wxICON_ERROR);
                return -1;
            }
        }
        // try/catch not necessary : if we reached this part, the window
        // is active
        cv::destroyWindow(title);
    }

    int result = wxMessageBox("Was the extraction successful ?", "Corners extraction", wxYES_NO | wxICON_QUESTION);
        if (result == wxYES) {
            // Saving corners positions in 2D and 3D
            for (int i = 0; i < allCorners.size(); ++i) {
                dataCalib->IOcalib[i].active_image = true;
                std::vector<cv::Point2f> corners = allCorners.at(i);               
                std::vector<cv::Point3f> corners3D = allCorners3D.at(i);
                dataCalib->IOcalib[i].CornersCoord2D = corners;
                dataCalib->IOcalib[i].CornersCoord3D = corners3D;
            }
        } else {
            for (int i = 0; i < allCorners.size(); ++i) {
                dataCalib->IOcalib[i].active_image = false;
            }
            return -1;
        }
        /*
        for (int i = 1; i <= dataCalib->nb_images; ++i) {
            cv::destroyWindow(std::string(IMG_NAME) + " " + std::to_string(i));
        }
        */
    return 0;
}