#ifndef CPPPROJECT_IMAGEFILTERS_H
#define CPPPROJECT_IMAGEFILTERS_H

#pragma once
#include <opencv2/opencv.hpp>

void applyBlur(cv::Mat& src, cv::Mat& dst, int size);
void applyMedianFilter(cv::Mat& src, cv::Mat& dst, int size);
void applyGaussianFilter(cv::Mat& src, cv::Mat& dst, int size);
void applyLaplacianEdgeDetection(cv::Mat& src, cv::Mat& dst, int ddepth, int kernel_size);

#endif //CPPPROJECT_IMAGEFILTERS_H
