#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <iomanip>

using namespace cv;
using namespace std;

int col_counter = 0;

void show(Mat src, string name) {	
	namedWindow(name);
	imshow(name, src);
}

Mat ApplyThreshold(Mat src, int thresh) {
	Mat dst;
	threshold(src, dst, thresh, 255, THRESH_BINARY);
	return dst;
}

Mat resizeImage(Mat src, double per) {
	Mat temp_dst;
	resize(src, temp_dst, cv::Size(), per, per);
	return temp_dst;
}

Mat prepareImage(Mat src, bool filter, int minVal, int maxVal, int thresh, int range) {
	Mat temp = src.clone();
	Mat originalResized;

	//Resize Image
	originalResized = temp;

	//Convert to Grayscale
	cvtColor(temp, temp, COLOR_BGR2GRAY);
	cout  << "[10%] Converted to Grayscale." << endl;

	//Apply Blurs
	blur(temp, temp, Size(11, 11));
	blur(temp, temp, Size(11, 11));
	GaussianBlur(temp, temp, Size(11, 11), 11, 11);
	cout << "[17%] Applied Blurs" << endl;

	//Apply Threshold
	temp = ApplyThreshold(temp, thresh);
	erode(temp, temp, (500, 500));
	cout << "[25%] Applied Threshold" << endl;
	//show(resizeImage(temp, .35), "threshold");

	vector<Vec3f> circles;
	
	//Find Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	cout << "[60%] Found Contours" << endl;

	//Filtering Contours by Area
	vector<vector<Point>> filtered_contours;
	vector<Vec4i> filtered_hierarchy;
	for (int i = 0; i < contours.size(); i++) {
		float area = contourArea(contours[i]);
		if (filter) {
			if (area > minVal && area < maxVal) {
				filtered_contours.push_back(contours[i]);
				filtered_hierarchy.push_back(hierarchy[i]);
			}
		}
		else {
			filtered_contours.push_back(contours[i]);
			filtered_hierarchy.push_back(hierarchy[i]);
		}
	}
	contours = filtered_contours;
	hierarchy = filtered_hierarchy;
	cout << "[75%] Filtered Contours" << endl;

	/* INICIO ANALISIS COLOR DE CONTORNO*/
	
	//Filter by Compositions	
	vector<vector<Point>> final_contours;
	vector<Vec4i> final_hierarchy;

	/*Calculate Contourn Centroid*/
	
	vector<Moments> mu(contours.size());
	vector<Point> mc(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mu[i] = moments(contours[i], false);
		mc[i] = Point(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	cout << "[80%] Calculated Centroids" << endl;

	/*Analyze color in centroid*/
	Mat gray;
	cvtColor(src, gray, COLOR_RGB2GRAY);
	for (int i = 0; i < filtered_contours.size(); i++) {
		Point center = mc[i];
		
		if ((int)gray.at<uchar>(center.y, center.x) < range ){
			cout << "Point("<< center.y << ", " << center.x << ") -> " <<(int)gray.at<uchar>(center.y, center.x) << endl;
			col_counter++;
			final_contours.push_back(contours[i]);
			final_hierarchy.push_back(hierarchy[i]);
		}
	}

	contours = final_contours;
	hierarchy = final_hierarchy;

	cout << "[85%] Filtered Colonies" << endl;

	/* FIN ANALISIS COLOR*/

	//Drawing Areas
	cout << "[95%] Drawing image..." << endl;
	Mat drawing = src.clone();
	Mat temp_gray = src.clone();
	cvtColor(src, temp_gray, COLOR_RGB2GRAY);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0,255,0);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		drawContours(temp_gray, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	cout << "[100%] Drawed Contours" << endl;
	return drawing;
}

int main() {
	/*Read Images*/
	Mat src0 = imread("DSC_0112.JPG");
	//prepImage(Map src, bool filter, int minArea, int threshold)
	Mat src = prepareImage(src0, true, 1000, 10000, 150, 162);
	show(resizeImage(src, .35), "Colonies");
	cout << "Number of Colonies found: " << col_counter << endl;

	waitKey(0);
	return 0;
}