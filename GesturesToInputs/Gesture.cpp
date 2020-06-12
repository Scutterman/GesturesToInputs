#include "Gesture.h"
#include <windows.h>
#include <tchar.h>
#include <dinput.h>
#include <map>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Tracker.h"

namespace GesturesToInputs {
    Gesture::Gesture(std::list<GestureInput> gestures) {
        this->gestures = gestures;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
    }
    
    void Gesture::calculateInstructions(std::map<std::string, Tracker> trackers) {
        text.setTo(0);
        textLine = 40;

        for (auto& input : gestures) {
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

            if (gestureDetected) {
                if (!input.keyPressed) {
                    sendInput(input.getKeyCode());
                    input.keyPressed = true;
                }
                log(input.getDebugMessage());
            }
            else if (input.keyPressed) {
                cancelInput(input.getKeyCode());
                input.keyPressed = false;
            }
        }
        
        cv::imshow("Text", text);
    }
    
    void Gesture::log(std::string textToAdd) {
        cv::putText(text, textToAdd, cv::Point(10, textLine), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(128));
        textLine += 40;
    }
    
    void Gesture::sendInput(int dikKeyCode) {
        ip.ki.dwFlags = KEYEVENTF_SCANCODE;
        ip.ki.wScan = dikKeyCode;
        SendInput(1, &ip, sizeof(INPUT));
    }
    
    void Gesture::cancelInput(int dikKeyCode) {
        ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        ip.ki.wScan = dikKeyCode;
        SendInput(1, &ip, sizeof(INPUT));
    }
}
