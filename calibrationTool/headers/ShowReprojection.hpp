#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"


// ============================================================================
// ------------------- Contains ShowReprojection function, --------------------
// ------------------- cv::drawChessboardCorners           --------------------
// ------------------- redefinition, draw crossing,        --------------------
// ------------------- utilitary function and mouse        --------------------
// ------------------- callback function.                  --------------------
// ============================================================================


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
 * Calculates the error per view for each active image, and then for each
 * of these images, displays it with the reprojected corners highlighted.
 * 
 * @param dataCalib              pointer of Calib used to read and set its parameters.
 * @param parent                 wxWindow from which the function was called.
 * @return int                   0 in case of success, -1 otherwise.
 */
int ShowReprojection(Calib *dataCalib, wxWindow* parent);


/**
 * @brief Redefinition of the drawChessboardCorners from openCV4.X : only draws crosses of color given
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
 * Allows the user to close a window by a left click on it, through
 * a pointer on a boolean value. SR stands for ShowReprojection.
 * 
 * @param evt   Event the function is responding to : here its a left click.
 * @param x     Coordinate of the mouse position along X axis.
 * @param y     Coordinate of the mouse position along Y axis.
 * @param flags Flags used for the action of the callback : 0 here.
 * @param data  Parameters used to perform an action : here its a boolean pointer.
 */
void callbackClickSR(int evt, int x, int y, int flags, void* data);


/**
 * Select the appropriate projectPoints method from openCV according to
 * the calibration type stored in dataCalib, and applies it with parameters
 * stored in dataCalib.
 * 
 * @param dataCalib       pointer of Calib used to read and set its parameters.
 * @param index           index of the image from dataCalib handled by the call.
 * @param imgPointsOutput vector of cv::Point2f containing the output reprojected points.
 * @return int            0 if the calibration type contained in dataCalib is valid, -1 otherwise.
 */
int projectPoints(Calib *dataCalib, int index, std::vector<cv::Point2f>& imgPointsOutput);