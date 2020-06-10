#include "TrackerValues.h"

namespace GesturesToInputs {
    TrackerValues::TrackerValues(int lowHue, int highHue, int lowSaturation, int highSaturation, int lowValue, int highValue)
    {
        low = ColourValues(lowHue, lowSaturation, lowValue);
        high = ColourValues(highHue, highSaturation, highValue);
    }
}