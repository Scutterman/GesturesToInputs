#pragma once
#include <windows.h>
#include <map>
#include "Tracker.h"
#include <dinput.h>

namespace GesturesToInputs {
    class Gesture {
    public:
        Gesture();

        void calculateInstructionsWithUnknownOrder(Tracker tracker1, Tracker tracker2);
    private:
        cv::Mat text = cv::Mat(100, 500, CV_8UC1);
        int textLine = 40;
        INPUT ip;
        std::map<int, bool> pressed = {
            { DIK_W, false },
            { DIK_W, false },
            { DIK_W, false },
            { DIK_W, false },
            { DIK_W, false },
            { DIK_W, false },
        };

        void log(std::string textToAdd);

        void _SendInput(int dikKeyCode);

        void CancelInput(int dikKeyCode);

        void calculateInstructions(Tracker left, Tracker right);
    };
}