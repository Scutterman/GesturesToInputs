#include "GesturesToInputs.h"

#include <iostream>

#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Gesture.h"
#include "PerformanceTimer.h"
#include "Tracker.h"
#include "Webcam.h"
#include "WebcamFrame.h"

using namespace GesturesToInputs;

int main(int argc, char** argv)
{
    try {
        Webcam cam = Webcam();
        if (!cam.initialised) {
            return 0;
        }
        cam.mirroredCapture = true;

        Tracker trackers[] = {
            Tracker("Red", std::list<TrackerValues>{
                TrackerValues(0, 10, 113, 255, 58, 255),
                TrackerValues(169, 179, 104, 255, 151, 255),
            }, cv::Scalar(0, 0, 255), cam.next().source.size()),
            Tracker("Green", std::list<TrackerValues> { TrackerValues(80, 95, 111, 255, 110, 255) }, cv::Scalar(0, 255, 0), cam.next().source.size())
        };

        Gesture gesture;
        PerformanceTimer perf;
        
        for (;;) {
            perf.Start();
            WebcamFrame frame = cam.next();
            for (auto& tracker : trackers) { tracker.track(frame.source); }

            gesture.calculateInstructionsWithUnknownOrder(trackers[0], trackers[1]);

            frame.drawGrid();
            cv::imshow("Original", frame.source + trackers[0].lines + trackers[1].lines);
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
