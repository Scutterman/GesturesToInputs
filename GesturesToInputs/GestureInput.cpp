#include "GestureInput.h"

namespace GesturesToInputs {
    GestureInput::GestureInput(std::list<GestureRule> rules, int keyScanCode, std::string debugMessageIfPressed)
    {
        this->rules = rules;
        scanCode = keyScanCode;
        debugMessage = debugMessageIfPressed;
    }

    std::list<GestureRule> GestureInput::getRules()
    {
        return rules;
    }

    int GestureInput::getKeyCode()
    {
        return scanCode;
    }

    std::string GestureInput::getDebugMessage()
    {
        return debugMessage;
    }
}
