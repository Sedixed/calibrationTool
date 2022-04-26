#pragma once

#include "structs.hpp"


// ============================================================================
// ----------------- Contains ExtractGridCorners function and -----------------
// ----------------- mouse callback function.                 -----------------
// ============================================================================


/**
 * Extract the grid corners from every image stored in the Calib structure.
 * For each image, displays it with the corners found highlighted and asks if the
 * extraction was successful. If not, the image is no longer active for calibration.
 * 
 * @param dataCalib pointer of Calib used to read and set its parameters.
 * @return int      0 in case of success, -1 if the extraction failed.
 */
int ExtractGridCorners(Calib *dataCalib);


/**
 * Allows the user to close a window by a left click on it, through
 * a pointer on a boolean value. ECC stands for ExtractGridCorners.
 * 
 * @param evt   Event the function is responding to : here its a left click.
 * @param x     Coordinate of the mouse position along X axis.
 * @param y     Coordinate of the mouse position along Y axis.
 * @param flags Flags used for the action of the callback : 0 here.
 * @param data  Parameters used to perform an action : here its a boolean pointer.
 */
void callbackClickECC(int evt, int x, int y, int flags, void* data);