#include "GesturesToInputs.h"

#include <opencv2/highgui.hpp>

namespace GesturesToInputs {
    GesturesToInputsProcessor::GesturesToInputsProcessor(std::map<std::string, Tracker*> trackers, std::vector<GestureInput>* gestures)
    {
        this->trackers = trackers;
        gestureDetection->setGestures(gestures);
    }
    
    // TODO:: Every tracker currently needs to calculate its own grid
    // This should probably be handled by Gestures.
    void GesturesToInputsProcessor::run() {
        if (!cam.initialised) {
            throw ERROR_NO_WEBCAM;
        }
        
        cam.mirroredCapture = webcamMirrored;

        for (;;) {
            perf.Start();
            auto frame = cam.next();
            for (auto& tracker : trackers) { tracker.second->track(&frame.source); }

            gestureDetection->calculateInstructions(trackers);

            frame.drawGrid();
            
            cv::Mat output = cv::Mat::zeros(frame.source.size(), CV_8UC3);
            output += frame.source;
            for (auto& tracker : trackers) { output += tracker.second->lines; }
            cv::imshow("Original", output);

            perf.End();

            if (cv::waitKey(10) == 27) {
                break;
            }
        }
    }
}
