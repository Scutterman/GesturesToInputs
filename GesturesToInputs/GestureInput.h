#pragma once
#include <list>
#include "GestureRule.h"

namespace GesturesToInputs {
    class GestureInput
    {
    private:
        std::list<GestureRule> rules;
        int scanCode;
        std::string debugMessage;
    public:
        bool keyPressed = false;
        GestureInput(std::list<GestureRule> rules, int keyScanCode, std::string debugMessageIfPressed = "");
        std::string getDebugMessage();
    };
}
