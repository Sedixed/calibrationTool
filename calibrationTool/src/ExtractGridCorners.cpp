#include "../headers/ExtractGridCorners.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define IMG_NAME "Corners extraction"

int ExtractGridCorners(Calib *dataCalib) {
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        if (!dataCalib->IOcalib[i].active_image) {
            continue;
        }

        cv::Mat src = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_COLOR);
    
        std::vector<cv::Point2f> pointBuf;
        int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;

        bool found = cv::findChessboardCorners(src, dataCalib->pref.pattern_size, pointBuf, chessBoardFlags);

        if (found) {   
            cv::Mat viewGray;
            cv::cvtColor(src, viewGray, cv::COLOR_BGR2GRAY);
            cv::cornerSubPix(viewGray, pointBuf, cv::Size(5, 5),
                                cv::Size(-1,-1), cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 30, 0.0001));
            cv::drawChessboardCorners(src, dataCalib->pref.pattern_size, cv::Mat(pointBuf), found);
            viewGray.release();
        }

        
        cv::imshow(IMG_NAME, src);

        int result = wxMessageBox("Was the extraction successful ?", "Corners extraction", wxYES_NO | wxICON_QUESTION);
        if (result == wxYES) {
            cv::destroyWindow(IMG_NAME);
            //save les corners
            dataCalib->IOcalib[i].CornersCoord2D = cv::Mat(pointBuf);
            dataCalib->IOcalib[i].active_image = true;
            src.release();
        } else {
            int sndResult = wxMessageBox("Do you want to try again ?", "Corners extraction", wxYES_NO | wxICON_QUESTION);

            if (sndResult == wxYES) {
                --i;
                cv::destroyWindow(IMG_NAME);
                src.release();
            } else {
                cv::destroyWindow(IMG_NAME);
                src.release();
                dataCalib->IOcalib[i].active_image = false;
            }
        }
    }
    return 0;
}