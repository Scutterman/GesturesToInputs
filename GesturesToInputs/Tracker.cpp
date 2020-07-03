#include "Tracker.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

namespace GesturesToInputs {
    Tracker::Tracker(std::string trackerName, std::vector<TrackerValues> trackedColours, cv::Scalar lineColour, bool drawTrackingLine) {
        this->lineColour = lineColour;
        this->drawTrackingLine = drawTrackingLine;
        name = trackerName;
        
        this->trackedColours = trackedColours;
        normaliseColourRanges();
        addControlWindows();
    }

    void Tracker::normaliseColourRanges()
    {
        for (auto& values : trackedColours) {
            if (values.low.hue > values.high.hue) {
                int newTrackerHighHue = values.high.hue;
                values.high.hue = maxHue;
                trackedColours.push_back(TrackerValues(0, newTrackerHighHue, values.low.saturation, values.high.saturation, values.low.value, values.high.value));
            }
        }
    }

    void Tracker::addControlWindows() {
        int i = 1;
        for (auto& initialValues : trackedColours) {
            auto windowName = name + " colour #" + std::to_string(i);
            cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

            cv::createTrackbar("Low Hue", windowName, &initialValues.low.hue, maxHue);
            cv::createTrackbar("High Hue", windowName, &initialValues.high.hue, maxHue);

            cv::createTrackbar("Low Saturation", windowName, &initialValues.low.saturation, maxSaturation);
            cv::createTrackbar("High Saturation", windowName, &initialValues.high.saturation, maxSaturation);

            cv::createTrackbar("Low Value", windowName, &initialValues.low.value, maxValue);
            cv::createTrackbar("High Value", windowName, &initialValues.high.value, maxValue);
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

    void Tracker::setOrientation(std::vector<cv::Point>& contour, cv::Size size)
    {
        int minX = size.width;
        int maxX = -1;
        int minY = size.height;
        int maxY = -1;

        for (auto point : contour) {
            if (point.x < minX) { minX = point.x; }
            if (point.x > maxX) { maxX = point.x; }
            if (point.y < minY) { minY = point.y; }
            if (point.y > maxY) { maxY = point.y; }
        }

        int width = maxX - minX;
        int height = maxY - minY;
        orientation = width <= height ? MARKER_ORIENTATION::PORTRAIT : MARKER_ORIENTATION::LANDSCAPE;
    }

    void Tracker::track(cv::Mat* frame) {
        if (!gridInitialised) {
            setupGrid(frame->size());
            gridInitialised = true;
        }

        std::vector<std::vector<cv::Point>> contours;
        cv::Mat threshold = cv::Mat::zeros(frame->size(), CV_8U);
        isolateColours(frame, &threshold);
        cv::findContours(threshold, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        detected = false;
        for (auto& contour :contours) {
            if (cv::contourArea(contour) < 350) { continue; }
            
            detected = true;
            
            cv::Moments imageMoments = cv::moments(contour);
            double area = imageMoments.m00;

            int centreX = imageMoments.m10 / area;
            int centreY = imageMoments.m01 / area;
            setOrientation(contour, frame->size());

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

            break;
        }
    }

    void Tracker::isolateColours(cv::Mat* frame, cv::Mat* threshold) {
        cv::Mat imageAsHSV;
        cv::cvtColor(*frame, imageAsHSV, cv::COLOR_BGR2HSV);

        for (auto& values : trackedColours) {
            cv::Mat imageWithThreshold;
            inRange(imageAsHSV, values.low.toScalar(), values.high.toScalar(), imageWithThreshold);

            //morphological opening (removes small objects from the foreground)
            erode(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            dilate(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            //morphological closing (removes small holes from the foreground)
            dilate(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            erode(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            *threshold += imageWithThreshold;
        }

        cv::imshow(name + " threshold value", *threshold);
    }
    
    MARKER_ORIENTATION Tracker::getOrientation()
    {
        return orientation;
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
