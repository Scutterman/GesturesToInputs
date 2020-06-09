#include "Tracker.h"

namespace GesturesToInputs {
    Tracker::Tracker(cv::Scalar colour, cv::Size size, bool drawTrackingLine) {
        // Origin is top left so higher y value is actually lower down in the image.
        topThird = size.height / 3;
        bottomThird = topThird * 2;

        farLeft = size.width / 5;
        middleLeft = farLeft * 2;
        middleRight = farLeft * 3;
        farRight = farLeft * 4;

        this->colour = colour;
        this->drawTrackingLine = drawTrackingLine;
        lines = cv::Mat::zeros(size, CV_8UC3);
    }

    void Tracker::track(cv::Mat threshold) {
        cv::Moments imageMoments = moments(threshold);
        double area = imageMoments.m00;

        if (area > 200000) {
            int centreX = imageMoments.m10 / area;
            int centreY = imageMoments.m01 / area;


            if (centreY <= topThird) { lastVerticalPosition = VERTICAL_POSITION::TOP; }
            else if (centreY >= bottomThird) { lastVerticalPosition = VERTICAL_POSITION::BOTTOM; }
            else { lastVerticalPosition = VERTICAL_POSITION::MIDDLE; }

            if (centreX <= farLeft) { lastHorizontalPosition = HORIZONTAL_POSITION::FAR_LEFT; }
            else if (centreX <= middleLeft) { lastHorizontalPosition = HORIZONTAL_POSITION::MIDDLE_LEFT; }
            else if (centreX <= middleRight) { lastHorizontalPosition = HORIZONTAL_POSITION::CENTRE; }
            else if (centreX <= farRight) { lastHorizontalPosition = HORIZONTAL_POSITION::MIDDLE_RIGHT; }
            else { lastHorizontalPosition = HORIZONTAL_POSITION::FAR_RIGHT; }

            if (drawTrackingLine && lastX >= 0 && lastY >= 0 && centreX >= 0 && centreY >= 0) {
                line(lines, cv::Point(centreX, centreY), cv::Point(lastX, lastY), colour, 2);
            }

            lastX = centreX;
            lastY = centreY;
        }
    }
    
    VERTICAL_POSITION Tracker::getVerticalPosition() { return lastVerticalPosition; }
    HORIZONTAL_POSITION Tracker::getHorizontalPosition() { return lastHorizontalPosition; }
    int Tracker::getXPosition() { return lastX; }
}