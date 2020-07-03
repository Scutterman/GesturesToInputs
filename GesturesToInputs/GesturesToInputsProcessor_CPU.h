#pragma once
#include "GesturesToInputsProcessor.h"
#include <opencv2\highgui.hpp>
#include "Webcam.h"

namespace GesturesToInputs {
    class GesturesToInputsProcessor_CPU : public GesturesToInputsProcessor
    {
    private:
        Webcam cam;
        std::map<std::string, Tracker*> trackers;
    public:
        void run();
        GesturesToInputsProcessor_CPU(
            std::map<std::string, Tracker*> trackers,
            std::vector<GestureInput>* gestures
        ) : GesturesToInputsProcessor{ gestures }, trackers{ trackers } {}
    };
}
