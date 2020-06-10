#include "ColourValues.h"

namespace GesturesToInputs {
    ColourValues::ColourValues(int hue, int saturation, int value)
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