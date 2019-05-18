
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat src, dst;
std::string win_name{ "window" };
int morph_op = 0;
int kernel_length = 0;

void get_morph_op(int, void*) {
	auto element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * kernel_length + 1, 2 * kernel_length + 1),
		cv::Point(kernel_length, kernel_length));
	switch (morph_op) {
	case 0:
		cv::erode(src, dst, element);
		break;
	case 1:
		cv::dilate(src, dst, element);
		break;
	default:
		cv::morphologyEx(src, dst, morph_op, element);
	}
	cv::imshow(win_name, dst);
}

int main(){
	src = cv::imread("..\\images\\Lenna.png", cv::IMREAD_COLOR);
	cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
	cv::imshow(win_name, src);
	cv::createTrackbar("Morph op:", win_name,
		&morph_op, 6, get_morph_op);
	cv::createTrackbar("Kernel:", win_name,
		&kernel_length, 31, get_morph_op);

	cv::waitKey();
	cv::destroyWindow(win_name);
}

