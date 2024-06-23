#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void main()
{
	string path = "Resource/test.png";
	Mat img = imread(path);
	Mat imgResize,imgCrop;
	resize(img, imgResize, Size(),0.75,0.75);

	Rect roi(100, 100, 400, 300);
	imgCrop = img(roi);

	imshow("img", img);
	imshow("imgResize", imgResize);
	imshow("imgCrop", imgCrop);
	waitKey(0);
}