#include "buttonsUtils.hpp"

namespace Btn {

    std::string labels[] = {
        "Perspective camera",
        "Spherical camera",
        "Help",
        "Exit",
        "Load images",
        "Extract grid corners",
        "Calibration",
        "Show corners projection",
        "Calibration results",
        "Save",
        "Preferences"
    };

    wxButton* btnFromId(ButtonsId id, wxWindow* parent) {
        return new wxButton(parent, id, labels[id]);
    }


}