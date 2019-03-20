#include <iostream>
#include <stdio.h>
#include "LightDetect.h"
#include <vector>

using namespace std;
using namespace cv;


int main()
{
    Mat src;
    Mat inHSV;
    Mat input;
    Mat resultFilter, After_dilate, withrect;
    cv::VideoCapture cap(0);
    auto i = cap.get(CV_CAP_PROP_EXPOSURE);
    std::cout << "Exposure \n" << i << std::endl;
    int exposure=-7;
    cap.set(CV_CAP_PROP_EXPOSURE, exposure);
    std::cout << "new Exposure " << cap.get(CV_CAP_PROP_EXPOSURE) << std::endl;
    for (;;) {
        Mat frame;
        cap>>frame;
        cv::imshow("Original feed", frame);
        inHSV=toHSV(frame);
        resultFilter=FilterColor(inHSV, 0);
        After_dilate=erode_dilate(resultFilter);
        withrect=findRec(After_dilate);
        cv::imshow("Filter Prototype I", withrect);
        char c = cvWaitKey(50);
        if (c == 27)
            break;
	}
	return 0;
}
