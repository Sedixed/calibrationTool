#include "../headers/ExtractGridCorners.hpp"

int ExtractGridCorners(Calib *dataCalib) {
    cv::Mat src = cv::imread(dataCalib->IOcalib[0].image_name, cv::IMREAD_COLOR);
    // 2 lignes suivantes p-e à bouger dans le if found
    
    std::vector<cv::Point2f> pointBuf;
    int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;

    bool found = cv::findChessboardCorners(src, dataCalib->pref.pattern_size, pointBuf, chessBoardFlags);
    std::cout<<"number of corners detected:"<<pointBuf.size()<<std::endl;
    if (found) {
        
        cv::Mat viewGray;
        cv::cvtColor(src, viewGray, cv::COLOR_BGR2GRAY);

        cv::cornerSubPix(viewGray, pointBuf, cv::Size(5, 5),
                            cv::Size(-1,-1), cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 30, 0.0001));
        cv::drawChessboardCorners(src, dataCalib->pref.pattern_size, cv::Mat(pointBuf), found);
        cv::imshow("Image View", src);
    }

    return 0;
}