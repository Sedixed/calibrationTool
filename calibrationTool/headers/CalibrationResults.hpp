#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"


// ============================================================================
// ------------------ Contains CalibrationResults function. -------------------
// ============================================================================


/**
 * Calculates the error per view and then displays in a 
 * wxMessageBox the results of the current calibration.
 * 
 * @param dataCalib              pointer of Calib used to read and set its parameters.
 * @return int                   0 in case of success, -1 otherwise.
 */
int CalibrationResults(Calib *dataCalib);