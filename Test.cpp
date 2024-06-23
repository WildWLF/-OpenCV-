#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


struct photo{
	Mat mat;
	Rect rect;
};

bool get_ruler(Mat img,photo &target)
{
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	Mat thresh;
	threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	Mat open;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresh, open, MORPH_OPEN, kernel);
	bitwise_not(open, open);
	vector<vector<Point>> contours;
	findContours(open, contours, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(2);
	int cnt = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		double peri = arcLength(contours[i], true);
		if (area > 30000&& area < 105000)
		{
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (conPoly[i].size() == 4)
			{
				Rect box = boundingRect(contours[i]);
				if (cnt <= 1)
				{
					boundRect[cnt] = box;
					cnt++;
				}
			}
		}
	}
	if (cnt < 1) return false;
	else
	{
		int i = 1;
		double ratio = double(boundRect[0].width + boundRect[1].width) / double(boundRect[0].height + boundRect[1].height);
		if (ratio > 2 && ratio < 6)
		{
			if (boundRect[0].tl().x < boundRect[1].tl().x)
			{
				i = 0;
			}
			Rect rect(boundRect[i].tl(), boundRect[1-i].br());
			target = { img(rect),rect };
		}
	}
	if (target.mat.empty()) return false;
	return true;
}

bool Read_Data(string filename, vector<Mat>& dataset)
{
	vector<String>imagePathList;
	glob(filename, imagePathList);
	if (imagePathList.empty()) return false;

	for (int i = 0; i < imagePathList.size(); i++)
	{
		Mat image = imread(imagePathList[i]);
		resize(image, image, Size(25, 50), 1, 1, INTER_LINEAR);
		dataset.push_back(image);
	}

	return true;
}

int pixCount(Mat image)
{
	int count = 0;
	if (image.channels() == 1)
	{
		for (int i = 0; i < image.rows; i++)
		{
			for (int j = 0; j < image.cols; j++)
			{
				if (image.at<uchar>(i, j) == 0)
				{
					count++;
				}
			}
		}

		return count;
	}
	else
	{
		return -1;
	}
}

