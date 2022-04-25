#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "structs.hpp"


// ============================================================================
// ------------------ Contains ComputeViewsError function. --------------------
// ============================================================================

/**
 * Todo (attendre de voir avec M. Boutteau si on met les résultats par image dans les résultats)
 * 
 * @param dataCalib              pointer of Calib used to read and set its parameters.
 * @param parent                 pointer of wxWindow that is the window in which the function was called.
 * @param show                   true if we want to show the images after reprojection, false otherwise.
 * @param perViewErrorCalculated true if the errors per view have been calculated previously, false otherwise.
 * @return int      0 in case of success, -1 otherwise.
 */
int ComputeViewsError(Calib *dataCalib, wxWindow* parent = NULL, bool show = false, bool perViewErrorCalculated = false);