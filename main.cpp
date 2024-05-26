#include <opencv2/opencv.hpp>
#include <QtWidgets>
#include "ImageFilters.h"
#include "FaceDetection.h"
#include "ColorAnalysis.h"

cv::Mat currentImage, originalImage;
bool isGray = false;

void displayHistogram(const cv::Mat& image) {
    std::vector<cv::Mat> bgr_planes;
    cv::split(image, bgr_planes);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};

    cv::Mat b_hist, g_hist, r_hist;

    cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange);
    cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange);
    cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < histSize; i++) {
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
                 cv::Scalar(255, 0, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
                 cv::Scalar(0, 255, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
                 cv::Scalar(0, 0, 255), 2, 8, 0);
    }

    cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
    cv::imshow("Histogram", histImage);
}

void resizeImageCustom() {
    bool ok;
    int width = QInputDialog::getInt(nullptr, "Resize Image", "Width:", 800, 1, 5000, 1, &ok);
    if (!ok) return;
    int height = QInputDialog::getInt(nullptr, "Resize Image", "Height:", 600, 1, 5000, 1, &ok);
    if (!ok) return;

    cv::resize(currentImage, currentImage, cv::Size(width, height));
}


void resizeImage(int width = 800, int height = 600) {
    cv::resize(currentImage, currentImage, cv::Size(width, height));
}

