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
	int	search_window_size;				// Size of the corners dectector window
	cv::Size render_size;				// Size of the renderer
	int parameters_flags;				// Flags to use for calibration
	int fixed_point;					// Fixed point to determine calibration function
} Preferences;


typedef struct mire {
	int nbSquareX;					// Number of squares along x					
	int nbSquareY;					// Number of squares along y
	double sizeSquareX;				// Size of a square along x (in mm)
	double sizeSquareY;				// Size of a square along y (in mm)
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
 * Calib : structure used for storing every information needed for perspective 
 * camera calibration.
 * 
 */
typedef struct calib {
    int nb_images;                      // Number of loaded images
    IOCalibration IOcalib[MAX_IMAGES];	// Images data used for the calibration
	Mire	calibPattern;				// Calibration pattern properties
	Preferences pref;					// Preferences
	cv::Mat intrinsics;					// Intrinsics parameters of the sensor
	double error;						// Mean error after calibration
} Calib;