#include "ColourValues.h"

namespace GesturesToInputs {
    ColourValues::ColourValues(int hue = 0, int saturation = 0, int value = 0)
    {
        this->hue = hue;
        this->saturation = saturation;
        this->value = value;
    }
    
    cv::Scalar ColourValues::toScalar()
    {
        return cv::Scalar(hue, saturation, value);
    }
}