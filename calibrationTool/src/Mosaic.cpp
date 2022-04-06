#include "../headers/Mosaic.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define MOSAIC_TITLE "Loaded images"

int Mosaic(cv::Mat images[], int nbImages, int width) {
    if (nbImages <= 0 || images == NULL) {
        wxMessageDialog(NULL, _("An error ocurred : not any images were loaded."), 
                        _(""), wxCENTER|wxICON_ERROR).ShowModal();
        return -1;
    }
    if (width <= 0) {
        wxMessageDialog(NULL, _("An error ocurred : width too low."), 
                        _(""), wxCENTER|wxICON_ERROR).ShowModal();
        return -1;
    }
    int n_col = (int)(floor(sqrt((double)(nbImages * images[0].cols) / (double)(images[0].rows))));
    int n_row = (int)(ceil((double)(nbImages) / (double)(n_col)));

    // Mosaic size
    cv::Size mosaicSize;
    mosaicSize.width = images[0].cols / n_col;
	mosaicSize.height = images[0].rows / n_row;

    cv::Mat render1 = cv::Mat(cv::Size(
                                n_col * mosaicSize.width, 
                                n_row * mosaicSize.height),
                                CV_8UC1);
    cv::Mat render2 = cv::Mat(cv::Size(
                                width,
                                (int) ((mosaicSize.height * n_row) * ((double)(width) / (double)(n_col * mosaicSize.width)))),
                                CV_8UC1);


    // writing data in the image

    cv::Mat tmp = cv::Mat(mosaicSize, CV_8UC1);
    // region of interest
    cv::Rect ROI;
    ROI.width = mosaicSize.width;
    ROI.height = mosaicSize.height;


    // TODO : revoir INTER_LINEAR, ROI, fonction image (eq to setImageROI)
    for (int j = 0; j < n_row; ++j) {
        for (int i = 0; i < n_col; ++i) {
            if (j * n_col + i < nbImages) {
                ROI.x = i * mosaicSize.width;
                ROI.y = j * mosaicSize.height;
                cv::resize(images[j * n_col + i], tmp, cv::INTER_LINEAR);
                render1 = cv::image(ROI);
            } else {
                break;
            }
        }
    }

    cv::resize(render1, render2, cv::INTER_LINEAR);

    return 0;
}