#include "main.h"

#include <iostream>

#include "GesturesToInputs.h"

using namespace GesturesToInputs;

int main(int argc, char** argv)
{
    try {
        auto redTrackerValues = std::list<TrackerValues>{
               TrackerValues(0, 10, 113, 255, 58, 255),
               TrackerValues(169, 179, 104, 255, 151, 255),
        };

        auto greenTrackerValues = std::list<TrackerValues>{ TrackerValues(80, 95, 111, 255, 110, 255) };
        
        std::map<std::string, Tracker> trackers = {
            { "Red", Tracker("Red", redTrackerValues, cv::Scalar(0, 0, 255)) },
            { "Green", Tracker("Green", greenTrackerValues, cv::Scalar(0, 255, 0)) }
        };

        auto grappleRules = std::list<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::CENTRE),
            GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::CENTRE)
        };

        auto jumpRules = std::list<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::FAR_LEFT),
            GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::FAR_RIGHT)
        };

        auto leftRules = std::list<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::RELATIVE_VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::LESS_THAN, "Green")
        };

        auto rightRules = std::list<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::RELATIVE_VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::MORE_THAN, "Green")
        };

        auto forwardRules = std::list<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::TOP),
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (int)VERTICAL_POSITION::TOP)
        };

        auto backwardRules = std::list<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::BOTTOM),
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (int)VERTICAL_POSITION::BOTTOM)
        };

        auto gestures = std::list<GestureInput>{
            GestureInput(grappleRules, DIK_F, "GRAPPLE"),
            GestureInput(jumpRules, DIK_SPACE, "JUMP"),
            GestureInput(forwardRules, DIK_W, "FORWARD"),
            GestureInput(leftRules, DIK_A, "LEFT"),
            GestureInput(backwardRules, DIK_S, "BACK"),
            GestureInput(rightRules, DIK_D, "RIGHT"),
        };

        auto processor = GesturesToInputsProcessor(trackers, gestures);
        processor.webcamIndex = 0;
        processor.webcamMirrored = true;
        processor.run();
    }
    catch (int e) {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    
    return 0;
}
