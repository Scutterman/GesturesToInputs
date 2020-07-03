#pragma once

#include <vector>
#include <map>

#include "GestureInput.h"

namespace GesturesToInputs {
    class GestureDetection
    {
    private:
        std::vector<GestureInput>* gestures;
    public:
        void setGestures(std::vector<GestureInput>* gestures);
        void calculateInstructions(std::map<std::string, Tracker*> trackers);
    };
}