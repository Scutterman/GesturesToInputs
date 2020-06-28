#pragma once
#include <windows.h>
#include <dinput.h>
#include <map>

#include <opencv2/core/types.hpp>

#include "Tracker.h"
#include "GestureInput.h"
#include "ContinuousMouseMovement.h"

namespace GesturesToInputs {
    class Gesture {
    public:
        Gesture();
        ~Gesture();
        void setGestures(std::list<GestureInput> gestures);
        void calculateInstructions(std::map<std::string, Tracker*> trackers);
    private:
        ContinuousMouseMovement mouseMovement;
        std::list<GestureInput> gestures;
        cv::Mat text = cv::Mat(500, 500, CV_8UC1);
        int textLine = 40;
        INPUT ip;

        void startMouseThread();
        void log(std::string textToAdd);
        void sendInput(int dikKeyCode);
        void cancelInput(int dikKeyCode);
        void sendMouseButton(int button);
        void cancelMouseButton(int button);
    };
}
