#include "../headers/LoadFile.hpp"
#include "../headers/Mosaic.hpp"
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

    // Number of images
    int nbImages;
    fs["Number of images"] >> nbImages;
    if (nbImages < 3) {
        wxMessageBox("Error : must provide at least 3 images.", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }

    // Mire data
    int nbSquareX;
    int nbSquareY;
    fs["Mire used"]["Number of squares along X"] >> nbSquareX;
    fs["Mire used"]["Number of squares along Y"] >> nbSquareY;
    if (nbSquareX < 4 || nbSquareY < 4) {
        wxMessageBox("Error : grid must be at least 4x4.", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }
    int sizeX;
    int sizeY;
    fs["Mire used"]["Size of a square along X"] >> sizeX;
    fs["Mire used"]["Size of a square along Y"] >> sizeY;
    if (sizeX <= 0 || sizeY <= 0) {
        wxMessageBox("Error : a square must have strict positive dimension.", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }
    
    // Preferences

    std::string method;
    // Perspective only
    if (dataCalib->type == PERSPECTIVE_TYPE) {
        fs["Method"] >> method;
        if (method.compare("calibrateCamera") != 0 && method.compare("calibrateCameraRO") != 0) {
            wxMessageBox("Error : invalid method", "Error loading file", wxICON_ERROR);
            fs.release();
            return -1;
        }
    }

    int renderWidth;
    int renderHeight;
    fs["Render size"]["Width"] >> renderWidth;
    fs["Render size"]["Height"] >> renderHeight;
    if (renderWidth <= 0 || renderHeight <= 0) {
        wxMessageBox("Error : invalid render size", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }

    int searchSize;
    fs["Search window size"] >> searchSize;
    if (searchSize <= 0) {
        wxMessageBox("Error : invalid search size", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }
    
    // Intrinsics parameters
    cv::Mat intrinsics;
    fs["Intrinsics parameters"] >> intrinsics;
    if (intrinsics.cols != 3 || intrinsics.rows != 3) {
        wxMessageBox("Error : invalid intrinsics matrix size", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }
    if (intrinsics.at<double>(0, 2) <= 0 || intrinsics.at<double>(1, 2) <= 0 || intrinsics.at<double>(2, 2) != 1 ||
        intrinsics.at<double>(1, 0) != 0 || intrinsics.at<double>(2, 0) != 0 || intrinsics.at<double>(2, 1) != 0) {
        wxMessageBox("Error : invalid intrinsics matrix content", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }

    // Distorsions
    cv::Mat dist;
    fs["Distorsion coefficients"] >> dist;
    if (dist.rows != 1) {
        wxMessageBox("Error : invalid distorsions matrix size", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }
    
    double Xi;
    if (dataCalib->type == SPHERICAL_TYPE) {
        fs["Xi"] >> Xi;
    }

    // Flags
    int flags;
    fs["Flags"] >> flags;
    if (flags < 0) {
        wxMessageBox("Error : invalid flags integer value", "Error loading file", wxICON_ERROR);
        fs.release();
        return -1;
    }

    // --- Results saving ---
    dataCalib->nb_images = nbImages;
    dataCalib->type = (type.compare("Perspective") == 0 ? PERSPECTIVE_TYPE : SPHERICAL_TYPE);
    dataCalib->calibPattern.nbSquareX = nbSquareX;
    dataCalib->calibPattern.nbSquareY = nbSquareY;
    dataCalib->calibPattern.sizeSquareX = sizeX;
    dataCalib->calibPattern.sizeSquareY = sizeY;
    // Mean error
    fs["Mean error"] >> dataCalib->error;
    if (dataCalib->type == PERSPECTIVE_TYPE) {
        dataCalib->pref.fixed_point = (method.compare("calibrateCamera") == 0 ? 0 : dataCalib->calibPattern.nbSquareX - 1);
    }
    dataCalib->pref.render_size.width = renderWidth;
    dataCalib->pref.render_size.height = renderHeight;
    dataCalib->pref.search_window_size = searchSize;
    dataCalib->intrinsics = intrinsics;
    dataCalib->distCoeffs = dist;
    if (dataCalib->type == SPHERICAL_TYPE) {
        dataCalib->Xi = Xi;
    }
    dataCalib->pref.parameters_flags = flags;

    // Views
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        std::string node = "View " + std::to_string(i + 1);
        cv::FileNode view(fs["Views data"][node.c_str()]);
        std::string path = (std::string)view["Path"];
        path.copy(dataCalib->IOcalib[i].image_name, path.length() + 1);
        view["View error"] >> dataCalib->IOcalib[i].errorView;
        view["Active"] >> dataCalib->IOcalib[i].active_image;
        view["Rotation vector"] >> dataCalib->IOcalib[i].rotationMat;
        view["Translation vector"] >> dataCalib->IOcalib[i].translationMat;
        cv::Mat corners2D;
        view["Image points"] >> corners2D;
        dataCalib->IOcalib[i].CornersCoord2D = MatToVecPoint2F(corners2D);
        cv::Mat corners3D;
        view["Object points"] >> corners3D;
        dataCalib->IOcalib[i].CornersCoord3D = MatToVecPoint3F(corners3D);
    }

    fs.release();

    // Show mosaic
    cv::Mat images[MAX_IMAGES];
    for (int i = 0; i < dataCalib->nb_images; ++i) {
        images[i] = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_GRAYSCALE);
        if (images[i].empty()) {
            wxMessageBox("Error : at least one image isn't correct.", "Error loading file", wxICON_ERROR);
            return -1;
        }
    }
    r = Mosaic(images, dataCalib->nb_images, dataCalib->pref.render_size.width);
    if (r != 0) {
        return -1;
    }
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