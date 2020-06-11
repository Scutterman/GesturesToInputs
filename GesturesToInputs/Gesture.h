#pragma once
#include <windows.h>
#include <dinput.h>
#include <map>

#include <opencv2/core/types.hpp>

#include "Tracker.h"
#include "GestureInput.h"

namespace GesturesToInputs {
    class Gesture {
    public:
        Gesture(std::list<GestureInput> gestures = std::list<GestureInput>());
        void calculateInstructions(std::map<std::string, Tracker> trackers);
    private:
        std::list<GestureInput> gestures;
        cv::Mat text = cv::Mat(100, 500, CV_8UC1);
        int textLine = 40;
        INPUT ip;

        void log(std::string textToAdd);
        void sendInput(int dikKeyCode);
        void cancelInput(int dikKeyCode);
    };
}
