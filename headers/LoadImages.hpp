#pragma once

#include "structs.hpp"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// ============================================================================
// ---------------------- Contains LoadImages function. -----------------------
// ============================================================================

/**
 * Opens a wxFileDialog to choose multiple images.
 * 
 * @param dataCalib pointer of Calib used to set its parameters.
 * @param parent    pointer of wxWindow that is the window in which the function was called.
 * @return int      0 if images were chosen and in a reasonable amount, -1 otherwise.
 */
int LoadImages(Calib* dataCalib, wxWindow* parent);
