#ifndef CPPPROJECT_MAIN_H
#define CPPPROJECT_MAIN_H

#include <opencv2/opencv.hpp>
#include <QtWidgets>

void displayHistogram(const cv::Mat& image);
void resizeImage(int width, int height);
void resizeImageCustom();
void loadImage(cv::Mat& image, QLabel* imageLabel);
void rotateImage(double angle);
void adjustBrightnessAndContrast(double alpha, int beta);
void saveImage(const cv::Mat& image, const std::string& outputPath);
void convertToGrayscale(const cv::Mat& inputImage, cv::Mat& outputImage);
void adjustSaturation(cv::Mat& image, double saturationFactor);

#endif //CPPPROJECT_MAIN_H
