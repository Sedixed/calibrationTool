#pragma once
// Most structures used throughout the app will be defined here

#define MAX_FILE_NAME 200
#define MAX_IMAGES 200

/**
 * IOCalibration : structure used for storing informations relatives to images used for
 * calibration.
 * 
 */
typedef struct iocalibration {
	// Input
	char image_name[MAX_FILE_NAME] {'\0'};		// Image path and name
	bool active_image;		            // Is the image useful for calibration ? 0 -> don't use the image

    /* Pas encore utilisé
	// Output
	CvMat		*CornersCoord2D;		// 2D coordinates of plane corners
	CvMat		*extrinsics_plane;		// extrinsics parameters of each plane (pose)
    */

} IOCalibration;


/**
 * @brief 
 * 
 */
typedef struct calib {
    int nb_images;                      // number of loaded images
    IOCalibration IOcalib[MAX_IMAGES];	// images data used for the calibration

    // à suivre..
} Calib;