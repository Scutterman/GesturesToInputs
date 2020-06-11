#pragma once
#include "Gesture.h"
#include "PerformanceTimer.h"
#include "Tracker.h"
#include "Webcam.h"

namespace GesturesToInputs {
    class GesturesToInputsProcessor {
    private:
        Webcam cam;
        Gesture gesture;
        PerformanceTimer perf;
        std::map<std::string, Tracker> trackers;
    public:
        const int ERROR_NO_WEBCAM = 200;
        int webcamIndex;
        bool webcamMirrored = true;
        GesturesToInputsProcessor(std::map<std::string, Tracker> trackers, std::list<GestureInput> gestures);
        void run();
    };
}