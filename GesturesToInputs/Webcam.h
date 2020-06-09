#pragma once
#include "WebcamFrame.h"
#include <opencv2/core/types.hpp>

namespace GesturesToInputs {
    class Webcam {
    private:
        cv::VideoCapture cap;
    public:
        bool initialised;
        bool mirroredCapture = false;

        Webcam();

        WebcamFrame next();
    };
}
