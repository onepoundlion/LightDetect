// Arthour: Chengxin Liu, James Cheng
// This is a program used to detect armor plate light bar and determine the position of the armor plate
// The Program is a part of MacRoboMaster's file

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <list>


using namespace std;
using namespace cv;

// covert the input video to HSV Color Space
cv::Mat toHSV(cv::Mat InputMat){

    cv::Mat MatcvtHSV;

    cv::cvtColor(InputMat, MatcvtHSV, cv::COLOR_BGR2HSV);

    return MatcvtHSV;
}

// Filter out the choosing color, red for 0; blue for 1;
cv::Mat FilterColor(cv::Mat InputMat, int enemycolor){

    // smoothen input image 1st.
    cv::GaussianBlur(InputMat,InputMat,Size(7,7),1.5,1.5);

    int Hmin=0;
    int Hmax=0;
    int Smin=0;
    int Smax=0;
    int Vmin=0;
    int Vmax=0;

    // set red valve value (USE HSVGUI to adjust)
    if(enemycolor==0){
        Hmin=150;
        Hmax=200;
        Smin=0;
        Smax=255;
        Vmin=160;
        Vmax=255;
    }
    // set blue valve value

    // Reference value for testing purposes
    // int thresh=40;

     //Vec3b hsvPixel(InputMat.at<Vec3b>(0,0));
    // set a min and max HSV value for filtering
    // cv::Scalar minHSV = cv::Scalar(hsvPixel.val[0] - thresh, hsvPixel.val[1] - thresh, hsvPixel.val[2] - thresh);
    // cv::Scalar maxHSV = cv::Scalar(hsvPixel.val[0] + thresh, hsvPixel.val[1] + thresh, hsvPixel.val[2] + thresh);

    cv::Scalar minHSV = cv::Scalar(Hmin, Smin, Vmin);

    cv::Scalar maxHSV = cv::Scalar(Hmax, Smax, Vmax);

    // mask and result
    cv::Mat maskHSV, resultHSV;

    // Filter out desired light bar based on HSV min and max parameters
    cv::inRange(InputMat, minHSV, maxHSV, maskHSV);

    // fill with color
    cv::bitwise_and(InputMat, InputMat, resultHSV, maskHSV);

    // cv::cvtColor(resultHSV, resultHSV, cv::COLOR_BGR2HSV);

    Mat hsv_channels[3];

    cv::split(resultHSV, hsv_channels);

    resultHSV=hsv_channels[2];

    return resultHSV;

}

// Further Filter out noises
cv::Mat erode_dilate(cv::Mat InputMat){
    // Get structure element and erode and dilate.
    Mat erodeStruct = getStructuringElement(MORPH_RECT,Size(3,3));

    Mat input=InputMat.clone();

    Mat result_erode, result_dilate;

    // Change last parameter of the following 2 functions based on computation limitation
    cv::erode(input, result_erode, erodeStruct, Point(-1, -1), 4);

    cv::dilate(result_erode, result_dilate, erodeStruct, Point(-1, -1), 4);

    return result_dilate;
}

// angle calculation
double angle(Point pt1, Point pt2, Point pt0){
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

cv::Mat findRec(cv::Mat InputMat){

    Mat input = InputMat;

   // Mat output_with_Rec;

    std::vector<vector<cv::Point>> contours;

    // std::vector<cv::Vec4i> hierachy;

    cv::findContours(input,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    // Mat drawing = cv::Mat::zeros(output_with_Rec.size(), CV_8UC3);

    cv::RotatedRect boundingbox;

    cv::Scalar color;

    vector<Point> rect;

    vector<Point> polygon;

    for (int i = 0; i < contours.size();i++){

    /*
        color = cv::Scalar(0,100,0);

        drawContours( drawing,contours,i,color,1,8,hierachy,0,cv::Point());

        boundingbox = cv::minAreaRect(contours[i]);

        cv::Point2f pts[4];

        for(int p=0; p<3; p++){
            corners[i][p] = pts[p];
        }

    }
    */

        approxPolyDP(contours[i], polygon, arcLength(contours[i], 1) * 0.02, 1);

        if (isContourConvex(polygon) && polygon.size() == 4){

            double maxCosine = 0;

            for (int j = 2; j < 5; j++){
                double cosine = fabs(angle(polygon[j % 4], polygon[j - 2], polygon[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }

            if (maxCosine < 0.3){
                rect=polygon;

                // you should see a black box around the light bar
                for (int i = 0; i < rect.size(); i++){

                    cv::line(input, rect[0],rect[1],cv::Scalar(10,255,255));

                    cv::line(input, rect[1],rect[2],cv::Scalar(10,255,255));

                    cv::line(input, rect[2],rect[3],cv::Scalar(10,255,255));

                    cv::line(input, rect[3],rect[0],cv::Scalar(10,255,255));
                }
            }

        }
    }
    return input;
}


