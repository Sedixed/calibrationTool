#include "../headers/LoadFile.hpp"
#include <opencv2/opencv.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

int LoadFile(Calib* dataCalib) {
    wxFileDialog dialog(NULL, _("Load YML file"), "", "",
                       "YML files (*.yml)|*.yml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    int r = dialog.ShowModal();
    if (r == wxID_CANCEL) {
        return -1;
    }
    cv::FileStorage fs(std::string(dialog.GetPath().mb_str()), cv::FileStorage::READ | cv::FileStorage::FORMAT_YAML);

    // Type check
    std::string type;
    fs["Type"] >> type;
    if (!IsValidType(type, dataCalib)) {
        std::string curType;
        switch(dataCalib->type) {
            case SPHERICAL_TYPE:
                curType = "Spherical";
                break;
            case PERSPECTIVE_TYPE:
                curType = "Perspective";
                break;
            default:
                curType = "Another";
                break;
        }
        wxMessageBox("Incompatible type of calibration for this file.\nYou have currently selected the \"" +
                    curType + "\" calibration type, and the YML file has the \"" + type + "\" calibration type.\n", "Load YML file", wxICON_ERROR);
        fs.release();
        return -1;
    }

    dataCalib->type = (type.compare("Perspective") == 0 ? PERSPECTIVE_TYPE : SPHERICAL_TYPE);

    fs["Number of images"] >> dataCalib->nb_images;
    // Mire data
    fs["Mire used"]["Number of squares along X"] >> dataCalib->calibPattern.nbSquareX;
    fs["Mire used"]["Number of squares along Y"] >> dataCalib->calibPattern.nbSquareY;
    fs["Mire used"]["Size of a square along X"] >> dataCalib->calibPattern.sizeSquareX;
    fs["Mire used"]["Size of a square along Y"] >> dataCalib->calibPattern.sizeSquareY;
    // Mean error
    fs["Mean error"] >> dataCalib->error;

    // Preferences

    // Perspective only
    if (dataCalib->type == PERSPECTIVE_TYPE) {
        std::string method;
        fs["Method"] >> method;
        dataCalib->pref.fixed_point = (method.compare("calibrateCamera") == 0 ? 0 : dataCalib->calibPattern.nbSquareX - 1);
    }
    
    fs["Render size"]["Width"] >> dataCalib->pref.render_size.width;
    fs["Render size"]["Height"] >> dataCalib->pref.render_size.height;
    fs["Search window size"] >> dataCalib->pref.search_window_size;
    // Intrinsics parameters
    fs["Intrinsics parameters"] >> dataCalib->intrinsics;
    // Distorsions
    fs["Distorsion coefficients"] >> dataCalib->distCoeffs;

    if (dataCalib->type == SPHERICAL_TYPE) {
        fs["Xi"] >> dataCalib->Xi;
    }

    // Flags
    fs["Flags"] >> dataCalib->pref.parameters_flags;

    // Views
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        std::string node = "View " + std::to_string(i + 1);
        cv::FileNode view(fs["Views data"][node.c_str()]);
        std::string path = (std::string)view["Path"];
        path.copy(dataCalib->IOcalib[i].image_name, path.length() + 1);
        view["View error"] >> dataCalib->IOcalib[i].errorView;
        view["Rotation vector"] >> dataCalib->IOcalib[i].rotationMat;
        view["Translation vector"] >> dataCalib->IOcalib[i].translationMat;
        cv::Mat corners2D;
        view["Image points"] >> corners2D;
        dataCalib->IOcalib[i].CornersCoord2D = MatToVecPoint2F(corners2D);
        cv::Mat corners3D;
        view["Object points"] >> corners3D;
        dataCalib->IOcalib[i].CornersCoord3D = MatToVecPoint3F(corners3D);
        // dégagera probablement
        dataCalib->IOcalib[i].active_image = true;
    }

    fs.release();
    return 0;
}


std::vector<cv::Point2f> MatToVecPoint2F(cv::Mat& m) {
    std::vector<cv::Point2f> v;
    for (int i = 0; i < m.cols; ++i) {
        cv::Point2f p;
        p.x = m.at<float>(0, i);
        p.y = m.at<float>(1, i);
        v.push_back(p);
    }
    return v;
}


std::vector<cv::Point3f> MatToVecPoint3F(cv::Mat& m) {
    std::vector<cv::Point3f> v;
    for (int i = 0; i < m.cols; ++i) {
        cv::Point3f p;
        p.x = m.at<float>(0, i);
        p.y = m.at<float>(1, i);
        p.z = m.at<float>(2, i);
        v.push_back(p);
    }
    return v;
}


bool IsValidType(std::string& type, Calib* dataCalib) {
    return (
        (type.compare("Perspective") == 0 && dataCalib->type == PERSPECTIVE_TYPE) ||
        (type.compare("Spherical") == 0 && dataCalib->type == SPHERICAL_TYPE)
    );
}