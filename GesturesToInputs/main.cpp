#include "main.h"

#include <dinput.h>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#include "structures.h"
#include "Gesture.h"
#include "GesturesToInputsProcessor_CPU.h"
#include "GesturesToInputsPorcessor_OpenGL.h"
#include "MediaFoundationWebcam.h"
#include "Shader.h"

#include <opencv2/highgui.hpp>

using namespace GesturesToInputs;

std::vector<GestureInput> dragonGameGestures() {
    auto boostRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::CENTRE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto flySlowlyRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::FAR_LEFT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    auto upRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (unsigned int)VERTICAL_POSITION::TOP),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::TOP)
    };

    auto tiltLeftRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::LESS_THAN, "Green")
    };

    auto downRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (unsigned int)VERTICAL_POSITION::BOTTOM),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::BOTTOM)
    };

    auto tiltRightRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::MORE_THAN, "Green")
    };

    return std::vector<GestureInput>{
        GestureInput(boostRules, DIK_F, "BOOST"),
        GestureInput(flySlowlyRules, DIK_SPACE, "SLOW DOWN"),
        GestureInput(upRules, DIK_W, "FLY UP"),
        GestureInput(tiltLeftRules, DIK_A, "TILT LEFT"),
        GestureInput(downRules, DIK_S, "FLY DOWN"),
        GestureInput(tiltRightRules, DIK_D, "TILT RIGHT"),
    };
}

std::vector<GestureInput> justCause2Gestures() {
    auto grappleRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto jumpRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto primaryWeaponRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::FAR_LEFT)
    };
    
    auto secondaryWeaponRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    auto forwardRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::TOP)
    };

    auto leftRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::FAR_LEFT)
    };

    auto backwardRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (unsigned int)VERTICAL_POSITION::BOTTOM),
    };

    auto rightRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto lookUpRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::TOP)
    };

    auto lookLeftRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto lookDownRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::BOTTOM),
    };

    auto lookRightRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    return std::vector<GestureInput>{
        GestureInput(grappleRules, DIK_F, "GRAPPLE"),
        GestureInput(jumpRules, DIK_SPACE, "JUMP"),
        GestureInput(primaryWeaponRules, MOUSEEVENTF_LEFTDOWN, "FIRE 1", GESTURE_INPUT_TYPE::MOUSE_BUTTON),
        GestureInput(secondaryWeaponRules, MOUSEEVENTF_RIGHTDOWN, "FIRE 2", GESTURE_INPUT_TYPE::MOUSE_BUTTON),
        GestureInput(lookUpRules, (int)MOUSE_DIRECTION::UP, "LOOK UP", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(lookLeftRules, (int)MOUSE_DIRECTION::LEFT, "LOOK LEFT", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(lookDownRules, (int)MOUSE_DIRECTION::DOWN, "LOOK DOWN", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(lookRightRules, (int)MOUSE_DIRECTION::RIGHT, "LOOK RIGHT", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(forwardRules, DIK_W, "FORWARD"),
        GestureInput(leftRules, DIK_A, "LEFT"),
        GestureInput(backwardRules, DIK_S, "BACK"),
        GestureInput(rightRules, DIK_D, "RIGHT"),
    };
}

std::vector<GestureInput> testGestures() {
    return std::vector<GestureInput>{
        GestureInput(std::vector<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", uint(VERTICAL_POSITION::BOTTOM)),
        }, DIK_G, "GREEN"),
        GestureInput(std::vector<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", uint(VERTICAL_POSITION::BOTTOM)),
        }, DIK_R, "RED")
    };
}

std::vector<GestureRuleData>* getRulesFromGestures(std::vector<GestureInput>* gestures, std::map<std::string, unsigned int>* trackerNameIndexMap) {
    auto rules = new std::vector<GestureRuleData>;
    unsigned int gestureIndex = 0;
    for (auto& gesture : *gestures) {
        for (auto& rule : gesture.getRules()) {
            GestureRuleData ruleData;
            ruleData.gestureIndex = gestureIndex;
            ruleData.type = unsigned int(rule.getType());
            ruleData.operation = int(rule.getOperation());
            ruleData.expectedValue = rule.getExpectedValue();
            ruleData.compareTwoTrackers = rule.isComparingTwoTrackers() ? 1 : 0;
            ruleData.trackerIndex = trackerNameIndexMap->at(rule.trackerName);
            ruleData.comparisonTrackerIndex = rule.isComparingTwoTrackers() ? trackerNameIndexMap->at(rule.comparisonTrackerName) : 0;
            rules->push_back(ruleData);
        }
        gestureIndex++;
    }
    return rules;
}

std::vector<ThresholdData>* getThresholdData(float greenTracker[4], float redTracker[4]) {
    auto thresholdData = new std::vector<ThresholdData>;
    thresholdData->push_back(ThresholdData(new float[4]{ 80, 111, 90, 255 }, new float[4]{ 95, 255, 255, 255 }, greenTracker));
    thresholdData->push_back(ThresholdData(new float[4]{ 167, 0, 0, 255 }, new float[4]{ 179, 255, 255, 255 }, redTracker));
    thresholdData->push_back(ThresholdData(new float[4]{ 0, 145, 90, 255 }, new float[4]{ 10, 215, 240, 255 }, redTracker));
    return thresholdData;
}

int main(int argc, char** argv)
{
    // TODO:: If no GPU or opengl version < 4.3 then fall back to cpu method.
    // TODO:: Cpu method may be optimisable since we only need rough bounding box. Moments can certainly be removed, and perhaps more optimisaitons.

    bool useGPU = true;

    if (useGPU) {
        try {
            Shader::setRoot(std::filesystem::path(argv[0]).parent_path() / "shaders");
            auto gestures = justCause2Gestures();
            float greenTracker[4] = { 87, 183, 183, 255 }; float redTracker[4] = { 174, 179, 205, 255 };
            
            MediaFoundationWebcam* webcam = new MediaFoundationWebcam();
            std::thread t1(&MediaFoundationWebcam::CreateVideoCaptureDevice, webcam);
            t1.detach();

            webcam->wait();
            auto openGlProcessor = new GesturesToInputsPorcessor_OpenGL(
                webcam,
                getThresholdData(greenTracker, redTracker),
                new std::vector<TrackerData>{ TrackerData(greenTracker), TrackerData(redTracker) },
                getRulesFromGestures(&gestures, new std::map<std::string, unsigned int>{ {"Green", 0}, {"Red", 1} }),
                &gestures
            );

            openGlProcessor->run();
        }
        catch (std::string e) {
            std::cout << "An exception occurred: " << e << std::endl;
        }
    }
    else {
        try {
            auto redTrackerValues = std::vector<TrackerValues> { TrackerValues(0, 10, 145, 215, 90, 240), TrackerValues(165, 179, 0, 255, 0, 255) };
            auto greenTrackerValues = std::vector<TrackerValues> { TrackerValues(80, 95, 111, 255, 90, 255) };
        
            auto redTracker = new Tracker("Red", redTrackerValues, cv::Scalar(0, 0, 255));
            auto greenTracker = new Tracker("Green", greenTrackerValues, cv::Scalar(0, 255, 0));
            std::map<std::string, Tracker*> trackers = {
                { "Red",  redTracker },
                { "Green", greenTracker }
            };

            auto gestures = justCause2Gestures();

            auto processor = GesturesToInputsProcessor_CPU(trackers, &gestures);
            processor.webcamIndex = 0;
            processor.webcamMirrored = true;
            processor.run();
        }
        catch (int e) {
            std::cout << "An exception occurred. Exception Nr. " << e << std::endl;;
        }
    
        return 0;
    }
}
