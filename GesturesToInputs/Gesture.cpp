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
                switch (input.getInputType()) {
                case GESTURE_INPUT_TYPE::KEYBOARD:
                    if (!input.active) {
                        sendInput(input.getValue());
                    }
                    break;
                case GESTURE_INPUT_TYPE::MOUSE_MOVE:
                    sendMouseMove(input.getValue());
                    break;
                case GESTURE_INPUT_TYPE::MOUSE_BUTTON:
                    if (!input.active) {
                        sendMouseButton(input.getValue());
                    }
                    break;
                }

                input.active = true;
                log(input.getDebugMessage());
            }
            else {
                switch (input.getInputType()) {
                case GESTURE_INPUT_TYPE::KEYBOARD:
                    if (input.active) {
                        cancelInput(input.getValue());
                    }
                    break;
                case GESTURE_INPUT_TYPE::MOUSE_BUTTON:
                    if (input.active) {
                        cancelMouseButton(input.getValue());
                    }
                    break;
                }

                input.active = false;
            }
        }

        cv::imshow("Text", text);
    }
    
    void Gesture::log(std::string textToAdd) {
        cv::putText(text, textToAdd, cv::Point(10, textLine), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(128));
        textLine += 40;
    }
    
    void Gesture::sendInput(int dikKeyCode) {
        ip.type = INPUT_KEYBOARD;
        ip.ki.dwFlags = KEYEVENTF_SCANCODE;
        ip.ki.wScan = dikKeyCode;
        SendInput(1, &ip, sizeof(INPUT));
    }
    
    void Gesture::cancelInput(int dikKeyCode) {
        ip.type = INPUT_KEYBOARD;
        ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        ip.ki.wScan = dikKeyCode;
        SendInput(1, &ip, sizeof(INPUT));
    }

    void Gesture::sendMouseButton(int button)
    {
        ip.type = INPUT_MOUSE;
        ip.mi.time = 0;
        ip.mi.mouseData = 0;
        ip.mi.dwExtraInfo = 0;
        ip.mi.dx = 0;
        ip.mi.dy = 0;
        ip.mi.dwFlags = button;
        SendInput(1, &ip, sizeof(INPUT));
    }

    void Gesture::cancelMouseButton(int button)
    {
        int release = 0;
        switch (button) {
        case MOUSEEVENTF_LEFTDOWN:
            release = MOUSEEVENTF_LEFTUP;
            break;
        case MOUSEEVENTF_RIGHTDOWN:
            release = MOUSEEVENTF_RIGHTUP;
            break;
        default:
            return;
        }

        ip.type = INPUT_MOUSE;
        ip.mi.time = 0;
        ip.mi.mouseData = 0;
        ip.mi.dwExtraInfo = 0;
        ip.mi.dx = 0;
        ip.mi.dy = 0;
        ip.mi.dwFlags = release;
        SendInput(1, &ip, sizeof(INPUT));
    }

    void Gesture::sendMouseMove(int direction)
    {
        int x = direction % 2;
        int y = direction / 2;
        ip.type = INPUT_MOUSE;
        ip.mi.time = 0;
        ip.mi.mouseData = 0;
        ip.mi.dwExtraInfo = 0;
        ip.mi.dx = x * 12;
        ip.mi.dy = y * 12;
        ip.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;
        SendInput(1, &ip, sizeof(INPUT));
    }
}
