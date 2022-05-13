#pragma once

#include "structs.hpp"


// ============================================================================
// ------------------- Contains ExtractGridCorners function. ------------------
// ============================================================================


/**
 * Extract the grid corners from every image stored in the Calib structure.
 * For each image, displays it with the corners found with a red cross on it
 * and it finally asks if the extraction was successful. If not, images are
 * no longer active for calibration.
 * 
 * @param dataCalib pointer of Calib used to read and set its parameters.
 * @return int      0 in case of success, -1 if the extraction failed.
 */
int ExtractGridCorners(Calib *dataCalib);
