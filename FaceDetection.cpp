#include "FaceDetection.h"

void detectFaces(cv::Mat& image) {
    std::vector<cv::Rect> faces;
    cv::CascadeClassifier face_cascade;

    face_cascade.load("/Users/jakubsadkiewicz/CLionProjects/CppProject/haarcascade_frontalface_default.xml");

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    face_cascade.detectMultiScale(gray, faces, 1.001, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    for (const auto& face : faces) {
        cv::rectangle(image, face, cv::Scalar(255, 0, 0), 2);
    }
}