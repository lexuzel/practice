
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat src, dst;
std::string win_name{ "window" };

int th_type = 0;
int th = 0;

void threshold_func(int, void*) {
	if(th_type == 5) cv::threshold(src, dst, th, 255, cv::THRESH_OTSU);
	else cv::threshold(src, dst, th, 255, th_type);
	cv::imshow(win_name, dst);
}

int main(){
	src = cv::imread("..\\images\\Lenna.png", cv::IMREAD_GRAYSCALE);
	cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
	cv::imshow(win_name, src);
	cv::createTrackbar("Type",
		win_name, &th_type, 5, threshold_func);
	cv::createTrackbar("Threshold",
		win_name, &th, 255, threshold_func);

	cv::Mat adapt_th, otsu_th;
	cv::namedWindow("Adaptive", cv::WINDOW_AUTOSIZE);

	int kernel = 0;
	cv::createTrackbar("Kernel", "Adaptive", &kernel, 6);
	while (cv::waitKey(30) != 27) {
		cv::adaptiveThreshold(src, adapt_th, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, kernel * 2 + 3, 0);
		cv::imshow("Adaptive", adapt_th);
	}

	cv::waitKey();
    cv::destroyAllWindows();
}

