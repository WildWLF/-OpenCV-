#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void main()
{
	Mat img(512, 512, CV_8UC3, Scalar(255,255,255));
	
	circle(img, Point(256, 256), 128, Scalar(255, 0, 255),FILLED);
	rectangle(img, Point(192, 192), Point(320 , 320), Scalar(255, 255, 255), FILLED);
	line(img, Point(192, 128), Point(320, 128), 2);
	putText(img, "Welcome To My World", Point(137, 262),FONT_HERSHEY_DUPLEX,0.75,Scalar(0,0,0),1);

	imshow("img", img);
	waitKey(0);

}