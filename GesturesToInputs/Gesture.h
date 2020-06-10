#pragma once
#include <windows.h>
#include <dinput.h>
#include <map>

#include <opencv2/core/types.hpp>

#include "Tracker.h"

namespace GesturesToInputs {
    class Gesture {
    public:
        Gesture();

        void calculateInstructionsWithUnknownOrder(std::map<std::string, Tracker> trackers);
    private:
        cv::Mat text = cv::Mat(100, 500, CV_8UC1);
        int textLine = 40;
        INPUT ip;
        std::map<int, bool> pressed = {
            { DIK_W, false },
            { DIK_A, false },
            { DIK_S, false },
            { DIK_D, false },
            { DIK_F, false },
            { DIK_SPACE, false },
        };

        void log(std::string textToAdd);

        void _SendInput(int dikKeyCode);

        void CancelInput(int dikKeyCode);

        void calculateInstructions(Tracker left, Tracker right);
    };
}