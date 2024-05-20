#include <opencv2/opencv.hpp>
#include <QtWidgets>

cv::Mat currentImage, originalImage;

void resizeImage(int width, int height) {
    cv::resize(currentImage, currentImage, cv::Size(width, height));
}

void rotateImage(double angle) {
    cv::Point2f center((currentImage.cols-1)/2.0, (currentImage.rows-1)/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(currentImage, currentImage, rot, currentImage.size());
}

void adjustBrightnessAndContrast(double alpha, int beta) {
    currentImage.convertTo(currentImage, -1, alpha, beta);
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
    originalImage = currentImage.clone();


    QWidget window;
    window.setWindowTitle("Image Editor");

    QVBoxLayout *layout = new QVBoxLayout(&window);
    QLabel *imageLabel = new QLabel;
    QImage qimg(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(800, 600, Qt::KeepAspectRatio));

    QPushButton *resizeButton = new QPushButton("Resize to 640x480");
    QPushButton *rotateButton = new QPushButton("Rotate 90 Degrees");
    QPushButton *adjustButton = new QPushButton("Increase Brightness/Contrast");
    QPushButton *resetButton = new QPushButton("Reset Brightness/Contrast");

    layout->addWidget(imageLabel);
    layout->addWidget(resizeButton);
    layout->addWidget(rotateButton);
    layout->addWidget(adjustButton);
    layout->addWidget(resetButton);

    QObject::connect(resizeButton, &QPushButton::clicked, [&]() {
        if (currentImage.cols != 640 || currentImage.rows != 480) {
            resizeImage(640, 480);
        } else {
            currentImage = originalImage.clone();
        }
        qimg = QImage(currentImage.data, currentImage.cols, currentImage.rows, currentImage.step, QImage::Format_RGB888);
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
    });

    window.show();
    return app.exec();
}
