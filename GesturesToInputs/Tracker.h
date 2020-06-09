#pragma once
#include <opencv2\core\types.hpp>
#include <opencv2\imgproc.hpp>

namespace GesturesToInputs {
    enum class VERTICAL_POSITION { BOTTOM, MIDDLE, TOP };
    enum class HORIZONTAL_POSITION { FAR_LEFT, MIDDLE_LEFT, CENTRE, MIDDLE_RIGHT, FAR_RIGHT };

    class Tracker {
    private:
        int lastX = -1, lastY = -1;
        VERTICAL_POSITION lastVerticalPosition;
        HORIZONTAL_POSITION lastHorizontalPosition;
        cv::Scalar colour;

        int topThird;
        int bottomThird;
        int farLeft;
        int middleLeft;
        int middleRight;
        int farRight;

        bool drawTrackingLine = false;
    public:
        cv::Mat lines;
        Tracker(cv::Scalar colour, cv::Size size, bool drawTrackingLine = false);

        void track(cv::Mat threshold);

        VERTICAL_POSITION getVerticalPosition();
        HORIZONTAL_POSITION getHorizontalPosition();
        int getXPosition();
    };
}