
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

typedef  std::pair<cv::Mat, int> DrawerResult;

cv::Mat src;
const int max = 1023;
const int default_value = 511;

int a11 = default_value;
int a12 = default_value;
int a13 = default_value;
int a21 = default_value;
int a22 = default_value;
int a23 = default_value;

inline double map_scale(const int value) {
	return 2.0 * value / max;
}

inline double map_skew(const int value) {
	return  2.0 * value / max - 1;
}

inline double map_shift(const int value, const int target_max) {
	return map_skew(value) * target_max;
}

void draw(int, void*) {
		cv::Mat affine_transform_matrix = (cv::Mat_<double>(2, 3) 
			<< map_scale(a11), map_skew(a12), map_shift(a13, src.cols), 
			   map_skew(a21), map_scale(a22), map_shift(a23, src.rows));

		cv::Mat transformed_image;
		cv::warpAffine(src, transformed_image, affine_transform_matrix, src.size());

		cv::Mat separator(src.rows, 2, CV_8UC3, cv::Scalar(0, 0, 0));

		cv::Mat display_buff;
		cv::hconcat(std::vector<cv::Mat>{ src, separator, transformed_image }, display_buff);
		cv::imshow("window", display_buff);
}

int main(){
	src = cv::imread("..\\images\\Lenna.png", cv::IMREAD_COLOR);
	std::string windowname{ "window" };

	cv::namedWindow(windowname);
	cv::createTrackbar("x11", windowname, &a11, max, draw);
	cv::createTrackbar("x12", windowname, &a12, max, draw);
	cv::createTrackbar("x13", windowname, &a13, max, draw);
	cv::createTrackbar("x21", windowname, &a21, max, draw);
	cv::createTrackbar("x22", windowname, &a22, max, draw);
	cv::createTrackbar("x23", windowname, &a23, max, draw);

	cv::waitKey();
	cv::destroyAllWindows();

}

