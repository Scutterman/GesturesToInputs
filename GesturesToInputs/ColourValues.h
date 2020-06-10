#pragma once
#include <opencv2\core\types.hpp>
namespace GesturesToInputs {
    class ColourValues {
    public:
        int hue;
        int saturation;
        int value;

        ColourValues(int hue = 0, int saturation = 0, int value = 0);
        cv::Scalar toScalar();
    };
}