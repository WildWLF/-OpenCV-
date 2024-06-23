#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void main()
{
	string path = "Resource\\test.png";
	Mat src = imread(path);
	Mat imgGray, imgCanny, imgBlur, imgDil, imgErode;

	cvtColor(src, imgGray,COLOR_BGR2GRAY);
	GaussianBlur(src, imgBlur, Size(3, 3), 3, 0);/*高斯模糊*/
	Canny(imgBlur, imgCanny, 25, 75);/*边缘化*/

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));/*返回指定形状与尺寸*/
	dilate(imgCanny, imgDil, kernel);/*边缘膨胀*/
	erode(imgDil, imgErode, kernel);/*边缘侵蚀*/

	imshow("img", src);
	imshow("imgGray", imgGray);
	imshow("imgBlur", imgBlur);
	imshow("imgCanny", imgCanny);
	imshow("imgDil", imgDil);
	imshow("imgErode", imgErode);
	waitKey(0);
}