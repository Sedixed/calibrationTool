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
 * @param dataCalib pointer of Calib used to read and set its parameters.
 * @param parent    pointer of wxWindow that is the window in which the function was called.
 * @param show      1 if we want to show the images after reprojection, 0 otherwise.
 * @return int      0 in case of success, -1 otherwise.
 */
int ComputeViewsError(Calib *dataCalib, wxWindow* parent = NULL, int show = 0);