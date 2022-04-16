#include "../headers/LoadImages.hpp"
#include "../headers/Mosaic.hpp"
#include <opencv2/opencv.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

int LoadImages(Calib* dataCalib) {
    wxString wildcard("Image files (*.bmp;*.tiff;*.jpg;*.jpeg;*.pgm;*.png)|*.bmp;*.tiff;*.jpg;*.jpeg;*.pgm;*.png");


    wxFileDialog* askImages = new wxFileDialog(NULL, "Load images", "", "", 
                                wildcard, wxFD_OPEN | wxFD_MULTIPLE);

    if (askImages->ShowModal() == wxID_OK) {
        wxArrayString paths;
        askImages->GetPaths(paths);

        // Amount of images
        int max = (int)paths.GetCount();
        if (max > MAX_IMAGES) {
            wxMessageDialog(NULL, _("An error ocurred : too many images were loaded."), 
                            _(""), wxCENTER | wxICON_ERROR).ShowModal();
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

        // Show the mosaic
        int r = Mosaic(images, max, dataCalib->pref.render_size.width);
        if (r != 0) {
            return -1;
        }
        
        for(int i = 0; i < max; ++i) {
		    images[i].release();
		}
        
        return 0;
    }
    return -1;
}