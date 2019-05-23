
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(){
	cv::Mat image = cv::imread("..\\images\\Lenna.png", cv::IMREAD_COLOR);
	cv::Mat templ = cv::imread("..\\images\\Lennas_eye.jpg", cv::IMREAD_COLOR);

	if (image.empty() || templ.empty()) return 1;

	cv::Mat result;
	cv::matchTemplate(image, templ, result, cv::TM_SQDIFF_NORMED);
	cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);

	double maxVal;
	cv::Point maxPoint;
	cv::minMaxLoc(result, nullptr, &maxVal, nullptr, &maxPoint);

	const auto getRectangle = [&templ, &maxPoint] {
		return cv::Rect(cv::Point(maxPoint.x - templ.cols / 2, maxPoint.y - templ.rows / 2), 
						cv::Point(maxPoint.x + templ.cols / 2, maxPoint.y + templ.rows / 2));
	};

	cv::rectangle(image, getRectangle(), cv::Scalar(0, 0, 255), 4);
	cv::imshow("win", image);

	cv::waitKey();
}

