#pragma once

#include "structs.hpp"

/**
 * ah oui oui
 * 
 * @param dataCalib pointer of Calib used to read and set its parameters.
 * @param show      1 if we want to show the images after reprojection, 0 otherwise.
 * @return int      0 in case of success, -1 otherwise.
 */
int CornersReprojection(Calib *dataCalib, int show = 0);