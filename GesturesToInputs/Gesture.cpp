#include "Gesture.h"
#include <windows.h>
#include <tchar.h>
#include <thread>
#include <dinput.h>
#include <map>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Tracker.h"

namespace GesturesToInputs {
    Gesture::Gesture() {
        startMouseThread();
        
        this->gestures = gestures;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
    }

    Gesture::~Gesture()
    {
        mouseMovement.halt();
    }

    void Gesture::setGestures(std::list<GestureInput> gestures)
    {
        this->gestures = gestures;
    }

    void Gesture::startMouseThread()
    {
        std::thread t1(&ContinuousMouseMovement::run, &mouseMovement);
        t1.detach();
    }

    void GesturesToInputs::Gesture::reset()
    {
        text.setTo(0);
        textLine = 40;
        x = 0; y = 0;
    }

    void Gesture::calculateInstructions(std::map<std::string, Tracker*> trackers) {
        reset();

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

            handleInput(&input, gestureDetected);
        }

        complete();
    }

    void Gesture::handleInput(GestureInput* input, bool gestureDetected)
    {
        if (gestureDetected) {
            switch (input->getInputType()) {
            case GESTURE_INPUT_TYPE::KEYBOARD:
                if (!input->active) {
                    sendInput(input->getValue());
                }
                break;
            case GESTURE_INPUT_TYPE::MOUSE_MOVE:
            {
                int direction = input->getValue();
                int gestureX = direction % 2;
                if (gestureX != 0) { x = gestureX; }
                int gestureY = direction / 2;
                if (gestureY != 0) { y = gestureY; }
                break;
            }
            case GESTURE_INPUT_TYPE::MOUSE_BUTTON:
                if (!input->active) {
                    sendMouseButton(input->getValue());
                }
                break;
            }

            input->active = true;
            log(input->getDebugMessage());
        }
        else {
            switch (input->getInputType()) {
            case GESTURE_INPUT_TYPE::KEYBOARD:
                if (input->active) {
                    cancelInput(input->getValue());
                }
                break;
            case GESTURE_INPUT_TYPE::MOUSE_BUTTON:
                if (input->active) {
                    cancelMouseButton(input->getValue());
                }
                break;
            }

            input->active = false;
        }
    }

    void GesturesToInputs::Gesture::complete()
    {
        mouseMovement.setX(x);
        mouseMovement.setY(y);

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
}
