#include "../headers/ExtractGridCorners.hpp"
#include "../headers/ImageUtils.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#define IMG_NAME "Corners extraction : "


int ExtractGridCorners(Calib *dataCalib) {
    std::vector<std::vector<cv::Point2f>> allCorners;
    std::vector<std::vector<cv::Point3f>> allCorners3D;

    std::string windowID = IMG_NAME;
    cv::namedWindow(windowID, cv::WND_PROP_AUTOSIZE);
    bool reIterated = false;
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        dataCalib->IOcalib[i].active_image = true;
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

        std::vector<cv::Point3f> corners3D;
        if (found) {
            cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.0001);
            cv::Mat viewGray;
            cv::cvtColor(src, viewGray, cv::COLOR_BGR2GRAY);
            cv::cornerSubPix(viewGray, corners, cv::Size(dataCalib->pref.search_window_size, dataCalib->pref.search_window_size),
                                cv::Size(-1,-1), criteria);
            drawChessboardCorners(src, patternSize, cv::Mat(corners), found, cv::Scalar(0, 0, 255, 0), 0);
            viewGray.release();

            // 3D coordinates of the points
            
            for (int j = 0; j < dataCalib->calibPattern.nbSquareY - 1; ++j) {
                for (int k = 0; k < dataCalib->calibPattern.nbSquareX - 1; ++k) {
                    corners3D.push_back(cv::Point3f(
                                        k * dataCalib->calibPattern.sizeSquareX,
                                        j * dataCalib->calibPattern.sizeSquareY, 
                                        0));
                }
            }
        } else {
            dataCalib->IOcalib[i].active_image = false;
        }

        // To remove the last added as we re-evaluate it
        if (reIterated) {
            allCorners.pop_back();
            allCorners3D.pop_back();
        }
        allCorners.push_back(corners);
        allCorners3D.push_back(corners3D);


        cv::resize(src, src, dataCalib->pref.render_size, cv::INTER_LINEAR);
        
        std::string title = std::string(IMG_NAME) + " " + std::to_string(i + 1);
        cv::setWindowTitle(windowID, title);
        // Zoom functionality
        cv::Rect r = ::selectROI(windowID, src, false, true);

        // After possible selection 

        // Exit via click on exit button
        if (r.size().width == EXIT && r.size().height == EXIT) {
            wxMessageBox("All corners weren't extracted properly : you won't be able to perform calibration.",
                "Corners extraction", wxICON_ERROR);
            src.release();
            return -1;
        }
        // Next image via Enter/Y/O (no ROI selected)
        if (r.size().width == NEXT_ENTER_OK && r.size().height == NEXT_ENTER_OK) {
            if (found) {
                dataCalib->IOcalib[i].active_image = true;
            }
            reIterated = false;
            src.release();
            continue;
        }
        // Next image via N (no ROI selected)
        if (r.size().width == NEXT_ENTER_NOT_OK && r.size().height == NEXT_ENTER_NOT_OK) {
            dataCalib->IOcalib[i].active_image = false;
            src.release();
            reIterated = false;
            continue;
        }
        // Reselect ROI for this image
        if (r.size().width == RESELECT && r.size().height == RESELECT) {
            --i;
            src.release();
            reIterated = true;
            continue;
        }

        // Selection, if set, is done

        cv::Mat focused = src(r);
        cv::resize(focused, focused, src.size(), cv::INTER_LINEAR);
        cv::imshow(windowID, focused);
        focused.release();
        src.release();
        
        while(1) {
            // Exception handling necessary for windows as getWindowProperty
            // throws an exception if the window is not found (eq. closed)
            try {
                if (cv::getWindowProperty(windowID, cv::WND_PROP_AUTOSIZE) == -1) {
                    wxMessageBox("All corners weren't extracted properly : you won't be able to perform calibration.",
                        "Corners extraction", wxICON_ERROR);
                    return -1;
                }
            } catch (std::exception& e) {
                wxMessageBox("All corners weren't extracted properly : you won't be able to perform calibration.",
                        "Corners extraction", wxICON_ERROR);
                return -1;
            }
            int k = cv::waitKey(1);
            // ESC
            if (k == 27) {
                --i;
                reIterated = true;
                break;
            }
            // ENTER, O, Y
            if (k == 13 || k == 111 || k == 121) {
                if (found) {
                    dataCalib->IOcalib[i].active_image = true;
                }
                reIterated = false;
                break;
            }
            if (k == 110) {
                dataCalib->IOcalib[i].active_image = false;
                reIterated = false;
                break;
            }
        }
    }
    
    try {
        cv::destroyWindow(windowID);
    } catch (std::exception& e) {
        // Nothing : already closed.
    }
    
    int result = wxMessageBox("Was the extraction successful ?", "Corners extraction", wxYES_NO | wxICON_QUESTION);
    if (result == wxYES) {
        // Saving corners positions in 2D and 3D
        for (int i = 0; i < dataCalib->nb_images; ++i) {
            if (!dataCalib->IOcalib[i].active_image) {
                continue;
            }
            std::vector<cv::Point2f> corners = allCorners.at(i);               
            std::vector<cv::Point3f> corners3D = allCorners3D.at(i);
            dataCalib->IOcalib[i].CornersCoord2D = corners;
            dataCalib->IOcalib[i].CornersCoord3D = corners3D;
            dataCalib->IOcalib[i].active_image = true;
            
        }
    } else {
        for (int i = 0; i < dataCalib->nb_images; ++i) {
            dataCalib->IOcalib[i].active_image = false;
        }
        return -1;
    }
    return 0;
}