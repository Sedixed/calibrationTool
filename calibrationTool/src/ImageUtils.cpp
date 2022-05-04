#include "../headers/ImageUtils.hpp"

void drawCross(cv::InputOutputArray _image, double x, double y, int size, int type, int thickness, cv::Scalar color) {
    using namespace cv;

    // Get header for input arrays
    Mat image = _image.getMat();
    
    if(type == 0) {
        line( image, Point((int)(x-size), (int)(y)), Point((int)(x+size), (int)(y)), color, thickness );
        line( image, Point((int)(x), (int)(y-size)), Point((int)(x), (int)(y+size)), color, thickness );
    } else {
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
                drawCross(image, pt.x, pt.y, 4, type, 1, color);
            }
        }
    }
}


void performZoom(Calib* dataCalib, cv::InputOutputArray image, cv::Point2f mousePos, std::string& windowID) {
    int w = dataCalib->pref.render_size.width;
    int h = dataCalib->pref.render_size.height;

    int roi_w = w / 4;
    int roi_h = h / 4;

    float xPos = mousePos.x;
    float yPos = mousePos.y;

    // ROI rect
    cv::Rect roi(xPos - (roi_w / 2), yPos - (roi_h / 2), roi_w, roi_h);
    // ROI image
    cv::Mat roiImg(cv::Size(roi_w, roi_h), CV_8UC1);
    cv::resize(image, roiImg, roiImg.size(), cv::INTER_LINEAR);
    cv::imshow(windowID, roiImg);
}


cv::Rect DerivedROISelector::select(const std::string &windowName, cv::Mat img, bool fromCenter) {
    using namespace cv;

    key = 0;
    imageSize = img.size();

    // Set the drawing mode
    selectorParams.drawFromCenter = fromCenter;

    // Show the image and give feedback to user
    imshow(windowName, img);

    // Copy the data, rectangle should be drawn in the fresh image
    selectorParams.image = img.clone();

    // Object selection
    setMouseCallback(windowName, mouseHandler, (void*)this);

    // end selection process on SPACE (32) or ENTER (13)
    while (!(key == 32 || key == 13)) {
        // Draw the selected object
        rectangle(selectorParams.image, selectorParams.box, Scalar(255, 0, 0), 2, 1);

        // Show the image bounding box
        imshow(windowName, selectorParams.image);

        // Reset the image
        selectorParams.image = img.clone();

        // Get keyboard event
        key = waitKey(30);

        // Cancel option (ESC)
        if (key == 27) {
            selectorParams.box = Rect();
            break;
        }
    }

    // Cleanup callback
    setMouseCallback(windowName, emptyMouseHandler, NULL);

    return selectorParams.box;
}


void DerivedROISelector::emptyMouseHandler(int, int, int, int, void*) {}


void DerivedROISelector::mouseHandler(int event, int x, int y, int flags, void *param) {
    DerivedROISelector *self = static_cast<DerivedROISelector *>(param);
    self->opencv_mouse_callback(event, x, y, flags);
}


void DerivedROISelector::opencv_mouse_callback(int event, int x, int y, int) {
    using namespace cv;
    switch (event) {
        // update the selected bounding box
        case EVENT_MOUSEMOVE:
            if (selectorParams.isDrawing) {
                if (selectorParams.drawFromCenter) {
                    // limit half extends to imageSize
                    float halfWidth = std::min(std::min(
                            std::abs(x - selectorParams.startPos.x),
                            selectorParams.startPos.x),
                            imageSize.width - selectorParams.startPos.x);
                    float halfHeight = std::min(std::min(
                            std::abs(y - selectorParams.startPos.y),
                            selectorParams.startPos.y),
                            imageSize.height - selectorParams.startPos.y);

                    selectorParams.box.width = halfWidth * 2;
                    selectorParams.box.height = halfHeight * 2;
                    selectorParams.box.x = selectorParams.startPos.x - halfWidth;
                    selectorParams.box.y = selectorParams.startPos.y - halfHeight;

                } else {
                    // limit x and y to imageSize
                    int lx = std::min(std::max(x, 0), imageSize.width);
                    int by = std::min(std::max(y, 0), imageSize.height);
                    selectorParams.box.width = std::abs(lx - selectorParams.startPos.x);
                    selectorParams.box.height = std::abs(by - selectorParams.startPos.y);
                    selectorParams.box.x = std::min((float)lx, selectorParams.startPos.x);
                    selectorParams.box.y = std::min((float)by, selectorParams.startPos.y);
                }
            }
            break;

        // start to select the bounding box
        case EVENT_LBUTTONDOWN:
            selectorParams.isDrawing = true;
            selectorParams.box = Rect2d(x, y, 0, 0);
            selectorParams.startPos = Point2f((float)x, (float)y);
            break;

        // cleaning up the selected bounding box
        case EVENT_LBUTTONUP:
            selectorParams.isDrawing = false;
            if (selectorParams.box.width < 0) {
                selectorParams.box.x += selectorParams.box.width;
                selectorParams.box.width *= -1;
            }
            if (selectorParams.box.height < 0) {
                selectorParams.box.y += selectorParams.box.height;
                selectorParams.box.height *= -1;
            }
            break;
    }
}


cv::Rect selectROI(const std::string& windowName, cv::InputArray img, bool fromCenter) {
    DerivedROISelector selector;
    return selector.select(windowName, img.getMat(), fromCenter);
}