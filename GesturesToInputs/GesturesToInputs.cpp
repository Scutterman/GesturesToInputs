#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include <chrono>
#include <windows.h>
#include <tchar.h>
#include <dinput.h>
#include <map>

using namespace cv;
using namespace std;

class ControlWindow {
public:
    int lowHue, lowSaturation, lowValue,
        highHue, highSaturation, highValue;

      ControlWindow(string name, int lowHue = 0, int highHue = 179, int lowSaturation = 0, int highSaturation = 255, int lowValue = 0, int highValue = 255) {
          this->lowHue = lowHue;
          this->highHue = highHue;
          this->lowSaturation = lowSaturation;
          this->highSaturation = highSaturation;
          this->lowValue = lowValue;
          this->highValue = highValue;

          namedWindow(name, WINDOW_AUTOSIZE);

          createTrackbar("Low Hue", name, &this->lowHue, 179);
          createTrackbar("High Hue", name, &this->highHue, 179);

          createTrackbar("Low Saturation", name, &this->lowSaturation, 255);
          createTrackbar("High Saturation", name, &this->highSaturation, 255);

          createTrackbar("Low Value", name, &this->lowValue, 255);
          createTrackbar("High Value", name, &this->highValue, 255);
      }

      Scalar lowScalar() {
          return Scalar(lowHue, lowSaturation, lowValue);
      }

      Scalar highScalar() {
          return Scalar(highHue, highSaturation, highValue);
      }
};

class Frame {
public:
    Mat source;

    Frame(VideoCapture cap, bool mirroredCapture) {
        cap >> source;
        if (source.empty()) // Check for invalid input
        {
            cout << "Could not open or find the image" << std::endl;
            throw 100;
        }

        if (mirroredCapture) {
            flip(source, source, 1);
        }
    }