void loadImage(cv::Mat& image, QLabel* imageLabel) {
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open Image", "", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    if (filePath.isEmpty()) return;

    image = cv::imread(filePath.toStdString());

    if (image.empty()) {
        QMessageBox::warning(nullptr, "Load Image", "The selected image could not be loaded.");
        return;
    }

    resizeImage(800, 600);

    QImage qimg(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void rotateImage(double angle) {
    cv::Point2f center((currentImage.cols-1)/2.0, (currentImage.rows-1)/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(currentImage, currentImage, rot, currentImage.size());
}

void adjustBrightnessAndContrast(double alpha, int beta) {
    currentImage.convertTo(currentImage, -1, alpha, beta);
}

void saveImage(const cv::Mat& image, const std::string& outputPath) {
    if (!cv::imwrite(outputPath, image)) {
        std::cerr << "Nie udało się zapisać obrazu na ścieżce: " << outputPath << std::endl;
    }
}

void convertToGrayscale(const cv::Mat& inputImage, cv::Mat& outputImage) {
    cv::cvtColor(inputImage, outputImage, cv::COLOR_BGR2GRAY);
}

void adjustSaturation(cv::Mat& image, double saturationFactor) {
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels(3);
    cv::split(hsvImage, channels);

    channels[1] = channels[1] * saturationFactor;
    cv::merge(channels, hsvImage);

    cv::cvtColor(hsvImage, image, cv::COLOR_HSV2BGR);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString imagePath = "/Users/jakubsadkiewicz/CLionProjects/CppProject/miciu.jpeg";
    if (imagePath.isEmpty()) {
        QMessageBox::critical(nullptr, "Error", "No image selected!");
        return -1;
    }

    currentImage = cv::imread(imagePath.toStdString());
    if (currentImage.empty()) {
        QMessageBox::critical(nullptr, "Error", "Could not load the image!");
        return -1;
    }
    resizeImage(800, 600);
    originalImage = currentImage.clone();

    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    QWidget window;
    window.setWindowTitle("Image Editor");

    QGridLayout *layout = new QGridLayout(&window);
    QLabel *imageLabel = new QLabel;
    QImage qimg(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));

    layout->addWidget(imageLabel, 0, 0, 1, 2);

    QPushButton *resizeButton = new QPushButton("Resize to custom resolution");
    resizeButton->setToolTip("Click to resize the image to a custom size.");
    QPushButton *rotateButton = new QPushButton("Rotate 90 Degrees");
    resizeButton->setToolTip("Click to rotate the image by 90 degrees.");
    QPushButton *adjustButton = new QPushButton("Increase Brightness/Contrast");
    adjustButton->setToolTip("Click to increase the brightness and contrast of the image.");
    QPushButton *resetButton = new QPushButton("Reset Filters");
    resetButton->setToolTip("Click to reset all filters and adjustments.");

    QPushButton *blurButton = new QPushButton("Apply Blur");
    blurButton->setToolTip("Click to apply a blur filter to the image.");
    QPushButton *medianButton = new QPushButton("Apply Median Filter");
    medianButton->setToolTip("Click to apply a median filter to the image.");
    QPushButton *gaussianButton = new QPushButton("Apply Gaussian Filter");
    gaussianButton->setToolTip("Click to apply a Gaussian filter to the image.");
    QPushButton *laplacianButton = new QPushButton("Apply Laplacian Filter");
    laplacianButton->setToolTip("Click to apply a Laplacian filter to the image.");

    QPushButton *detectFacesButton = new QPushButton("Detect Faces");
    detectFacesButton->setToolTip("Click to detect faces in the image.");
    QPushButton *findDominantColorsButton = new QPushButton("Find Dominant Colors");
    findDominantColorsButton->setToolTip("Click to find the dominant colors in the image.");

    QPushButton *saveButton = new QPushButton("Save Image");
    saveButton->setToolTip("Click to save the image to a file.");
    QPushButton *convertToGrayscaleButton = new QPushButton("Convert to Grayscale");
    convertToGrayscaleButton->setToolTip("Click to convert the image to grayscale.");

    QPushButton *adjustSaturationButton = new QPushButton("Adjust Saturation");
    adjustSaturationButton->setToolTip("Click to adjust the saturation of the image.");
    QPushButton *loadButton = new QPushButton("Load Image");
    loadButton->setToolTip("Click to load a new image.");

    QPushButton *histogramButton = new QPushButton("Display Histogram");
    histogramButton->setToolTip("Click to display the histogram of the image.");

    layout->addWidget(resizeButton, 1, 0);
    layout->addWidget(rotateButton, 1, 1);
    layout->addWidget(adjustButton, 2, 0);
    layout->addWidget(resetButton, 2, 1);

    layout->addWidget(blurButton, 3, 0);
    layout->addWidget(medianButton, 3, 1);
    layout->addWidget(gaussianButton, 4, 0);
    layout->addWidget(laplacianButton, 4, 1);

    layout->addWidget(detectFacesButton, 5, 0);
    layout->addWidget(findDominantColorsButton, 5, 1);

    layout->addWidget(adjustSaturationButton, 6, 0);
    layout->addWidget(convertToGrayscaleButton, 6, 1);

    layout->addWidget(saveButton, 7, 0);
    layout->addWidget(loadButton, 7, 1);

    layout->addWidget(histogramButton, 8, 0);


    QObject::connect(resizeButton, &QPushButton::clicked, [&]() {
        resizeImageCustom();
        QImage qimg(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
    });

    QObject::connect(rotateButton, &QPushButton::clicked, [&]() {
        rotateImage(90);
        qimg = QImage(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
    });

    QObject::connect(adjustButton, &QPushButton::clicked, [&]() {
        adjustBrightnessAndContrast(1.1, 10);
        qimg = QImage(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
    });

    QObject::connect(resetButton, &QPushButton::clicked, [&]() {
        currentImage = originalImage.clone();
        if (currentImage.cols != 800 || currentImage.rows != 600) {
            cv::resize(currentImage, currentImage, cv::Size(800, 600));
        }
        qimg = QImage(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
        isGray = false;
    });

    QObject::connect(blurButton, &QPushButton::clicked, [&]() {
        cv::Mat dst;
        applyBlur(currentImage, dst, 5);
        currentImage = dst;
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));
    });

    QObject::connect(medianButton, &QPushButton::clicked, [&]() {
        cv::Mat dst;
        applyMedianFilter(currentImage, dst, 5);
        currentImage = dst;
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));
    });

    QObject::connect(gaussianButton, &QPushButton::clicked, [&]() {
        cv::Mat dst;
        applyGaussianFilter(currentImage, dst, 5);
        currentImage = dst;
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));
    });

    QObject::connect(laplacianButton, &QPushButton::clicked, [&]() {
        cv::Mat dst;
        applyLaplacianEdgeDetection(currentImage, dst, CV_16S, 3);
        currentImage = dst;
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));
    });

    QObject::connect(detectFacesButton, &QPushButton::clicked, [&]() {
        detectFaces(currentImage);
        QImage qimg(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));
    });

    QObject::connect(findDominantColorsButton, &QPushButton::clicked, [&]() {
        std::vector<cv::Vec3b> dominantColors;
        findDominantColors(currentImage, 3, dominantColors);
        cv::Mat dst = cv::Mat::zeros(100, 100, CV_8UC3);
        for (int i = 0; i < dominantColors.size(); i++) {
            cv::rectangle(dst, cv::Point(i * 100 / dominantColors.size(), 0), cv::Point((i + 1) * 100 / dominantColors.size(), 100), cv::Scalar(dominantColors[i]), -1);
        }
        currentImage = dst;
        QImage qimg(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));
    });

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        QString filter = "Image Files (*.png *.jpg *.jpeg *.bmp)";
        QString savePath = QFileDialog::getSaveFileName(nullptr, "Save Image", "/Users/jakubsadkiewicz/CLionProjects/CppProject/", filter);
        if (!savePath.isEmpty()) {
            saveImage(currentImage, savePath.toStdString());
        }
    });

    QObject::connect(convertToGrayscaleButton, &QPushButton::clicked, [&]() {
        if (!isGray) {
            cv::Mat dst;
            convertToGrayscale(currentImage, dst);
            currentImage = dst;
            QImage qimg(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Grayscale8);
            imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(800, 600, Qt::KeepAspectRatio));
            isGray = true;
        }
    });

    QObject::connect(adjustSaturationButton, &QPushButton::clicked, [&]() {
        adjustSaturation(currentImage, 1.5);
        qimg = QImage(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
    });

    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
        loadImage(currentImage, imageLabel);
        originalImage = currentImage.clone();
        resizeImage(800, 600);
    });

    QObject::connect(histogramButton, &QPushButton::clicked, [&]() {
        displayHistogram(currentImage);
    });

    window.show();
    return app.exec();
}
