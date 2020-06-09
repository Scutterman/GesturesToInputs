#pragma once
#include <opencv2/core/types.hpp>

#include "WebcamFrame.h"

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
