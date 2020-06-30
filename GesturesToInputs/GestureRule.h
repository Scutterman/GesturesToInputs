#pragma once
#include <string>
#include "Tracker.h"

namespace GesturesToInputs {
    enum class GESTURE_RULE_TYPE { ORIENTATION, HORIZONTAL_COMPARE, VERTICAL_COMPARE };
    enum class GESTURE_RULE_COMPARISON_OPERATION { LESS_THAN = -1, EQUAL = 0, MORE_THAN = 1 };
    
    class GestureRule {
    private:
        GESTURE_RULE_TYPE type;
        GESTURE_RULE_COMPARISON_OPERATION operation;
        unsigned int expectedValue;
        bool compare(unsigned int trackerValue);
        bool _isComparingTwoTrackers = false;
    public:
        std::string trackerName;
        std::string comparisonTrackerName;
        GestureRule(GESTURE_RULE_TYPE type, std::string trackerName, unsigned int expectedValue, GESTURE_RULE_COMPARISON_OPERATION operation = GESTURE_RULE_COMPARISON_OPERATION::EQUAL);
        GestureRule(GESTURE_RULE_TYPE type, std::string trackerName, GESTURE_RULE_COMPARISON_OPERATION operation, std::string comparisonTrackerName);
        bool compare(Tracker* tracker);
        bool compare(Tracker* tracker, Tracker* trackerToCompare);
        bool isComparingTwoTrackers();

        GESTURE_RULE_TYPE getType();
        GESTURE_RULE_COMPARISON_OPERATION getOperation();
        unsigned int getExpectedValue();
    };
}
