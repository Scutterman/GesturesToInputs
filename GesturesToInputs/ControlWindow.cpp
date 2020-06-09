#include "ControlWindow.h"
#include <opencv2\highgui.hpp>

using namespace cv;

namespace GesturesToInputs {
    ControlWindow::ControlWindow(std::string name, int lowHue, int highHue, int lowSaturation, int highSaturation, int lowValue, int highValue) {
        this->lowHue = lowHue;
        this->highHue = highHue;
        this->lowSaturation = lowSaturation;
        this->highSaturation = highSaturation;
        this->lowValue = lowValue;
        this->highValue = highValue;

        namedWindow(name, WINDOW_AUTOSIZE);

        createTrackbar("Low Hue", name, &this->lowHue, 179);
        createTrackbar("High Hue", name, &this->highHue, 179);

        createTrackbar("Low Saturation", name, &this->lowSaturation, 255);
        createTrackbar("High Saturation", name, &this->highSaturation, 255);

        createTrackbar("Low Value", name, &this->lowValue, 255);
        createTrackbar("High Value", name, &this->highValue, 255);
    }
    Scalar ControlWindow::lowScalar() {
        return Scalar(lowHue, lowSaturation, lowValue);
    }
    Scalar ControlWindow::highScalar() {
        return Scalar(highHue, highSaturation, highValue);
    }
}