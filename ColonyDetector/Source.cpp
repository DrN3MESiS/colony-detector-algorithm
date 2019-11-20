#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

using namespace cv;
using namespace std;

void show(Mat src, string name) {	
	namedWindow(name);
	imshow(name, src);
}

Mat ApplyThreshold(Mat src) {
	Mat dst;
	threshold(src, dst, 120, 255, THRESH_BINARY);
	return dst;
}

Mat resizeIMG(Mat src, float per) {
	Mat temp_dst;
	resize(src, temp_dst, cv::Size(), per, per);
	return temp_dst;
}

Mat prepareImage(Mat src, bool debug) {
	Mat temp = src.clone();
	if (debug) {
		src = resizeIMG(src, 0.40);
	}
	/*Gaussian Blur*/
	Mat gSRC;
	GaussianBlur(temp, gSRC, Size(3, 3), 0, 0, BORDER_DEFAULT);
	/*Convert to Grayscale*/
	Mat sSRC;
	cvtColor(gSRC, sSRC, COLOR_BGR2GRAY);

	Mat canny = src.clone();
	Canny(src, canny, 120, 120*2);

	vector<vector<Point>> contours;
	vector<vector<Point>> filtered_contours;
	vector<Vec4i> hierarchy;

	findContours(canny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (int i = 0; i < contours.size(); i++) {
		float area = contourArea(contours[i]);
		if (area > 10) {
			filtered_contours.push_back(contours[i]);
		}
	}

	Mat drawing = Mat::zeros(canny.size(), CV_8UC3);
	for (int i = 0; i < filtered_contours.size(); i++)
	{
		Scalar color = Scalar(0,255,0);
		drawContours(drawing, filtered_contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	
	return drawing;
}

int main() {
	/*Read Images*/
	Mat src0 = imread("DSC_0112.JPG");
	Mat src = prepareImage(src0, true);
	show(resizeIMG(src, .60), "bruh");

	waitKey(0);
	return 0;
}