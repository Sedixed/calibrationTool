#include "../headers/ButtonsUtils.hpp"

namespace Btn {

    wxButton* btnFromId(ButtonsId id, wxWindow* parent) {
        return new wxButton(parent, id, labels[id]);
    }

    const std::vector<wxButton*> baseButtons(wxWindow* parent) {
        return std::vector<wxButton*> {
            new wxButton(parent, ID_PERSPECTIVE, labels[ID_PERSPECTIVE]),
            new wxButton(parent, ID_SPHERICAL, labels[ID_SPHERICAL]),
            new wxButton(parent, ID_HELP, labels[ID_HELP]),
            new wxButton(parent, ID_EXIT, labels[ID_EXIT])
        };
    }

    const std::vector<wxButton*> perspectiveButtons(wxWindow* parent) {
        return std::vector<wxButton*> {
            new wxButton(parent, ID_LOAD_IMG, labels[ID_LOAD_IMG]),
            new wxButton(parent, ID_EXTRACT_GRID_CORNERS, labels[ID_EXTRACT_GRID_CORNERS]),
            new wxButton(parent, ID_CALIB, labels[ID_CALIB]),
            new wxButton(parent, ID_SHOW_CORNERS_PROJ, labels[ID_SHOW_CORNERS_PROJ]),
            new wxButton(parent, ID_CALIB_RESULTS, labels[ID_CALIB_RESULTS]),
            new wxButton(parent, ID_SAVE, labels[ID_SAVE]),
            new wxButton(parent, ID_PREFERENCES, labels[ID_PREFERENCES]),
            new wxButton(parent, ID_EXIT, labels[ID_EXIT])
        };
    }

}