#include "../headers/Mosaic.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define MOSAIC_TITLE "Loaded images"

int Mosaic(cv::Mat images[], int nbImages, int width) {
    if (nbImages <= 0 || images == NULL) {
        wxMessageDialog(NULL, _("An error ocurred : not any images were loaded."), 
                        _(""), wxCENTER | wxICON_ERROR).ShowModal();
        return -1;
    }
    if (width <= 0) {
        wxMessageDialog(NULL, _("An error ocurred : width too low."), 
                        _(""), wxCENTER | wxICON_ERROR).ShowModal();
        return -1;
    }
    if (nbImages < 3) {
        wxMessageDialog(NULL, _("Please provide at least 3 images."), 
                        _(""), wxCENTER | wxICON_ERROR).ShowModal();
        return -1;
    }
    int n_col = (int)(floor(sqrt((double)(nbImages * images[0].cols) / (double)(images[0].rows))));
    int n_row = (int)(ceil((double)(nbImages) / (double)(n_col)));

    // Size of an element of the mosaic
    cv::Size mosaicSize;
    mosaicSize.width = images[0].cols / n_col;
	mosaicSize.height = images[0].rows / n_row;

    // Image that will be resized to fit in the final image
    cv::Mat render1 = cv::Mat(cv::Size(
                                n_col * mosaicSize.width, 
                                n_row * mosaicSize.height),
                                CV_8UC1);

    // Final image
    cv::Mat render2 = cv::Mat(cv::Size(
                                width,
                                (int) ((mosaicSize.height * n_row) * ((double)(width) / (double)(n_col * mosaicSize.width)))),
                                CV_8UC1);

    // writing data in the image

    cv::Mat tmp = cv::Mat(mosaicSize, CV_8UC1);
    // region of interest
    int ROIwidth = mosaicSize.width;
    int ROIheight = mosaicSize.height;
    
    for (int j = 0; j < n_row; ++j) {
        for (int i = 0; i < n_col; ++i) {
            if (j * n_col + i < nbImages) {
                int ROIx = i * mosaicSize.width;
                int ROIy = j * mosaicSize.height;

                cv::resize(images[j * n_col + i], tmp, tmp.size(), cv::INTER_LINEAR);
                cv::Mat r = render1(cv::Rect(ROIx, ROIy, ROIwidth, ROIheight));
                tmp.copyTo(r);
                r.release();
            } else {
                break;
            }
        }
    }

    // To ensure the new images will be displayed if it isn't
    // the first calibration on this execution
    if (cv::getWindowProperty(MOSAIC_TITLE, cv::WND_PROP_AUTOSIZE) != -1) {
        cv::destroyWindow(MOSAIC_TITLE);
    }

    cv::resize(render1, render2, render2.size(), cv::INTER_LINEAR);
    cv::imshow(MOSAIC_TITLE, render2);
    tmp.release();
    render1.release();
    render2.release();

    return 0;
}