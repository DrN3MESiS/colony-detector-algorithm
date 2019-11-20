#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

using namespace cv;
using namespace std;

int col_counter = 0;

void show(Mat src, string name) {	
	namedWindow(name);
	imshow(name, src);
}

Mat ApplyThreshold(Mat src) {
	Mat dst;
	threshold(src, dst, 120, 255, THRESH_BINARY);
	return dst;
}

Mat resizeImage(Mat src, float per) {
	Mat temp_dst;
	resize(src, temp_dst, cv::Size(), per, per);
	return temp_dst;
}

Mat prepareImage(Mat src, bool filter, int filVal, int thresh, float globalRes) {
	Mat temp = src.clone();
	Mat originalResized;

	/*Resize Image*/
	temp = resizeImage(temp, globalRes);
	originalResized = temp;

	/*Convert to Grayscale*/
	cvtColor(temp, temp, COLOR_BGR2GRAY);

	/*Apply Threshold*/
	temp = ApplyThreshold(temp);

	/*Apply Canny*/
	Canny(temp, temp, thresh, thresh *2);

	/*Find Contours*/
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(temp, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	/*Filtering Contours by Area*/
	vector<vector<Point>> filtered_contours;
	vector<Vec4i> filtered_hierarchy;
	for (int i = 0; i < contours.size(); i++) {
		float area = contourArea(contours[i]);
		if (filter) {
			if (area > filVal) {
				filtered_contours.push_back(contours[i]);
				filtered_hierarchy.push_back(hierarchy[i]);
			}
		}
		else {
			filtered_contours.push_back(contours[i]);
			filtered_hierarchy.push_back(hierarchy[i]);
		}
	}

	/*Filter by Compositions*/

	//NOTA PARA GUILLE AQUI EMPIEZA PARTE QUE FALTA
	/*Wey, la idea principal de esta parte que vas a ver, era que de cada contorno analizado,
	vamos a calcular el color predominante que hay dentro, y si hay mas rojo que los demas colores,
	entonces podemos decir que es una colonia, por lo que hacemos un col_counter++ y añadimos el contorno
	y su hierarchy en [i] a final_countours y final_hierarchy respectivamente.
	
	Lo que termine haciendo, porque no supe como hacer lo de arriba, fue calcular el centroide del contorno,
	y fijarme cual era el color predominante del pixel justo en el punto del centroide, y si era mas cercano
	a rojo, hacia lo mismo que arriba si es que cumplia. (El codigo funciona, pero no nos da ninguna colonia correcta)*/

	/*Calculate Contourn Centroid*/
	/*
	vector<vector<Point>> final_contours;
	vector<Vec4i> final_hierarchy;
	vector<Moments> mu(filtered_contours.size());
	vector<Point> mc(filtered_contours.size());
	for (int i = 0; i < filtered_contours.size(); i++) {
		mu[i] = moments(filtered_contours[i], false);
		mc[i] = Point(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	cvtColor(originalResized, originalResized, COLOR_BGR2HSV);
	for (int i = 0; i < filtered_contours.size(); i++) {
		Point center = mc[i];
		unsigned char * p = originalResized.ptr(center.y, center.x); //Obtain color
		uchar r = p[2];
		uchar g = p[1];
		uchar b = p[0];

		if (r > 0 && r < 255 && g < 50 && b < 50) {
			col_counter++;
			final_contours.push_back(filtered_contours[i]);
			final_hierarchy.push_back(filtered_hierarchy[i]);
		}
	}*/

	//HASTA AQUI TERMINA LA PARTE QUE FALTA

	//Drawing Areas
	Mat drawing = resizeImage(src, globalRes);
	for (int i = 0; i < filtered_contours.size(); i++)
	{
		Scalar color = Scalar(0,255,0);
		drawContours(drawing, filtered_contours, i, color, 2, 8, filtered_hierarchy, 0, Point());
	}

	return drawing;
}

int main() {
	/*Read Images*/
	Mat src0 = imread("DSC_0112.JPG");
	//prepImage(Map src, bool filter, int minArea, int cannyThreshold, float resizeVal)
	Mat src = prepareImage(src0, true, 10, 120, 0.40);
	show(resizeImage(src, .70), "Colonies");
	cout << "Number of Colonies found: " << col_counter << endl;

	waitKey(0);
	return 0;
}