#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void getContours(Mat imgDil, Mat img)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	 
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	string contourType;
	for (int i = 0; i < (int) contours.size(); i++)
	{
		auto area = contourArea(contours[i]);
		
		if (area > 1000)
		{
			auto peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			boundRect[i] = boundingRect(conPoly[i]);
			cout << area << endl;

			int objCor = (int) conPoly[i].size();
			if (objCor == 3) contourType = "Tri";
			else if (objCor == 4)
			{
				float flag = 1.0*boundRect[i].width / boundRect[i].height;
				if (flag <= 1.1 && flag >= 0.9) contourType = "Sqr";
				else contourType = "Rect";
			}
			else contourType = "Circle";

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			putText(img, contourType, {boundRect[i].x,boundRect[i].y - 5}, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0), 1);
		}
	}
}

void main()
{
	string path = "Resource/shapes.png";
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);

	getContours(imgDil, img);
	imshow("Canny", imgCanny);
	imshow("shapes", img);
	waitKey(0);
}