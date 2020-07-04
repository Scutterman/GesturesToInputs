#include "GestureDetection.h"
#include "Gesture.h"

namespace GesturesToInputs {
    void GestureDetection::setGestures(std::vector<GestureInput>* gestures)
    {
        this->gestures = gestures;
    }

    std::vector<GestureInput>* GestureDetection::getGestures()
    {
        return gestures;
    }

    void GestureDetection::calculateInstructions(std::map<std::string, Tracker*> trackers) {
        auto gesture = Gesture::getInstance();
        gesture->reset();

        for (auto& input : *gestures) {
            bool gestureDetected = true;
            for (auto& rule : input.getRules()) {
                bool rulePassed = false;
                if (rule.isComparingTwoTrackers()) {
                    // TODO:: check trackers exist in list
                    rulePassed = rule.compare(trackers.find(rule.trackerName)->second, trackers.find(rule.comparisonTrackerName)->second);
                }
                else {
                    // TODO:: check tracker exists in list
                    rulePassed = rule.compare(trackers.find(rule.trackerName)->second);
                }

                if (!rulePassed) {
                    gestureDetected = false;
                    break;
                }
            }

            gesture->handleInput(&input, gestureDetected);
        }

        gesture->complete();
    }
}
