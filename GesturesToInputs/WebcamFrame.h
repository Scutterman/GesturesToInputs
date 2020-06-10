#pragma once
#include <opencv2/videoio.hpp>

namespace GesturesToInputs {
    class WebcamFrame {
    public:
        const int NO_WEBCAM_FRAME = 100;
        cv::Mat source;

        WebcamFrame(cv::VideoCapture cap, bool mirroredCapture);

        void drawGrid();
    };
}
