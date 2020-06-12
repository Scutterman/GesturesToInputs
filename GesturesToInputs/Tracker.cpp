#include "Tracker.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

namespace GesturesToInputs {
    Tracker::Tracker(std::string trackerName, std::list<TrackerValues> trackedColours, cv::Scalar lineColour, bool drawTrackingLine) {
        this->trackedColours = trackedColours;
        this->lineColour = lineColour;
        this->drawTrackingLine = drawTrackingLine;
        name = trackerName;
        
        addControlWindows();
    }

    void Tracker::addControlWindows() {
        int i = 1;
        for (auto &initialValues : trackedColours) {
            auto windowName = name + " colour #" + std::to_string(i);
            cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

            cv::createTrackbar("Low Hue", windowName, &initialValues.low.hue, 179);
            cv::createTrackbar("High Hue", windowName, &initialValues.high.hue, 179);

            cv::createTrackbar("Low Saturation", windowName, &initialValues.low.saturation, 255);
            cv::createTrackbar("High Saturation", windowName, &initialValues.high.saturation, 255);

            cv::createTrackbar("Low Value", windowName, &initialValues.low.value, 255);
            cv::createTrackbar("High Value", windowName, &initialValues.high.value, 255);
            i++;
        }
    }

    void Tracker::setupGrid(cv::Size size) {
        // Origin is top left so higher y value is actually lower down in the image.
        topThird = size.height / 3;
        bottomThird = topThird * 2;

        farLeft = size.width / 5;
        middleLeft = farLeft * 2;
        middleRight = farLeft * 3;
        farRight = farLeft * 4;
        lines = cv::Mat::zeros(size, CV_8UC3);
    }

    void Tracker::track(cv::Mat frame) {
        if (!gridInitialised) {
            setupGrid(frame.size());
            gridInitialised = true;
        }

        cv::Moments imageMoments = cv::moments(isolateColours(frame));
        double area = imageMoments.m00;

        detected = area > 200000;
        if (detected) {
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
                line(lines, cv::Point(centreX, centreY), cv::Point(lastX, lastY), lineColour, 2);
            }

            lastX = centreX;
            lastY = centreY;
        }
    }

    cv::Mat Tracker::isolateColours(cv::Mat frame) {
        cv::Mat imageAsHSV;
        cv::cvtColor(frame, imageAsHSV, cv::COLOR_BGR2HSV);
        cv::Mat threshold = cv::Mat::zeros(frame.size(), CV_8U);

        for (auto& values : trackedColours) {
            cv::Mat imageWithThreshold;
            inRange(imageAsHSV, values.low.toScalar(), values.high.toScalar(), imageWithThreshold);

            //morphological opening (removes small objects from the foreground)
            erode(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            dilate(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            //morphological closing (removes small holes from the foreground)
            dilate(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            erode(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            threshold += imageWithThreshold;
        }

        cv::imshow(name + " threshold value", threshold);
        return threshold;
    }
    
    VERTICAL_POSITION Tracker::getVerticalPosition() { return lastVerticalPosition; }
    HORIZONTAL_POSITION Tracker::getHorizontalPosition() { return lastHorizontalPosition; }
    int Tracker::getXPosition() { return lastX; }
    std::string Tracker::getName()
    {
        return name;
    }

    bool Tracker::isDetected()
    {
        return detected;
    }
}
