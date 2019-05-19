
#include "pch.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

cv::Mat src, dst1, dst2, dst;
std::string win_name{ "window" };
int kernel{ 0 };

cv::Mat alpha_blend;

void derivative(int, void*) {
	cv::Sobel(src, dst1, -1, 1, 0, kernel * 2 + 3);
	cv::Sobel(src, dst2, -1, 0, 1, kernel * 2 + 3);
	cv::hconcat(std::vector<cv::Mat>{ dst1, dst2 }, dst);
	cv::imshow(win_name, dst);

	cv::addWeighted(dst1, 0.5, dst2, 0.5, 0, alpha_blend);
	cv::normalize(alpha_blend, alpha_blend, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	cv::namedWindow("Alpha Blending", cv::WINDOW_AUTOSIZE);
	cv::imshow("Alpha Blending", alpha_blend);
}

cv::Mat detected_edges, dst_canny;
int lowThreshold = 0;
void CannyThreshold(int, void*){
	cv::blur(src, detected_edges, cv::Size(3, 3));
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*3, 3);
	dst_canny = cv::Scalar::all(0);
	src.copyTo(dst_canny, detected_edges);
	imshow("Canny", dst_canny);
}

int main(){

	src = cv::imread("..\\images\\Lenna.png", cv::IMREAD_GRAYSCALE);

	cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
	cv::imshow(win_name, src);
	cv::createTrackbar("Kernel",
		win_name, &kernel, 10, derivative);

	cv::namedWindow("Alpha Blending", cv::WINDOW_AUTOSIZE);

	cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Min Threshold:", "Canny", &lowThreshold, 80, CannyThreshold);
	CannyThreshold(0, 0);

	cv::waitKey();
	cv::destroyAllWindows();
}

