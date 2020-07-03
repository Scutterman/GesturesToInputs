#pragma once
#include <vector>
#include "GestureDetection.h"
#include "PerformanceTimer.h"
#include "Tracker.h"
#include "Webcam.h"

namespace GesturesToInputs {
    class GesturesToInputsProcessor {
    private:
        Webcam cam;
        GestureDetection* gestureDetection = new GestureDetection();
        PerformanceTimer perf;
        std::map<std::string, Tracker*> trackers;
    public:
        const int ERROR_NO_WEBCAM = 200;
        int webcamIndex;
        bool webcamMirrored = true;
        GesturesToInputsProcessor(std::map<std::string, Tracker*> trackers, std::vector<GestureInput>* gestures);
        void run();
    };
}