#include "ImageFilters.h"

void applyBlur(cv::Mat& src, cv::Mat& dst, int size) {
    cv::blur(src, dst, cv::Size(size, size));
}

void applyMedianFilter(cv::Mat& src, cv::Mat& dst, int size) {
    cv::medianBlur(src, dst, size);
}

void applyGaussianFilter(cv::Mat& src, cv::Mat& dst, int size) {
    cv::GaussianBlur(src, dst, cv::Size(size, size), 0);
}

void applyLaplacianEdgeDetection(cv::Mat& src, cv::Mat& dst, int ddepth, int kernel_size) {
    cv::Laplacian(src, dst, ddepth, kernel_size);
}
