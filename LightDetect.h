#ifndef LIGHTDETECT_H_INCLUDED
#define LIGHTDETECT_H_INCLUDED

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include <vector>

using namespace cv;

cv::Mat toHSV(cv::Mat InputMat);

cv::Mat FilterColor(cv::Mat InputMat, int enemycolor);

cv::Mat erode_dilate(cv::Mat InputMat);

cv::Mat findRec(cv::Mat InputMat);
#endif // LIGHTDETECT_H_INCLUDED
