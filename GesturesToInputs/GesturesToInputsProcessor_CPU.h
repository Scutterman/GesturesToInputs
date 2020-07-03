#pragma once
#include "GesturesToInputs.h"
#include <opencv2\highgui.hpp>

namespace GesturesToInputs {
    class GesturesToInputsProcessor_CPU : public GesturesToInputsProcessor
    {
    private:
        Webcam cam;
    public:
        void run();
        GesturesToInputsProcessor_CPU(
            std::map<std::string, Tracker*> trackers,
            std::vector<GestureInput>* gestures
        ) : GesturesToInputsProcessor{ trackers, gestures } {}
    };
}
