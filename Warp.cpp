#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat matrix;

void main()
{
	string path = "Resource/cards.jpg";

	Mat img = imread(path);
	Mat imgWarp;

	Point2f src[4] = { { 776,108 }, { 1018,84 }, { 841,357 }, { 1116,331 } };
	Point2f des[4] = { { 0.0f,0.0f }, { 250,0.0f }, { 0.0f,350 }, { 250,350 } };

	matrix = getPerspectiveTransform(src, des);
	warpPerspective(img, imgWarp, matrix, Point(250, 350));

	for (int i = 0; i < 4; i++)
	{
		circle(img, src[i], 5, Scalar(0, 0, 255),FILLED);
	}

	imshow("cards", img);
	imshow("img Warp", imgWarp);

	waitKey(0);
}