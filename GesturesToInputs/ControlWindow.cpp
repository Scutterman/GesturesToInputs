#include <opencv2/highgui.hpp>
#include "ControlWindow.h"

namespace GesturesToInputs {
    ControlWindow::ControlWindow(std::string name, int lowHue, int highHue, int lowSaturation, int highSaturation, int lowValue, int highValue) {
        this->lowHue = lowHue;
        this->highHue = highHue;
        this->lowSaturation = lowSaturation;
        this->highSaturation = highSaturation;
        this->lowValue = lowValue;
        this->highValue = highValue;

        cv::namedWindow(name, cv::WINDOW_AUTOSIZE);

        cv::createTrackbar("Low Hue", name, &this->lowHue, 179);
        cv::createTrackbar("High Hue", name, &this->highHue, 179);

        cv::createTrackbar("Low Saturation", name, &this->lowSaturation, 255);
        cv::createTrackbar("High Saturation", name, &this->highSaturation, 255);

        cv::createTrackbar("Low Value", name, &this->lowValue, 255);
        cv::createTrackbar("High Value", name, &this->highValue, 255);
    }
    
    cv::Scalar ControlWindow::lowScalar() {
        return cv::Scalar(lowHue, lowSaturation, lowValue);
    }
    
    cv::Scalar ControlWindow::highScalar() {
        return cv::Scalar(highHue, highSaturation, highValue);
    }
}