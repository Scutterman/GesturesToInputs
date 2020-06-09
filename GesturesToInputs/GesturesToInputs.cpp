#include "GesturesToInputs.h"

#include <iostream>

#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ControlWindow.h"
#include "Gesture.h"
#include "PerformanceTimer.h"
#include "Tracker.h"
#include "Webcam.h"
#include "WebcamFrame.h"

using namespace GesturesToInputs;

int main(int argc, char** argv)
{
    try {
        ControlWindow redUpperControls = ControlWindow("Red U Controls", 169, 179, 104, 255, 151, 255);
        ControlWindow redLowerControls = ControlWindow("Red L Controls", 0, 10, 113, 255, 58, 255);
        ControlWindow greenControls = ControlWindow("Green Controls", 80, 95, 111, 255, 110, 255);
        
        Webcam cam = Webcam();
        if (!cam.initialised) {
            return 0;
        }
        cam.mirroredCapture = true;
    
        Tracker redTracker = Tracker(cv::Scalar(0, 0, 255), cam.next().source.size());
        Tracker greenTracker = Tracker(cv::Scalar(0, 255, 0), cam.next().source.size());

        Gesture gesture;
        PerformanceTimer perf;
        for (;;) {
            perf.Start();
            WebcamFrame frame = cam.next();
            
            cv::Mat redThreshold = frame.isolateColour(redUpperControls.lowScalar(), redUpperControls.highScalar()) + frame.isolateColour(redLowerControls.lowScalar(), redLowerControls.highScalar());
            redTracker.track(redThreshold);
            
            cv::Mat greenThreshold = frame.isolateColour(greenControls.lowScalar(), greenControls.highScalar());
            greenTracker.track(greenThreshold);

            gesture.calculateInstructionsWithUnknownOrder(redTracker, greenTracker);

            frame.drawGrid();
            //imshow("Threshold", redThreshold + greenThreshold); // Show our image inside it.
            cv::imshow("Original", frame.source + redTracker.lines + greenTracker.lines);
            perf.End();
            
            if (cv::waitKey(10) == 27) {
                break;
            }
        }
    }
    catch (int e) {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    return 0;
}
