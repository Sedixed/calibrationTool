#include "buttonsUtils.hpp"

namespace Btn {

    wxButton* btnFromId(ButtonsId id, wxWindow* parent) {
        switch(id) {
            case ID_PERSPECTIVE: return new wxButton(parent, id, "Perspective camera");
            case ID_SPHERICAL: return new wxButton(parent, id, "Spherical camera");
            case ID_HELP: return new wxButton(parent, id, "Help");
            case ID_EXIT: return new wxButton(parent, id, "Exit");
            case ID_LOAD_IMG: return new wxButton(parent, id, "Load images");
            case ID_EXTRACT_GRID_CORNERS: return new wxButton(parent, id, "Extract grid corners");
            case ID_CALIB: return new wxButton(parent, id, "Calibration");
            case ID_SHOW_CORNERS_PROJ: return new wxButton(parent, id, "Show corners projection");
            case ID_CALIB_RESULTS: return new wxButton(parent, id, "Calibration results");
            case ID_SAVE: return new wxButton(parent, id, "Save");
            case ID_PREFERENCES: return new wxButton(parent, id, "Preferences");
            default: return NULL;
        }
    }
}