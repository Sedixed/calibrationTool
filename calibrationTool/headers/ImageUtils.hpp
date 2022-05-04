#pragma once

#include "structs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

/**
 * Draw a cross on an image.
 * 
 * @param _image    The image to draw the cross on.
 * @param x         The x coordinate of the cross.
 * @param y         The y coordinate of the cross.
 * @param size      Size of the cross in pixels.
 * @param type      0 = vertical cross, 1 = 45 degrees cross.
 * @param thickness Thickness of the cross.
 * @param color     Color of the cross.
 */
void drawCross(cv::InputOutputArray _image, double x, double y, int size, int type, int thickness, cv::Scalar color);


/**
 * Redefinition of the drawChessboardCorners from openCV4.X : only draws crosses of color given
 * by the cv::Scalar color.
 * 
 * @param image           Destination image. It must be an 8-bit color image. 
 * @param patternSize     Number of inner corners per a chessboard row and column
 * @param _corners        Array of detected corners.
 * @param patternWasFound Parameter indicating whether the complete board was found or not.
 * @param color           Color the circles must be drawn with.
 * @param type            Type of cross
 */ 
void drawChessboardCorners(cv::InputOutputArray image, cv::Size patternSize, cv::InputArray _corners, bool patternWasFound, cv::Scalar color, int type);


void performZoom(Calib* dataCalib, cv::InputOutputArray image, cv::Point2f mousePos, std::string& windowID);


cv::Rect selectROI(const std::string& windowName, cv::InputArray img, bool fromCenter);


class DerivedROISelector {
    private:
        // save the keypressed character
        int key;
        cv::Size imageSize;

    public:
        struct handlerT {
            // basic parameters
            bool isDrawing;
            cv::Rect2d box;
            cv::Mat image;
            cv::Point2f startPos;

            // parameters for drawing from the center
            bool drawFromCenter;

            // initializer list
            handlerT() : isDrawing(false), drawFromCenter(true){};
        } selectorParams;

        cv::Rect select(const std::string &windowName, cv::Mat img, bool fromCenter = true);

    private:
        static void emptyMouseHandler(int, int, int, int, void*);

        static void mouseHandler(int event, int x, int y, int flags, void *param);

        void opencv_mouse_callback(int event, int x, int y, int);
};