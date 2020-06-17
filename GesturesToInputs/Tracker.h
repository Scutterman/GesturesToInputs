#pragma once
#include <list>
#include <opencv2/core/mat.hpp>
#include "TrackerValues.h"

namespace GesturesToInputs {
    enum class MARKER_ORIENTATION { PORTRAIT, LANDSCAPE };
    enum class VERTICAL_POSITION { BOTTOM, MIDDLE, TOP };
    enum class HORIZONTAL_POSITION { FAR_LEFT, MIDDLE_LEFT, CENTRE, MIDDLE_RIGHT, FAR_RIGHT };

    class Tracker {
    private:
        const int maxHue = 179;
        const int maxSaturation = 255;
        const int maxValue = 255;

        std::list<TrackerValues> trackedColours;
        int lastX = -1, lastY = -1;
        MARKER_ORIENTATION orientation;
        VERTICAL_POSITION lastVerticalPosition;
        HORIZONTAL_POSITION lastHorizontalPosition;
        cv::Scalar lineColour;
        std::string name;

        bool detected = false;
        bool gridInitialised = false;
        int topThird;
        int bottomThird;
        int farLeft;
        int middleLeft;
        int middleRight;
        int farRight;

        bool drawTrackingLine = false;
        void normaliseColourRanges();
        void addControlWindows();
        void setupGrid(cv::Size size);
        void setOrientation(std::vector<cv::Point>& contour, cv::Size size);
    public:
        cv::Mat isolateColours(cv::Mat frame);
        cv::Mat lines;
        Tracker(std::string trackerName, std::list<TrackerValues> trackedColours,  cv::Scalar lineColour, bool drawTrackingLine = false);

        void track(cv::Mat frame);
        MARKER_ORIENTATION getOrientation();
        VERTICAL_POSITION getVerticalPosition();
        HORIZONTAL_POSITION getHorizontalPosition();
        int getXPosition();
        std::string getName();
        bool isDetected();
    };
}
