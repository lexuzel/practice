
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>


const int MAX_KERNEL_LENGTH = 31;
int kernel_length = 0;
int blur_type = 0;

cv::Mat src, dst;
std::string win_name{ "window" };

void display_dst(int, void*) {
	int kernel_size = kernel_length * 2 + 1;
	switch (blur_type){
	case 0:
		blur(src, dst, cv::Size(kernel_size, kernel_size), cv::Point(-1, -1));
		break;
	case 1:
		GaussianBlur(src, dst, cv::Size(kernel_size, kernel_size), 0, 0);
		break;
	case 2:
		medianBlur(src, dst, kernel_size);
		break;
	case 3:
		bilateralFilter(src, dst, kernel_size, kernel_size, kernel_size);
	}
	cv::imshow(win_name, dst);
}

int main(){
	std::string image_path{ "..\\images\\Lenna.png" };
	src = cv::imread(image_path, cv::IMREAD_COLOR);

	cv::namedWindow(win_name, cv::WINDOW_FREERATIO);
	cv::createTrackbar("Kernel", win_name,
		&kernel_length, MAX_KERNEL_LENGTH / 2,
		display_dst);
	cv::createTrackbar("Blur", win_name,
		&blur_type, 3,
		display_dst);
	cv::imshow(win_name, src);

	cv::waitKey();
	cv::destroyWindow(win_name);
}

