#include "GestureRule.h"

namespace GesturesToInputs {
    GestureRule::GestureRule(GESTURE_RULE_TYPE type, std::string trackerName, int expectedValue, GESTURE_RULE_COMPARISON_OPERATION operation)
    {
        this->type = type;
        this->trackerName = trackerName;
        this->expectedValue = expectedValue;
        this->operation = operation;
    }

    GestureRule::GestureRule(GESTURE_RULE_TYPE type, std::string trackerName, GESTURE_RULE_COMPARISON_OPERATION operation, std::string comparisonTrackerName)
    {
        this->type = type;
        this->trackerName = trackerName;
        this->operation = operation;
        this->comparisonTrackerName = comparisonTrackerName;
        this->_isComparingTwoTrackers = true;
    }

    bool GestureRule::compare(Tracker tracker)
    {
        // TODO:: throw exception
        if (tracker.getName() != trackerName) {
            return false;
        }

        switch (type) {
        case GESTURE_RULE_TYPE::HORIZONTAL_COMPARE:
            return compare((int)tracker.getHorizontalPosition());
        case GESTURE_RULE_TYPE::VERTICAL_COMPARE:
            return compare((int)tracker.getVerticalPosition());
        default:
            // TODO:: throw exception
            return false;
        }
    }

    bool GestureRule::compare(Tracker tracker, Tracker trackerToCompare)
    {
        // TODO:: throw exception
        if (tracker.getName() != trackerName) {
            return false;
        }
    
        // TODO:: throw exception
        if (trackerToCompare.getName() != comparisonTrackerName) {
            return false;
        }

        switch (type) {
        case GESTURE_RULE_TYPE::RELATIVE_HORIZONTAL_COMPARE:
            expectedValue = (int)trackerToCompare.getHorizontalPosition();
            return compare((int)tracker.getHorizontalPosition());
        case GESTURE_RULE_TYPE::RELATIVE_VERTICAL_COMPARE:
            expectedValue = (int)trackerToCompare.getVerticalPosition();
            return compare((int)tracker.getVerticalPosition());
        default:
            // TODO:: throw exception
            return false;
        }
    }

    bool GestureRule::isComparingTwoTrackers()
    {
        return _isComparingTwoTrackers;
    }

    bool GestureRule::compare(int trackerValue)
    {
        int relativity = trackerValue == expectedValue ? 0 : trackerValue < expectedValue ? -1 : 1;
        return relativity == (int)operation;
    }
}
