#include "GesturesToInputsProcessor.h"

namespace GesturesToInputs {
    GesturesToInputsProcessor::GesturesToInputsProcessor(std::vector<GestureInput>* gestures)
    {
        gestureDetection->setGestures(gestures);
    }
}
