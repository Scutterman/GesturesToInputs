#pragma once
#include <string>
#include <opencv2\core\types.hpp>

namespace GesturesToInputs {
    class ControlWindow {
    public:
        int lowHue, lowSaturation, lowValue,
            highHue, highSaturation, highValue;

        ControlWindow(std::string name, int lowHue = 0, int highHue = 179, int lowSaturation = 0, int highSaturation = 255, int lowValue = 0, int highValue = 255);

        cv::Scalar lowScalar();

        cv::Scalar highScalar();
    };
}