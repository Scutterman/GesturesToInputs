#pragma once
#include <vector>
#include "GestureDetection.h"
#include "PerformanceTimer.h"

namespace GesturesToInputs {
    class GesturesToInputsProcessor {
    protected:
        GestureDetection* gestureDetection = new GestureDetection();
        PerformanceTimer perf;
    public:
        const int ERROR_NO_WEBCAM = 200;
        int webcamIndex = 0;
        bool webcamMirrored = true;
        GesturesToInputsProcessor(std::vector<GestureInput>* gestures);
        virtual void run() = 0;
    };
}