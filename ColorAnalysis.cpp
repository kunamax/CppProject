#include "ColorAnalysis.h"

void findDominantColors(const cv::Mat& image, int k, std::vector<cv::Vec3b>& dominantColors) {
    cv::Mat data;
    image.convertTo(data, CV_32F);
    data = data.reshape(1, data.total());

    cv::Mat labels, centers;
    cv::kmeans(data, k, labels, cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

    centers = centers.reshape(3, centers.rows);
    dominantColors.clear();

    for (int i = 0; i < centers.rows; i++) {
        dominantColors.push_back(centers.at<cv::Vec3f>(i, 0));
    }
}