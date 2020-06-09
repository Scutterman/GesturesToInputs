#pragma once
#include <chrono>
#include <opencv2/imgproc.hpp>

namespace GesturesToInputs {
    class PerformanceTimer {
    private:
        cv::Mat frameTime = cv::Mat(100, 300, CV_8UC1);
        std::chrono::steady_clock::time_point startPoint;

    public:
        PerformanceTimer();
        void Start();
        void End();
    };
}
