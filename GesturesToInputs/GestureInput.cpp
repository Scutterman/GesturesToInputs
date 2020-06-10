#include "GestureInput.h"

GesturesToInputs::GestureInput::GestureInput(std::list<GestureRule> rules, int keyScanCode, std::string debugMessageIfPressed)
{
    this->rules = rules;
    scanCode = keyScanCode;
    debugMessage = debugMessageIfPressed;
}

std::string GesturesToInputs::GestureInput::getDebugMessage()
{
    return debugMessage;
}
