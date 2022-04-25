#pragma once

#include <opencv2/opencv.hpp>


// ============================================================================
// ------------------------ Contains Mosaic function. -------------------------
// ============================================================================

/**
 * Displays a mosaic of images.
 * 
 * @param images   pointer on an array of images.
 * @param nbImages number of images to show.
 * @param width    the width of the mosaic.
 * @return int     0 in case of success, -1 otherwise.
 */
int Mosaic(cv::Mat images[], int nbImages, int width);