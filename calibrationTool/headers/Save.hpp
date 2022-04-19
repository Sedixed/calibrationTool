#pragma once

#include "structs.hpp"

/**
 * Save the current parameters for this calibration into a YML file, with 
 * name and location given by the user.
 * 
 * @param dataCalib pointer of Calib used to save its parameters in a YML file.
 * @return int      0 in case of success, -1 otherwise.
 */
int Save(Calib* dataCalib);


/**
 * Converts a vector v of Point2f into a 2x(v.size()) matrix containing
 * its data.
 * 
 * @param v        Vector of Point2f.
 * @return cv::Mat 2x(v.size()) matrix of float, containing v data.
 */
cv::Mat VecPoint2FToMat(std::vector<cv::Point2f>& v);


/**
 * Converts a vector v of Point3f into a 3x(v.size()) matrix containing
 * its data.
 * 
 * @param v        Vector of Point3f.
 * @return cv::Mat 3x(v.size()) matrix of float, containing v data.
 */
cv::Mat VecPoint3FToMat(std::vector<cv::Point3f>& v);
