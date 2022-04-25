#pragma once

#include "structs.hpp"


// ============================================================================
// --------------------- Contains Calibration function. -----------------------
// ============================================================================


/**
 * Realizes the calibration using data stored in the dataCalib structure.
 * It may also update the structure, depending of the flags it contains.
 * 
 * @param dataCalib pointer of Calib used to read and set its parameters.
 * @return int      0 in case of success, -1 otherwise.
 */
int Calibration(Calib* dataCalib);
