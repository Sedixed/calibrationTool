#pragma once

#include "structs.hpp"


/**
 * Extract the grid corners from every image stored in the Calib structure.
 * For each image, displays it with the corners found highlighted and asks if the
 * extraction was successful. If not, the image is no longer active for calibration.
 * 
 * @param dataCalib pointer of Calib used to read and set its parameters.
 * @return int      0 in case of success, -1 if the extraction failed.
 */
int ExtractGridCorners(Calib *dataCalib);