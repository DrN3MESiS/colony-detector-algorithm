#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace cv;
using namespace std;

void show(Mat src, string name, bool doResize) {
	Mat temp_dst;
	if (doResize) {
		resize(src, temp_dst, cv::Size(), 0.25, 0.25);
	}
	else {
		temp_dst = src;
	}
	
	namedWindow(name);
	imshow(name, temp_dst);
}

Mat ApplyThreshold(Mat src) {
	Mat dst;
	threshold(src, dst, 120, 255, THRESH_BINARY);
	return dst;
}

int main() {
	/*Read Images*/
	Mat src = imread("DSC_0112.JPG");

	/*Gaussian Blur*/
	Mat gSRC;
	GaussianBlur(src, gSRC, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/*Convert to Grayscale*/
	Mat sSRC;
	cvtColor(gSRC, sSRC, COLOR_BGR2GRAY);

	/*Apply Threshold*/
	Mat tSRC = ApplyThreshold(sSRC);

	/*Apply Filters*/
	Mat canny = src.clone();
	Canny(src, canny, 120, 255);
	show(canny, "canny", true);

	waitKey(0);
	return 0;
}