#pragma once

#include "structs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

// Used to set the size of the cv::Rect returned by DerivedROISelector::select for special cases
#define RESELECT 0
#define NEXT_ENTER_OK -1
#define NEXT_ENTER_NOT_OK -2
#define EXIT -3


// ============================================================================
// -------- Contains DerivedROISelector class declaration, drawCross, ---------
// -------- drawChessboardCorners and selectROI functions.            ---------
// ============================================================================



// ============================================================================
// ----------------------------------------------------------------------------
// ----------------- DerivedROISelector class declaration ---------------------
// ----------------------------------------------------------------------------
// @Desc : Modified version of the ROISelector class from OpenCV. It contains
//  all the content of the initial class, except overloaded select functions.
//  Its behaviour differs in the function select, which now has its non-used
//  parts removed (like cross drawing) and has mainly its keyboard entry 
//  behaviour modified.
// ----------------------------------------------------------------------------
// @Source : https://github.com/opencv/opencv/blob/4.x/modules/highgui/src/roiSelector.cpp
// ----------------------------------------------------------------------------
// @Cons : None
// ============================================================================
class DerivedROISelector {
    private:
        int key;
        cv::Size imageSize;

    public:
        struct handlerT {
            // Basic parameters
            bool isDrawing;
            cv::Rect2d box;
            cv::Mat image;
            cv::Point2f startPos;

            // Parameters for drawing from the center
            bool drawFromCenter;

            // Initializer list
            handlerT() : isDrawing(false), drawFromCenter(true) {};
        } selectorParams;


        /**
         * Allows users to select a ROI on the given image. The function creates a 
         * window and allows users to select a ROI using the mouse.
         * Controls :
         *  - Enter to finish selection
         *  - Escape to exit selection
         *  - If needApproval is true, O / Y / N to finish selection
         * 
         * @param windowName   Name of the window where selection process will be shown. 
         * @param img          Image to select a ROI. 
         * @param fromCenter   If true center of selection will match initial mouse position. 
         *                          In opposite case a corner of selection rectangle will 
         *                          correspont to the initial mouse position. 
         * @param needApproval If true, allows user to finish selection with O, Y and N
         * @return cv::Rect    Selected ROI, empty rect if selection canceled or special rect :
         *                          - Rect with height of NEXT_ENTER_OK if exited with Enter, O, or Y
         *                          - Rect with height of NEXT_ENTER_NOT_OK if exited with N
         *                          - Rect with height of EXIT if the window was closed manually          
         */
        cv::Rect select(const std::string &windowName, cv::Mat img, bool fromCenter = true, bool needApproval = false);

    private:
        // Empty mouse handler to reset it (not modified)
        static void emptyMouseHandler(int, int, int, int, void*);

        // Mouse handler provided by OpenCV (not modified)
        static void mouseHandler(int event, int x, int y, int flags, void *param);

        // Mouse callback provided by OpenCV (not modified)
        void opencv_mouse_callback(int event, int x, int y, int);
};


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


/**
 * Calls the function select from DerivedROISelector with the parameters given.
 * 
 * @param windowName   Name of the window where selection process will be shown. 
 * @param img          Image to select a ROI. 
 * @param fromCenter   If true center of selection will match initial mouse position. 
 *                          In opposite case a corner of selection rectangle will 
 *                          correspont to the initial mouse position. 
 * @param needApproval If true, allows user to finish selection with O, Y and N
 * @return cv::Rect    Selected ROI, empty rect if selection canceled or special rect :
 *                          - Rect with height of NEXT_ENTER_OK if exited with Enter, O, or Y
 *                          - Rect with height of NEXT_ENTER_NOT_OK if exited with N
 *                          - Rect with height of EXIT if the window was closed manually           
 */
cv::Rect selectROI(const std::string& windowName, cv::InputArray img, bool fromCenter, bool needApproval);