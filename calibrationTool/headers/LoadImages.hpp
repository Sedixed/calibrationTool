#pragma once

#include "structs.hpp"

/**
 * Opens a wxFileDialog to choose multiple images.
 * 
 * @param dataCalib pointer of Calib used to set its parameters.
 * @return int      0 if images were chosen and in a reasonable amount, -1 otherwise.
 */
int LoadImages(Calib* dataCalib);
