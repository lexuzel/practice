
#include "pch.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <numeric>
#include <iostream>
#include <map>

void equalizeHistColored(const cv::Mat& src, cv::Mat& dst) {
	std::vector<cv::Mat> planes(src.channels());
	cv::split(src, planes);
	for (int i = 0; i < src.channels(); ++i) {
		cv::equalizeHist(planes[i], planes[i]);
	}
	cv::merge(planes, dst);
}

void calcHistColored(const cv::Mat& src, cv::Mat& dst, const int histSize = 256, const std::vector<float>& ranges = std::vector<float>{ 0, 256 }) {
	std::vector<cv::Mat> planes(src.channels());
	cv::split(src, planes);
	const float * r = ranges.data();
	std::vector<cv::Mat> hist(src.channels());
	for (int i = 0; i < src.channels(); ++i) {
		cv::calcHist(&planes[i], 1, 0, cv::Mat(), // do not use mask
			hist[i], 1, &histSize, &r, // 1 channel
			true, false); // the histogram is uniform
	}
	cv::merge(hist, dst);
}

void drawHist(cv::Mat& image, const cv::Mat& hist, const std::vector<cv::Scalar>& colors) {
	double maxVal;
	cv::minMaxLoc(hist, nullptr, &maxVal);
	const cv::Point2f step(1.0 * image.cols / hist.rows, image.rows / maxVal);

	const auto buildHistSegmentPoint = [&image, step](int i, float value) {
		return cv::Point2f(step.x * i, image.rows - step.y * value);
	};

	image = 0;
	auto values = hist.ptr<float>(1);
	for (int i = 1; i < hist.rows; ++i) {
		for (int j = 0; j < hist.channels(); ++j) {
			cv::line(image, buildHistSegmentPoint(i - 1, values[(i - 1) * hist.channels() + j]), buildHistSegmentPoint(i, values[i * hist.channels() + j]), colors[j]);
		}
	}
}

void findHistogram(const cv::Mat image, cv::Mat& hist_plot) {
	cv::MatND hist;
	calcHistColored(image, hist);
	drawHist(hist_plot, hist, image.channels() == 1 ? std::vector<cv::Scalar>{ cv::Scalar(127, 127, 127) } : std::vector<cv::Scalar>{ cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255) });
}

std::map<int, std::string> hist_comp_method_names{
	{ 0, "HISTCMP_CORREL" },
	{ 1, "HISTCMP_CHISQR" },
	{ 2, "HISTCMP_CHISQR" },
	{ 3, "HISTCMP_INTERSECT" },
	{ 4, "HISTCMP_HELLINGER" },
	{ 5, "HISTCMP_CHISQR_ALT" },
	{ 6, "HISTCMP_KL_DIV" }
};

double compareHistColored(const cv::Mat& hist1, const cv::Mat& hist2, int method) {
	if (hist1.channels() != hist2.channels())
		throw std::exception("Histogram channels size mismatch");

	std::vector<cv::Mat> planes1(hist1.channels());
	std::vector<cv::Mat> planes2(hist2.channels());
	cv::split(hist1, planes1);
	cv::split(hist2, planes2);

	std::vector<double> scores(hist1.channels());
	std::transform(planes1.begin(), planes1.end(), planes2.begin(), scores.begin(), [method](const cv::Mat& plane1, const cv::Mat& plane2) {
		return cv::compareHist(plane1, plane2, method);
	});
	return std::accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
}

void compareHistograms(const cv::Mat& image1, const cv::Mat& image2) {
	cv::MatND hist1, hist2;
	calcHistColored(image1, hist1);
	calcHistColored(image2, hist2);

	for (auto method : std::vector<int>{ cv::HISTCMP_CORREL, cv::HISTCMP_CHISQR, cv::HISTCMP_CHISQR, cv::HISTCMP_INTERSECT, cv::HISTCMP_HELLINGER, cv::HISTCMP_CHISQR_ALT, cv::HISTCMP_KL_DIV }) {
		std::cout << hist_comp_method_names[method] << ": " << compareHistColored(hist1, hist2, method) << std::endl;
	}
}

int main(int argc, const char** argv) {
	cv::Mat image = cv::imread("..\\images\\Lenna.png", cv::IMREAD_COLOR);
	cv::resize(image, image, cv::Size(640, 640));

	cv::Mat image_eq;
	equalizeHistColored(image, image_eq);

	cv::Mat hist_plot(cv::Size(640, 360), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat hist_eq_plot(cv::Size(640, 360), CV_8UC3, cv::Scalar(0, 0, 0));

	findHistogram(image, hist_plot);
	findHistogram(image_eq, hist_eq_plot);

	cv::Mat images;
	cv::hconcat(std::vector<cv::Mat>{ image, image_eq}, images);
	cv::Mat histograms;
	cv::hconcat(std::vector<cv::Mat>{ hist_plot, hist_eq_plot}, histograms);
	cv::Mat images_and_histograms;
	cv::vconcat(std::vector<cv::Mat>{ images, histograms }, images_and_histograms);
	cv::resize(images_and_histograms, images_and_histograms, cv::Size(), 0.7, 0.7);
	cv::imshow("Result", images_and_histograms);

	compareHistograms(image, image_eq);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
