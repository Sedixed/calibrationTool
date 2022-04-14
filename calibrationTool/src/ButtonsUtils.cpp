#include "../headers/ButtonsUtils.hpp"

namespace Btn {

    wxButton* btnFromId(ButtonsId id, wxWindow* parent) {
        return new wxButton(parent, id, mainLabels[id]);
    }

    const std::vector<wxButton*> baseButtons(wxWindow* parent) {
        return std::vector<wxButton*> {
            new wxButton(parent, ID_PERSPECTIVE, mainLabels[ID_PERSPECTIVE]),
            new wxButton(parent, ID_SPHERICAL, mainLabels[ID_SPHERICAL]),
            new wxButton(parent, ID_HELP, mainLabels[ID_HELP]),
            new wxButton(parent, ID_EXIT, mainLabels[ID_EXIT])
        };
    }

    const std::vector<wxButton*> perspectiveButtons(wxWindow* parent) {
        return std::vector<wxButton*> {
            new wxButton(parent, ID_LOAD_IMG, mainLabels[ID_LOAD_IMG]),
            new wxButton(parent, ID_EXTRACT_GRID_CORNERS, mainLabels[ID_EXTRACT_GRID_CORNERS]),
            new wxButton(parent, ID_CALIB, mainLabels[ID_CALIB]),
            new wxButton(parent, ID_SHOW_CORNERS_PROJ, mainLabels[ID_SHOW_CORNERS_PROJ]),
            new wxButton(parent, ID_CALIB_RESULTS, mainLabels[ID_CALIB_RESULTS]),
            new wxButton(parent, ID_SAVE, mainLabels[ID_SAVE]),
            new wxButton(parent, ID_LOAD_FILE, mainLabels[ID_LOAD_FILE]),
            new wxButton(parent, ID_PREFERENCES, mainLabels[ID_PREFERENCES]),
            new wxButton(parent, ID_EXIT, mainLabels[ID_EXIT])
        };
    }

    const std::vector<wxButton*> preferencesButtons(wxWindow* parent) {
        return std::vector<wxButton*> {
            new wxButton(parent, ID_EXIT_CANCEL, mainLabels[ID_EXIT_CANCEL]),
            new wxButton(parent, ID_EXIT_OK, mainLabels[ID_EXIT_OK])
        };
    }
}