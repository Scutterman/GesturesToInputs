#pragma once
#include <opencv2\core\types.hpp>
#include "ColourValues.h"

namespace GesturesToInputs {
    class TrackerValues
    {
    public:
        ColourValues low;
        ColourValues high;
        TrackerValues(int lowHue, int highHue, int lowSaturation, int highSaturation, int lowValue, int highValue);
    };
}