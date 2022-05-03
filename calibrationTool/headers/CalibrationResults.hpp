#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"
#include <wx/grid.h>


// ============================================================================
// ------------------ Contains CalibrationResults function and ----------------
// ------------------ standard deviation function.             ----------------
// ============================================================================


/**
 * Calculates the error per view and then displays in a 
 * wxFrame the results of the current calibration.
 * 
 * @param dataCalib              pointer of Calib used to read and set its parameters.
 * @param parent                 wxWindow from which the function was called.
 * @return int                   0 in case of success, -1 otherwise.
 */
int CalibrationResults(Calib *dataCalib, wxWindow* parent);


/**
 * Calculates the standard deviation of the errors per view.
 * 
 * @param dataCalib Pointer of calib containing the errors per view.
 * @return double   Standard deviation of the errors per view.
 */
double standardDeviation(Calib *dataCalib);