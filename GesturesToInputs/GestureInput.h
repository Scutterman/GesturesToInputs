#pragma once
#include <list>
#include "GestureRule.h"

namespace GesturesToInputs {
    class GestureInput
    {
    private:
        std::list<GestureRule> rules;
        int scanCode;
        bool keyPressed = false;
        std::string debugMessage;
    public:
        GestureInput(std::list<GestureRule> rules, int keyScanCode, std::string debugMessageIfPressed = "");
        std::string getDebugMessage();
    };
}
