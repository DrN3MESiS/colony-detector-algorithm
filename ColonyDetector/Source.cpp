#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace cv;
using namespace std;

void show(Mat src, string name) {
	namedWindow(name);
	imshow(name, src);
}

int main() {

	/*Read Images*/
	Mat src = imread("DSC_0112.JPG", IMREAD_GRAYSCALE);

	show(src, "bruh");

	waitKey(0);
	return 0;
}