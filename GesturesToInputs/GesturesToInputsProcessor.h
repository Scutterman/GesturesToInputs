#pragma once
#include <vector>
#include "GestureDetection.h"
#include "PerformanceTimer.h"
#include "Tracker.h"

namespace GesturesToInputs {
    class GesturesToInputsProcessor {
    protected:
        GestureDetection* gestureDetection = new GestureDetection();
        PerformanceTimer perf;
        std::map<std::string, Tracker*> trackers;
    public:
        const int ERROR_NO_WEBCAM = 200;
        int webcamIndex;
        bool webcamMirrored = true;
        GesturesToInputsProcessor(std::map<std::string, Tracker*> trackers, std::vector<GestureInput>* gestures);
        virtual void run() = 0;
    };
}