#include "GestureRule.h"

namespace GesturesToInputs {
    GestureRule::GestureRule(GESTURE_RULE_TYPE type, std::string trackerName, unsigned int expectedValue, GESTURE_RULE_COMPARISON_OPERATION operation)
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

    bool GestureRule::compare(Tracker* tracker)
    {
        // TODO:: throw exception
        if (tracker->getName() != trackerName) {
            return false;
        }
        
        if (!tracker->isDetected()) {
            return false;
        }

        switch (type) {
        case GESTURE_RULE_TYPE::ORIENTATION:
            return compare((unsigned int)tracker->getOrientation());
        case GESTURE_RULE_TYPE::HORIZONTAL_COMPARE:
            return compare((unsigned int)tracker->getHorizontalPosition());
        case GESTURE_RULE_TYPE::VERTICAL_COMPARE:
            return compare((unsigned int)tracker->getVerticalPosition());
        default:
            // TODO:: throw exception
            return false;
        }
    }

    bool GestureRule::compare(Tracker* tracker, Tracker* trackerToCompare)
    {
        // TODO:: throw exception
        if (tracker->getName() != trackerName) {
            return false;
        }
    
        // TODO:: throw exception
        if (trackerToCompare->getName() != comparisonTrackerName) {
            return false;
        }

        if (!tracker->isDetected() || !trackerToCompare->isDetected()) {
            return false;
        }

        switch (type) {
        case GESTURE_RULE_TYPE::ORIENTATION:
            expectedValue = (unsigned int)trackerToCompare->getOrientation();
            return compare((unsigned int)tracker->getOrientation());
        case GESTURE_RULE_TYPE::HORIZONTAL_COMPARE:
            expectedValue = (unsigned int)trackerToCompare->getHorizontalPosition();
            return compare((unsigned int)tracker->getHorizontalPosition());
        case GESTURE_RULE_TYPE::VERTICAL_COMPARE:
            expectedValue = (unsigned int)trackerToCompare->getVerticalPosition();
            return compare((unsigned int)tracker->getVerticalPosition());
        default:
            // TODO:: throw exception
            return false;
        }
    }

    bool GestureRule::isComparingTwoTrackers()
    {
        return _isComparingTwoTrackers;
    }

    GESTURE_RULE_TYPE GestureRule::getType()
    {
        return type;
    }

    GESTURE_RULE_COMPARISON_OPERATION GestureRule::getOperation()
    {
        return operation;
    }

    unsigned int GestureRule::getExpectedValue()
    {
        return expectedValue;
    }

    bool GestureRule::compare(unsigned int trackerValue)
    {
        int relativity = trackerValue == expectedValue ? 0 : trackerValue < expectedValue ? -1 : 1;
        return relativity == (int)operation;
    }
}
