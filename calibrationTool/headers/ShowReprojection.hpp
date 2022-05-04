#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"


// ============================================================================
// ------------------- Contains ShowReprojection function, --------------------
// ------------------- and an utilitary function.          --------------------
// ============================================================================


/**
 * Calculates the error per view for each active image, and then for each
 * of these images, displays it with the reprojected corners with red crosses on them,
 * and the precalculated one with blue crosses on them.
 * 
 * @param dataCalib              pointer of Calib used to read and set its parameters.
 * @param parent                 wxWindow from which the function was called.
 * @return int                   0 in case of success, -1 otherwise.
 */
int ShowReprojection(Calib *dataCalib, wxWindow* parent);


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