#include "WebcamFrame.h"

#include <opencv2/imgproc.hpp>

namespace GesturesToInputs {
    WebcamFrame::WebcamFrame(cv::VideoCapture cap, bool mirroredCapture) {
        cap >> source;
        if (source.empty())
        {
            throw NO_WEBCAM_FRAME;
        }

        if (mirroredCapture) {
            flip(source, source, 1);
        }
    }
    cv::Mat WebcamFrame::isolateColour(cv::Scalar low, cv::Scalar high) {
        cv::Mat imageAsHSV;
        cvtColor(source, imageAsHSV, cv::COLOR_BGR2HSV);

        cv::Mat imageWithThreshold;
        inRange(imageAsHSV, low, high, imageWithThreshold);

        //morphological opening (removes small objects from the foreground)
        erode(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        dilate(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

        //morphological closing (removes small holes from the foreground)
        dilate(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        erode(imageWithThreshold, imageWithThreshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        return imageWithThreshold;
    }
    void WebcamFrame::drawGrid() {
        cv::Size size = source.size();
        int oneThird = size.height / 3;
        line(source, cv::Point(0, oneThird), cv::Point(size.width, oneThird), cv::Scalar(255), 2);
        line(source, cv::Point(0, oneThird * 2), cv::Point(size.width, oneThird * 2), cv::Scalar(255), 2);

        int oneFifth = size.width / 5;
        line(source, cv::Point(oneFifth, 0), cv::Point(oneFifth, size.height), cv::Scalar(255), 2);
        line(source, cv::Point(oneFifth * 2, 0), cv::Point(oneFifth * 2, size.height), cv::Scalar(255), 2);
        line(source, cv::Point(oneFifth * 3, 0), cv::Point(oneFifth * 3, size.height), cv::Scalar(255), 2);
        line(source, cv::Point(oneFifth * 4, 0), cv::Point(oneFifth * 4, size.height), cv::Scalar(255), 2);
    }
}