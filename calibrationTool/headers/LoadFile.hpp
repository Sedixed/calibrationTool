#pragma once

#include "structs.hpp"

/**
 * Load parameters for this calibration from a YML file, with 
 * name and location given by the user.
 * 
 * @param dataCalib pointer of Calib used to read its parameters from a YML file.
 * @return int      0 in case of success, -1 otherwise.
 */
int LoadFile(Calib* dataCalib);


/**
 * Converts a 2x(v.size()) matrix into a vector of Point2f containing
 * its data.
 * 
 * @param m        2x(v.size()) matrix of float.
 * @return cv::Mat Vector of Point2f, containing m data.
 */
std::vector<cv::Point2f> MatToVecPoint2F(cv::Mat& m);


/**
 * Converts a 3x(v.size()) matrix into a vector of Point3f containing
 * its data.
 * 
 * @param m        3x(v.size()) matrix of float.
 * @return cv::Mat Vector of Point3f, containing m data.
 */
std::vector<cv::Point3f> MatToVecPoint3F(cv::Mat& m);