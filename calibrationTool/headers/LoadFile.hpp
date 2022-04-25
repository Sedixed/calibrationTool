#pragma once

#include "structs.hpp"


// ============================================================================
// ---- Contains LoadFile function, and some utilitaries functions to      ----
// ---- translate a matrix into a vector of points.                        ----
// ============================================================================


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


/**
 * Tests if the type stored as a string in a YML file is valid, depending
 * on the current calibration type (spherical / perspective).
 * 
 * @param type      The type of calibration stored as a string in a YML file.
 * @param dataCalib pointer of Calib used to read the calibration type.
 * @return int      Returns true if the types are matching.
 */
bool IsValidType(std::string& type, Calib* dataCalib);