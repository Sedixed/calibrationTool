#include "../headers/LoadImages.hpp"
#include "../headers/Mosaic.hpp"
#include <opencv2/opencv.hpp>


int LoadImages(Calib* dataCalib, wxWindow* parent) {
    wxString wildcard("Image files (*.bmp;*.tiff;*.jpg;*.jpeg;*.pgm;*.png)|*.bmp;*.tiff;*.jpg;*.jpeg;*.pgm;*.png");
    wxFileDialog* askImages = new wxFileDialog(NULL, "Load images", "", "", 
                                wildcard, wxFD_OPEN | wxFD_MULTIPLE);

    if (askImages->ShowModal() == wxID_OK) {
        wxArrayString paths;
        askImages->GetPaths(paths);

        // Amount of images
        int max = (int) paths.GetCount();
        if (max > MAX_IMAGES) {
            wxMessageDialog(NULL, _("An error ocurred : too many images were loaded."), 
                            _(""), wxCENTER | wxICON_ERROR).ShowModal();
            return -1;
        }

        dataCalib->nb_images = max;
        cv::Mat images[MAX_IMAGES];

        for (int i = 0; i < max; ++i) {
            //save image path/name into dataCalib
            wxCharBuffer buffer = paths[i].ToUTF8();
            memcpy((void *)dataCalib->IOcalib[i].image_name, buffer.data(), buffer.length() + 1);

            // open image to check its validity
            images[i] = cv::imread(dataCalib->IOcalib[i].image_name, cv::IMREAD_GRAYSCALE);

            if (images[i].empty()) {
                dataCalib->IOcalib[i].active_image = false;
            } else {
                dataCalib->IOcalib[i].active_image = true;
            }
        }
        // Show the mosaic
        int r = Mosaic(images, max, dataCalib->pref.render_size.width);

        if (r != 0) {
            return -1;
        }
        parent->SetFocus();
        
        for(int i = 0; i < max; ++i) {
		    images[i].release();
		}
        
        return 0;
    }
    return -1;
}