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
        /*std::map<std::string, Tracker>::iterator it = trackers.begin();
        auto tracker1 = *it;
        std::advance(it, 1);
        auto tracker2 = *it;
        if (tracker1.second.getXPosition() < tracker2.second.getXPosition()) {
            calculateInstructions(tracker1.second, tracker2.second);
        }
        else {
            calculateInstructions(tracker2.second, tracker1.second);
        }*/
        
        text.setTo(0);
        textLine = 40;

        for (auto& input : gestures) {
            bool gestureDetected = false;
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

            if (gestureDetected && !input.keyPressed) {
                sendInput(input.getKeyCode());
                input.keyPressed = true;
                log(input.getDebugMessage());
            }
            else if (!gestureDetected && input.keyPressed) {
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
        /*if (pressed[dikKeyCode] == false) {
            pressed[dikKeyCode] = true;
        }*/
    }
    
    void Gesture::cancelInput(int dikKeyCode) {
        ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        ip.ki.wScan = dikKeyCode;
        SendInput(1, &ip, sizeof(INPUT));
        /*if (pressed[dikKeyCode] == true) {
            pressed[dikKeyCode] = false;
        }*/
    }
    
    /*void Gesture::calculateInstructions(Tracker left, Tracker right) {
        text.setTo(0);
        textLine = 40;

        if (left.getHorizontalPosition() == HORIZONTAL_POSITION::CENTRE && right.getHorizontalPosition() == HORIZONTAL_POSITION::CENTRE) {
            log("FAST");
            sendInput(DIK_F);
            Sleep(100);
            cancelInput(DIK_F);
        }

        if (left.getHorizontalPosition() == HORIZONTAL_POSITION::FAR_LEFT && right.getHorizontalPosition() == HORIZONTAL_POSITION::FAR_RIGHT) {
            log("SLOW");
            sendInput(DIK_SPACE);
        }
        else {
            cancelInput(DIK_SPACE);
        }

        if (left.getVerticalPosition() < right.getVerticalPosition()) {
            log("TILT LEFT");
            sendInput(DIK_A);
        }
        else {
            cancelInput(DIK_A);
        }

        if (right.getVerticalPosition() < left.getVerticalPosition()) {
            log("TILT RIGHT");
            sendInput(DIK_D);
        }
        else {
            cancelInput(DIK_D);
        }

        if (left.getVerticalPosition() == VERTICAL_POSITION::TOP && right.getVerticalPosition() == VERTICAL_POSITION::TOP) {
            log("FLY UP");
            sendInput(DIK_W);
        }
        else {
            cancelInput(DIK_W);
        }

        if (left.getVerticalPosition() == VERTICAL_POSITION::BOTTOM && right.getVerticalPosition() == VERTICAL_POSITION::BOTTOM) {
            log("FLY DOWN");
            sendInput(DIK_S);
        }
        else {
            cancelInput(DIK_S);
        }

        cv::imshow("Text", text);
    }*/
}
