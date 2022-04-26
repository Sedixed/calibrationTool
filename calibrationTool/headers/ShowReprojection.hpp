#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"


// ============================================================================
// ------------------- Contains ShowReprojection function, --------------------
// ------------------- utilitary function and mouse        --------------------
// ------------------- callback function.                  --------------------
// ============================================================================


/**
 * Calculates the error per view, and then for each image, 
 * displays it with the reprojected corners highlighted
 * extraction was successful.
 * 
 * @param dataCalib              pointer of Calib used to read and set its parameters.
 * @param parent                 wxWindow from which the function was called.
 * @return int                   0 in case of success, -1 otherwise.
 */
int ShowReprojection(Calib *dataCalib, wxWindow* parent);


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