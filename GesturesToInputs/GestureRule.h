#pragma once
#include <string>
#include "Tracker.h"
enum class GESTURE_RULE_TYPE { HORIZONTAL_COMPARE, VERTICAL_COMPARE, RELATIVE_HORIZONTAL_COMPARE, RELATIVE_VERTICAL_COMPARE };
enum class GESTURE_RULE_COMPARISON_OPERATION { LESS_THAN = -1, EQUAL = 0, MORE_THAN = 1 };

namespace GesturesToInputs {
    class GestureRule
    {
    private:
        GESTURE_RULE_TYPE type;
        std::string trackerName;
        std::string comparisonTrackerName;
        GESTURE_RULE_COMPARISON_OPERATION operation;
        int expectedValue;
        bool compare(int trackerValue);
        bool _isComparingTwoTrackers;
    public:
        GestureRule(GESTURE_RULE_TYPE type, std::string trackerName, int expectedValue, GESTURE_RULE_COMPARISON_OPERATION operation = GESTURE_RULE_COMPARISON_OPERATION::EQUAL);
        GestureRule(GESTURE_RULE_TYPE type, std::string trackerName, GESTURE_RULE_COMPARISON_OPERATION operation, std::string comparisonTrackerName);
        bool compare(Tracker tracker);
        bool compare(Tracker tracker, Tracker trackerToCompare);
        bool isComparingTwoTrackers();
    };
}
