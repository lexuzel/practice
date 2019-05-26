
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
//#include <vector>

cv::Mat preprocessImage(cv::Mat img){
	cv::Mat thresh;
	cv::threshold(img, thresh, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	cv::Mat opened;
	auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(thresh, opened, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);

	return opened;
}

std::pair<cv::Mat, cv::Mat> distanceTransform(cv::Mat opened){
	cv::Mat sure_bg;
	cv::dilate(opened, sure_bg, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 2);

	cv::Mat dist_transform;
	cv::distanceTransform(opened, dist_transform, cv::DIST_L2, 5);
	
	cv::Mat dist_transform_normalized;
	cv::normalize(dist_transform, dist_transform_normalized, 0, 255, cv::NORM_MINMAX, CV_8UC3);

	cv::Mat sure_fg;
	double max;
	cv::minMaxLoc(dist_transform, nullptr, &max);
	cv::threshold(dist_transform, sure_fg, max*0.7, 255, CV_THRESH_BINARY);
	sure_fg.convertTo(sure_fg, CV_8UC3);

	return std::make_pair(sure_bg, sure_fg);
}

cv::Mat generateMarkers(cv::Mat bg, cv::Mat fg){
	cv::Mat unknown;
	cv::Mat markers;
	cv::absdiff(bg, fg, unknown);

	cv::connectedComponents(fg, markers);

	markers = markers + 1;

	cv::Mat zeros = cv::Mat::zeros(markers.size(), markers.type());
	zeros.copyTo(markers, unknown);

	return markers;
}

cv::Mat visualize(cv::Mat img, cv::Mat markers){
	markers.convertTo(markers, CV_8UC3);
	cv::threshold(markers, markers, 1, 255, cv::THRESH_BINARY);

	auto changedImg = img.clone();
	changedImg += cv::Scalar(0, 0, 200);
	changedImg.copyTo(img, markers);

	return img;
}

int main(){
	auto img = cv::imread("..\\images\\water_coins.jpg", cv::ImreadModes::IMREAD_GRAYSCALE);
	if (img.empty()) return 1;

	auto opened = preprocessImage(img);
	auto bg_fg = distanceTransform(opened);
	auto sure_fg = bg_fg.first;
	auto sure_bg = bg_fg.second;

	auto markers = generateMarkers(sure_bg, sure_fg);
	cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
	cv::watershed(img, markers);
	markers.convertTo(markers, CV_8UC1);
	cv::normalize(markers, markers, 0, 255, cv::NORM_MINMAX);
	cv::Mat edges;
	cv::Canny(markers, edges, 127, 255);
	cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);

	auto visualized = visualize(img.clone(), markers);

	cv::Mat dst;
	cv::hconcat(visualized, edges, dst);
	cv::imshow("win", dst);

	cv::waitKey();
}

