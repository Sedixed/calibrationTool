#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"


// ============================================================================
// ------------------- Contains ShowReprojection function. --------------------
// ============================================================================


/**
 * C alculates the error per view if perErrorViewCalculated is false. Then,
 * for each image, displays it with the reprojected corners highlighted
 * extraction was successful.
 * 
 * @param dataCalib              pointer of Calib used to read and set its parameters.
 * @param parent                 wxWindow from which the function was called.
 * @param perErrorViewCalculated true if the errors per view have been calculated previously, false otherwise.
 * @return int                   0 in case of success, -1 otherwise.
 */
int ShowReprojection(Calib *dataCalib, wxWindow* parent, bool perErrorViewCalculated);