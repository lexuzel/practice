
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat src, dst;
std::string win_name{ "window" };

int th_type = 0;
int th = 0;

void threshold_func(int, void*) {
	cv::threshold(src, dst, th, 255, th_type);
	cv::imshow(win_name, dst);
}

int main(){
	src = cv::imread("..\\images\\Lenna.png", cv::IMREAD_GRAYSCALE);
	cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
	cv::imshow(win_name, src);
	cv::createTrackbar("Type",
		win_name, &th_type, 4, threshold_func);
	cv::createTrackbar("Threshold",
		win_name, &th, 255, threshold_func);

	cv::waitKey();
	cv::destroyWindow(win_name);

}