bool divide_number(photo &target,vector<photo> &numbers)
{

	string filename = "D:/OpenCV/posdata/";
	vector<Mat>dataset;
	if (!Read_Data(filename, dataset)) return false;
	Mat img = target.mat;
	Mat gray;
	cvtColor(target.mat, gray, COLOR_BGR2GRAY);

	Mat thresh;
	threshold(gray, thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(1, 1));
	Mat close;
	morphologyEx(thresh, close, MORPH_CLOSE, kernel);
	bitwise_not(close, close);
	vector<vector<Point>>contours;
	findContours(close, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);

		auto peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
		if (area > 22 && area < 180)
		{
			auto peri = arcLength(contours[i],true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			Rect rect = boundingRect(contours[i]);
			double ratio = double(rect.height) / double(rect.width);
			if (ratio > 1)
			{
				Mat roi = img(rect);
				resize(roi, roi, Size(25, 50), 1, 1, INTER_LINEAR);	 
				Mat roi_gray;
				cvtColor(roi, roi_gray, COLOR_BGR2GRAY);

				Mat roi_thresh;
				threshold(roi_gray, roi_thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

				int minCount = 300;
				int index = -1;

				for (int j = 0; j < dataset.size(); j++)
				{
					Mat temp_gray;
					cvtColor(dataset[j], temp_gray, COLOR_BGR2GRAY);

					Mat temp_thresh;
					threshold(temp_gray, temp_thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

					Mat dst;
					absdiff(roi_thresh, temp_thresh, dst);

					int count = pixCount(dst);
					if (count < minCount)
					{
						minCount = count;
						index = j;
					}

				}
				if(index!=-1) numbers.push_back({ roi ,rect });
			}
		}
	}

	for (int i = 0; i < numbers.size() - 1; i++)
	{
		for (int j = 0; j < numbers.size() - 1 - i; j++)
		{
			if (numbers[j].rect.x > numbers[j + 1].rect.x)
			{
				photo temp = numbers[j];
				numbers[j] = numbers[j + 1];
				numbers[j + 1] = temp;
			}
		}
	}
	if(numbers.size() < 4) return false;
	return true;
}



bool NumberDetection(vector<photo>& numbers, vector<int>& result_index)
{
	string filename = "D:/OpenCV/posdata/";
	vector<Mat>dataset;
	if (!Read_Data(filename, dataset)) return false;

	for (int i = 0; i < numbers.size(); i++)
	{
		Mat roi_gray;
		cvtColor(numbers[i].mat, roi_gray, COLOR_BGR2GRAY);

		Mat roi_thresh;
		threshold(roi_gray, roi_thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);
		
		int minCount = 300;
		int index = -1;
		for (int j = 0; j < dataset.size(); j++)
		{
			Mat temp_gray;
			cvtColor(dataset[j], temp_gray, COLOR_BGR2GRAY);

			Mat temp_thresh;
			threshold(temp_gray, temp_thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);
			
			Mat dst;
			absdiff(roi_thresh, temp_thresh, dst);
			
			int count = pixCount(dst);
			if (count < minCount)
			{
				minCount = count;
				index = j;
			}
		}
		result_index.push_back(index);
	}
	return true;
}

bool find_line(photo &target,photo &line_contour)
{
	Mat img = target.mat;
	Mat dst;
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	inRange(imgHSV, Scalar(0, 0, 0), Scalar(179, 252, 255), dst);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(1, 1));
	Mat close;
	morphologyEx(dst, close, MORPH_CLOSE, kernel);
	vector<vector<Point>> contours;
	bitwise_not(close, close);
	findContours(close, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area > 400)
		{
			auto peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			Rect box = boundingRect(conPoly[i]);
			line_contour = { img(box),box };
		}
	}
	if (line_contour.mat.empty()) return false;
	else return true;
}

void output_result(vector<photo> numbers, vector<int> result_index, photo line_contour)
{
	int minn = 1e9;
	int res = -1;
	bool left = false;
	double kd = 1e9;
	for (int i = 1; i < result_index.size(); i++)
	{
		int idx = result_index[i];
		if (idx == result_index[i - 1]) continue;
		else
		{
			double k =  10.0 * abs(idx - result_index[i - 1])/ abs(numbers[i].rect.tl().x - numbers[i - 1].rect.tl().x) * 1.0;
			kd = min(k, kd);
		}
	}
	for (int i = 0; i < numbers.size(); i++)
	{
		if (abs(line_contour.rect.tl().x - numbers[i].rect.tl().x) < minn)
		{
			minn = abs(line_contour.rect.tl().x - numbers[i].rect.tl().x);
			res = i;
			if (line_contour.rect.tl().x > numbers[i].rect.tl().x) left = false;
			else left = true;
		}
	}
	double ans;
	if (left||res==0) {
		ans = result_index[res] * 10 - kd * abs((numbers[res].rect.tl().x - line_contour.rect.tl().x));
	}
	else
	{
		ans = kd * abs((numbers[res].rect.tl().x - line_contour.rect.tl().x)) - result_index[res]*10;
	}
	printf("%.2fmm", fabs(ans));
}
void main()
{
	Mat img;
	img = imread("TestPhotos/K000+243_53.png");
	photo target;


	if (get_ruler(img, target) == true)
	{
		vector<photo> numbers;
		if (divide_number(target, numbers) == true)
		{
			vector<int> result_index;
			if (NumberDetection(numbers, result_index))
			{
				photo line_contour;
				if (find_line(target,line_contour))
				{
					output_result(numbers,result_index,line_contour);
					imshow("img", img);
					waitKey(0);
				}
				else
				{
					cout << "No find line" << endl;
				}
			}
			else
			{
				cout << "No detection numbers" << endl;
			}
		}
		else cout << "No divide" << endl;
	}
	else cout << "No found ruler" << endl;
}
