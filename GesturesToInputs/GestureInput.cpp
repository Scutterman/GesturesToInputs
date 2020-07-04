#include "GestureInput.h"

namespace GesturesToInputs {
    GestureInput::GestureInput(std::vector<GestureRule> rules, int keyScanCode, std::string debugMessageIfPressed, GESTURE_INPUT_TYPE inputType)
    {
        this->rules = rules;
        value = keyScanCode;
        debugMessage = debugMessageIfPressed;
        this->inputType = inputType;
    }

    std::vector<GestureRule> GestureInput::getRules()
    {
        return rules;
    }

    int GestureInput::getValue()
    {
        return value;
    }

    std::string GestureInput::getDebugMessage()
    {
        return debugMessage;
    }
    
    GESTURE_INPUT_TYPE GestureInput::getInputType()
    {
        return inputType;
    }
}
