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
        void reset();
        void handleInput(GestureInput* input, bool gestureDetected);
        void complete();
    private:
        ContinuousMouseMovement mouseMovement;
        std::list<GestureInput> gestures;
        cv::Mat text = cv::Mat(500, 500, CV_8UC1);
        int textLine = 40;
        INPUT ip;

        int x = 0, y = 0;

        void startMouseThread();
        void log(std::string textToAdd);
        void sendInput(int dikKeyCode);
        void cancelInput(int dikKeyCode);
        void sendMouseButton(int button);
        void cancelMouseButton(int button);
    };
}
