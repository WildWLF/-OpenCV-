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
	GaussianBlur(src, imgBlur, Size(3, 3), 3, 0);/*��˹ģ��*/
	Canny(imgBlur, imgCanny, 25, 75);/*��Ե��*/

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));/*����ָ����״��ߴ�*/
	dilate(imgCanny, imgDil, kernel);/*��Ե����*/
	erode(imgDil, imgErode, kernel);/*��Ե��ʴ*/

	imshow("img", src);
	imshow("imgGray", imgGray);
	imshow("imgBlur", imgBlur);
	imshow("imgCanny", imgCanny);
	imshow("imgDil", imgDil);
	imshow("imgErode", imgErode);
	waitKey(0);
}