#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void main()
{
	string path = "TestPhotos/K000+243_53.png";

	Mat img = imread(path);
	Mat imgHSV, mask;
	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;

	
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	Mat dst;
	inRange(imgHSV, Scalar(0, 0, 0), Scalar(179, 240, 255), dst);

	imshow("dst", dst);
	waitKey(0);
	/*namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true)
	{
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		inRange(imgHSV, lower, upper, mask);

		imshow("img", img);
		imshow("imgHSV", imgHSV);
		imshow("mask", mask);

		waitKey(1);
	}*/
}