#pragma once

#include <opencv2/opencv.hpp>
// Most structures used throughout the app will be defined here

#define MAX_FILE_NAME 200
#define MAX_IMAGES 200

/**
 * Preferences : structure used to define the preferences for calibration.
 * 
 */
typedef struct preferences {
	int	search_window_size;				// size of the corners dectector window
	cv::Size render_size;				// size of the renderer
} Preferences;


typedef struct mire {
	int nbSquareX;					// number of squares along x					
	int nbSquareY;					// number of squares along y
	double sizeSquareX;				// size of a square along x (in mm)
	double sizeSquareY;				// size of a square along y (in mm)
} Mire;

/**
 * IOCalibration : structure used for storing informations relatives to images used for
 * calibration.
 * 
 */
typedef struct iocalibration {
	// Input
	char image_name[MAX_FILE_NAME] {'\0'};		// Image path and name
	bool active_image;		           			// Is the image useful for calibration ? 0 -> don't use the image

	// Output
	std::vector<cv::Point2f>		CornersCoord2D;		// 2D coordinates of plane corners
	std::vector<cv::Point3f>        CornersCoord3D;		// 3D coordinates of plane corners
	cv::Mat							rotationMat;	    // Rotation matrice (3 rows and 1 column)
	cv::Mat							translationMat;		// Translation matrice (3 rows and 1 column)
} IOCalibration;


/**
 * @brief 
 * 
 */
typedef struct calib {
    int nb_images;                      // number of loaded images
    IOCalibration IOcalib[MAX_IMAGES];	// images data used for the calibration
	Mire	calibPattern;				// calibration pattern properties
	Preferences pref;					// preferences
	cv::Mat intrinsics;					// intrinsics parameters of the sensor
	double error;						// error after calibration
    // à suivre..
} Calib;