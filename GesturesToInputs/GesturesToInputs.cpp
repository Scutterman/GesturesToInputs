#include "GesturesToInputs.h"

#include <opencv2/highgui.hpp>

namespace GesturesToInputs {
    GesturesToInputsProcessor::GesturesToInputsProcessor(std::list<Tracker> trackers)
    {
        this->trackers = trackers;
    }
    // TODO:: GesturesToInputs class should have a list of gestures passed into it
    // Configuration will be supplied by main()
    
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
            for (auto& tracker : trackers) { tracker.track(frame.source); }

            gesture.calculateInstructionsWithUnknownOrder(trackers);

            frame.drawGrid();
            
            cv::Mat output = cv::Mat::zeros(frame.source.size(), CV_8UC3);
            output += frame.source;
            for (auto& tracker : trackers) { output += tracker.lines; }
            cv::imshow("Original", output);

            perf.End();

            if (cv::waitKey(10) == 27) {
                break;
            }
        }
    }
}
