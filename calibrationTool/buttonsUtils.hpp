#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


namespace Btn {
    enum ButtonsId : int {
        ID_PERSPECTIVE = 0,
        ID_SPHERICAL,
        ID_HELP,
        ID_EXIT,
        ID_LOAD_IMG,
        ID_EXTRACT_GRID_CORNERS,
        ID_CALIB,
        ID_SHOW_CORNERS_PROJ,
        ID_CALIB_RESULTS,
        ID_SAVE,
        ID_PREFERENCES
    };

    /**
     * Returns a pointer of wxButton with an appropriate label depending on id.
     * This wxButton will be displayed on the wxWindow pointed by parent.
     * 
     * @param id            identifier of a wxButton, from ButtonsId.
     * @param parent        the wxWindow the wxButton result will be displayed on.
     * @return wxButton*    the wxButton resulting from the id given.
     */
    wxButton* btnFromId(ButtonsId id, wxWindow* parent);
}