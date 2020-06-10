#include "main.h"

#include <iostream>

#include "GesturesToInputs.h"

using namespace GesturesToInputs;

int main(int argc, char** argv)
{
    try {
        auto redTrackerValues = std::list<TrackerValues>{
               TrackerValues(0, 10, 113, 255, 58, 255),
               TrackerValues(169, 179, 104, 255, 151, 255),
        };

        auto greenTrackerValues = std::list<TrackerValues>{ TrackerValues(80, 95, 111, 255, 110, 255) };
        
        std::map<std::string, Tracker> trackers = {
            { "Red", Tracker("Red", redTrackerValues, cv::Scalar(0, 0, 255)) },
            { "Green", Tracker("Green", greenTrackerValues, cv::Scalar(0, 255, 0)) }
        };

        auto processor = GesturesToInputsProcessor(trackers);
        processor.webcamIndex = 0;
        processor.webcamMirrored = true;
        processor.run();
    }
    catch (int e) {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    
    return 0;
}
