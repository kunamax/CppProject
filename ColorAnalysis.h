#ifndef CPPPROJECT_COLORANALYSIS_H
#define CPPPROJECT_COLORANALYSIS_H

#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

void findDominantColors(const cv::Mat& image, int k, std::vector<cv::Vec3b>& dominantColors);

#endif //CPPPROJECT_COLORANALYSIS_H
