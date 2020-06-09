#include "WebcamFrame.h"
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

namespace GesturesToInputs {
    WebcamFrame::WebcamFrame(VideoCapture cap, bool mirroredCapture) {
        cap >> source;
        if (source.empty())
        {
            throw NO_WEBCAM_FRAME;
        }

        if (mirroredCapture) {
            flip(source, source, 1);
        }
    }
    Mat WebcamFrame::isolateColour(Scalar low, Scalar high) {
        Mat imageAsHSV;
        cvtColor(source, imageAsHSV, COLOR_BGR2HSV);

        Mat imageWithThreshold;
        inRange(imageAsHSV, low, high, imageWithThreshold);

        //morphological opening (removes small objects from the foreground)
        erode(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (removes small holes from the foreground)
        dilate(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imageWithThreshold, imageWithThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        return imageWithThreshold;
    }
    void WebcamFrame::drawGrid() {
        Size size = source.size();
        int oneThird = size.height / 3;
        line(source, Point(0, oneThird), Point(size.width, oneThird), Scalar(255), 2);
        line(source, Point(0, oneThird * 2), Point(size.width, oneThird * 2), Scalar(255), 2);

        int oneFifth = size.width / 5;
        line(source, Point(oneFifth, 0), Point(oneFifth, size.height), Scalar(255), 2);
        line(source, Point(oneFifth * 2, 0), Point(oneFifth * 2, size.height), Scalar(255), 2);
        line(source, Point(oneFifth * 3, 0), Point(oneFifth * 3, size.height), Scalar(255), 2);
        line(source, Point(oneFifth * 4, 0), Point(oneFifth * 4, size.height), Scalar(255), 2);
    }
}