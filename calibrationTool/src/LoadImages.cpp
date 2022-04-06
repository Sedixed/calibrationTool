#include "../headers/LoadImages.hpp"
#include <opencv2/opencv.hpp>

int LoadImages(Calib* dataCalib) {
    wxString wildcard("image files (*.bmp;*.tiff;*.jpg;*.pgm;*.png)|*.bmp;*.tiff;*.jpg;*.pgm;*.png");


    wxFileDialog* askImages = new wxFileDialog(NULL, "Load images", "", "", 
                                wildcard, wxFD_OPEN|wxFD_MULTIPLE);

    if (askImages->ShowModal() == wxID_OK) {
        wxArrayString paths;
        askImages->GetPaths(paths);

        int max = (int)paths.GetCount();
        if (max > MAX_IMAGES) {
            wxMessageDialog(NULL, _("An error ocurred : too many images were loaded"), 
                            _(""), wxCENTER|wxICON_ERROR).ShowModal();
            return -1;
        }

        dataCalib->nb_images = max;

        cv::Mat images[MAX_IMAGES];

        for (int i = 0; i < max; ++i) {
            //save image path/name into dataCalib
            strcpy(dataCalib->IOcalib[i].image_name, paths[i].c_str());

            // open image to check its validity
            images[i] = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_GRAYSCALE);

            if (images[i].empty()) {
                dataCalib->IOcalib[i].active_image = false;
            } else {
                dataCalib->IOcalib[i].active_image = true;
            }
        }

        // Mosaic ? à voir

        for(int i = 0; i < max; ++i) {
		    images[i].release();
		}
        
        return 0;
    }
    return -1;
}