    Mat isolateColour(Scalar low, Scalar high) {
        Mat imageAsHSV;
        cvtColor(source, imageAsHSV, COLOR_BGR2HSV);

        Mat imageWithThreshold;
        inRange(imageAsHSV, low, high, imageWithThreshold);

        //morphological opening (removes small objects from the foreground)
        erode(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (removes small holes from the foreground)
        dilate(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        return imageWithThreshold;
    }

    void drawGrid() {
        Size size = source.size();
        int oneThird = size.height / 3;
        line(source, Point(0, oneThird), Point(size.width, oneThird), Scalar(255), 2);
        line(source, Point(0, oneThird * 2), Point(size.width, oneThird * 2), Scalar(255), 2);

        int oneFifth = size.width / 5;
        line(source, Point(oneFifth, 0), Point(oneFifth, size.height), Scalar(255), 2);
        line(source, Point(oneFifth * 2, 0), Point(oneFifth * 2, size.height), Scalar(255), 2);
        line(source, Point(oneFifth * 3, 0), Point(oneFifth * 3, size.height), Scalar(255), 2);
        line(source, Point(oneFifth * 4, 0), Point(oneFifth * 4, size.height), Scalar(255), 2);
    }
};

enum class VERTICAL_POSITION { BOTTOM, MIDDLE, TOP };
enum class HORIZONTAL_POSITION { FAR_LEFT, MIDDLE_LEFT, CENTRE, MIDDLE_RIGHT, FAR_RIGHT };

class Tracker {
private:
    int lastX = -1, lastY = -1;
    VERTICAL_POSITION lastVerticalPosition;
    HORIZONTAL_POSITION lastHorizontalPosition;
    Scalar colour;

    int topThird;
    int bottomThird;
    int farLeft;
    int middleLeft;
    int middleRight;
    int farRight;

    bool drawTrackingLine = false;
public:
    Mat lines;
    Tracker(Scalar colour, Size size, bool drawTrackingLine = false) {
        // Origin is top left so higher y value is actually lower down in the image.
        topThird = size.height / 3;
        bottomThird = topThird * 2;
        
        farLeft = size.width / 5;
        middleLeft = farLeft * 2;
        middleRight = farLeft * 3;
        farRight = farLeft * 4;
        
        this->colour = colour;
        this->drawTrackingLine = drawTrackingLine;
        lines = Mat::zeros(size, CV_8UC3);
    }
    
    void track(Mat threshold) {
        Moments imageMoments = moments(threshold);
        double area = imageMoments.m00;
        
        if (area > 200000) {
            double centreX = imageMoments.m10 / area;
            double centreY = imageMoments.m01 / area;

            
            if (centreY <= topThird) { lastVerticalPosition = VERTICAL_POSITION::TOP; }
            else if (centreY >= bottomThird) { lastVerticalPosition = VERTICAL_POSITION::BOTTOM;  }
            else { lastVerticalPosition = VERTICAL_POSITION::MIDDLE; }

            if (centreX <= farLeft) { lastHorizontalPosition = HORIZONTAL_POSITION::FAR_LEFT; }
            else if (centreX <= middleLeft) { lastHorizontalPosition = HORIZONTAL_POSITION::MIDDLE_LEFT; }
            else if (centreX <= middleRight) { lastHorizontalPosition = HORIZONTAL_POSITION::CENTRE; }
            else if (centreX <= farRight) { lastHorizontalPosition = HORIZONTAL_POSITION::MIDDLE_RIGHT; }
            else { lastHorizontalPosition = HORIZONTAL_POSITION::FAR_RIGHT; }

            if (drawTrackingLine && lastX >= 0 && lastY >= 0 && centreX >= 0 && centreY >= 0) {
                line(lines, Point(centreX, centreY), Point(lastX, lastY), colour, 2);
            }

            lastX = centreX;
            lastY = centreY;
        }
    }

    VERTICAL_POSITION getVerticalPosition() { return lastVerticalPosition; }
    HORIZONTAL_POSITION getHorizontalPosition() { return lastHorizontalPosition; }
    int getXPosition() { return lastX; }
};

class Webcam {
private:
    VideoCapture cap;
public:
    bool initialised;
    bool mirroredCapture = false;
    
    Webcam() {
        initialised = cap.open(0);
    }

    Frame next() {
        return Frame(cap, mirroredCapture);
    }
};

class Gesture {
public:
    Gesture() {
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
    }

    void calculateInstructionsWithUnknownOrder(Tracker tracker1, Tracker tracker2) {
        if (tracker1.getXPosition() < tracker2.getXPosition()) {
            calculateInstructions(tracker1, tracker2);
        }
        else {
            calculateInstructions(tracker2, tracker1);
        }
    }
private:
    Mat text = Mat(100, 500, CV_8UC1);
    int textLine = 40;
    INPUT ip;
    std::map<int, bool> pressed = {
        { DIK_W, false },
        { DIK_W, false },
        { DIK_W, false },
        { DIK_W, false },
        { DIK_W, false },
        { DIK_W, false },
    };
    
    void log(string textToAdd) {
        putText(text, textToAdd, Point(10, textLine), FONT_HERSHEY_SIMPLEX, 1, Scalar(128));
        textLine += 40;
    }

    void _SendInput(int dikKeyCode) {
        if (pressed[dikKeyCode] == false) {
            pressed[dikKeyCode] = true;
            ip.ki.dwFlags = KEYEVENTF_SCANCODE;
            ip.ki.wScan = dikKeyCode;
            SendInput(1, &ip, sizeof(INPUT));
        }
    }

    void CancelInput(int dikKeyCode) {
        if (pressed[dikKeyCode] == true) {
            ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            ip.ki.wScan = dikKeyCode;
            SendInput(1, &ip, sizeof(INPUT));
            pressed[dikKeyCode] = false;
        }
    }
    
    void calculateInstructions(Tracker left, Tracker right) {
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

        //imshow("Text", text);
    }
};

int main(int argc, char** argv)
{
    try {
        Mat frameTime = Mat(100, 300, CV_8UC1);
        // namedWindow("Frame Time", WINDOW_AUTOSIZE);

        ControlWindow redUpperControls = ControlWindow("Red U Controls", 169, 179, 104, 255, 151, 255);
        ControlWindow redLowerControls = ControlWindow("Red L Controls", 0, 10, 113, 255, 58, 255);
        ControlWindow greenControls = ControlWindow("Green Controls", 80, 95, 111, 255, 110, 255);
        
        Webcam cam;
        if (!cam.initialised) {
            return 0;
        }
        cam.mirroredCapture = true;
    
        Tracker redTracker = Tracker(Scalar(0, 0, 255), cam.next().source.size());
        Tracker greenTracker = Tracker(Scalar(0, 255, 0), cam.next().source.size());

        Gesture gesture;

        for (;;) {
            auto t1 = std::chrono::high_resolution_clock::now();
            Frame frame = cam.next();
            
            Mat redThreshold = frame.isolateColour(redUpperControls.lowScalar(), redUpperControls.highScalar()) + frame.isolateColour(redLowerControls.lowScalar(), redLowerControls.highScalar());
            redTracker.track(redThreshold);
            
            Mat greenThreshold = frame.isolateColour(greenControls.lowScalar(), greenControls.highScalar());
            greenTracker.track(greenThreshold);

            gesture.calculateInstructionsWithUnknownOrder(redTracker, greenTracker);

            frame.drawGrid();
            //imshow("Threshold", redThreshold + greenThreshold); // Show our image inside it.
            imshow("Original", frame.source + redTracker.lines + greenTracker.lines);
            
            auto t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
            frameTime.setTo(0);
            putText(frameTime, to_string(duration) + "ms", Point(10, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(128));
            //imshow("Frame Time", frameTime);

            if (waitKey(10) == 27) {
                break;
            }
        }
    }
    catch (int e) {
        cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    return 0;
}
