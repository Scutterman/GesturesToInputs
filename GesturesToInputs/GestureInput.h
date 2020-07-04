#pragma once
#include <vector>
#include "GestureRule.h"

namespace GesturesToInputs {
    enum class GESTURE_INPUT_TYPE { KEYBOARD, MOUSE_BUTTON, MOUSE_MOVE };
    enum class MOUSE_DIRECTION { UP = -2, RIGHT = 1, LEFT = -1, DOWN = 2 };

    struct GestureRuleData {
        unsigned int gestureIndex = 0;
        unsigned int type = 0;
        int operation = 0;
        unsigned int expectedValue = 0;
        unsigned int compareTwoTrackers = 0;
        unsigned int trackerIndex = 0;
        unsigned int comparisonTrackerIndex = 0;
        unsigned int padding = 0;
    };

    class GestureInput
    {
    private:
        std::vector<GestureRule> rules;
        int value;
        GESTURE_INPUT_TYPE inputType;
        std::string debugMessage;
    public:
        std::vector<GestureRule> getRules();
        int getValue();
        bool active = false;
        GestureInput(std::vector<GestureRule> rules, int keyScanCode, std::string debugMessageIfPressed = "", GESTURE_INPUT_TYPE inputType = GESTURE_INPUT_TYPE::KEYBOARD);
        std::string getDebugMessage();
        GESTURE_INPUT_TYPE getInputType();
    };
}
