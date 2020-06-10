#include "Gesture.h"
#include <windows.h>
#include <tchar.h>
#include <dinput.h>
#include <map>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Tracker.h"

namespace GesturesToInputs {
    Gesture::Gesture() {
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
    }
    void Gesture::calculateInstructionsWithUnknownOrder(std::list<Tracker> trackers) {
        std::list<Tracker>::iterator it = trackers.begin();
        auto tracker1 = *it;
        std::advance(it, 1);
        auto tracker2 = *it;
        if (tracker1.getXPosition() < tracker2.getXPosition()) {
            calculateInstructions(tracker1, tracker2);
        }
        else {
            calculateInstructions(tracker2, tracker1);
        }
    }
    
    void Gesture::log(std::string textToAdd) {
        cv::putText(text, textToAdd, cv::Point(10, textLine), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(128));
        textLine += 40;
    }
    
    void Gesture::_SendInput(int dikKeyCode) {
        if (pressed[dikKeyCode] == false) {
            pressed[dikKeyCode] = true;
            ip.ki.dwFlags = KEYEVENTF_SCANCODE;
            ip.ki.wScan = dikKeyCode;
            SendInput(1, &ip, sizeof(INPUT));
        }
    }
    
    void Gesture::CancelInput(int dikKeyCode) {
        if (pressed[dikKeyCode] == true) {
            ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            ip.ki.wScan = dikKeyCode;
            SendInput(1, &ip, sizeof(INPUT));
            pressed[dikKeyCode] = false;
        }
    }
    
    void Gesture::calculateInstructions(Tracker left, Tracker right) {
        text.setTo(0);
        textLine = 40;

        if (left.getHorizontalPosition() == HORIZONTAL_POSITION::CENTRE && right.getHorizontalPosition() == HORIZONTAL_POSITION::CENTRE) {
            log("FAST");
            _SendInput(DIK_F);
            Sleep(100);
            CancelInput(DIK_F);
        }

        if (left.getHorizontalPosition() == HORIZONTAL_POSITION::FAR_LEFT && right.getHorizontalPosition() == HORIZONTAL_POSITION::FAR_RIGHT) {
            log("SLOW");
            _SendInput(DIK_SPACE);
        }
        else {
            CancelInput(DIK_SPACE);
        }

        if (left.getVerticalPosition() < right.getVerticalPosition()) {
            log("TILT LEFT");
            _SendInput(DIK_A);
        }
        else {
            CancelInput(DIK_A);
        }

        if (right.getVerticalPosition() < left.getVerticalPosition()) {
            log("TILT RIGHT");
            _SendInput(DIK_D);
        }
        else {
            CancelInput(DIK_D);
        }

        if (left.getVerticalPosition() == VERTICAL_POSITION::TOP && right.getVerticalPosition() == VERTICAL_POSITION::TOP) {
            log("FLY UP");
            _SendInput(DIK_W);
        }
        else {
            CancelInput(DIK_W);
        }

        if (left.getVerticalPosition() == VERTICAL_POSITION::BOTTOM && right.getVerticalPosition() == VERTICAL_POSITION::BOTTOM) {
            log("FLY DOWN");
            _SendInput(DIK_S);
        }
        else {
            CancelInput(DIK_S);
        }

        cv::imshow("Text", text);
    }
}
