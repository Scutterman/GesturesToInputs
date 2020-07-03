#include "GesturesToInputs.h"

namespace GesturesToInputs {
    GesturesToInputsProcessor::GesturesToInputsProcessor(std::map<std::string, Tracker*> trackers, std::vector<GestureInput>* gestures)
    {
        this->trackers = trackers;
        gestureDetection->setGestures(gestures);
    }
}
