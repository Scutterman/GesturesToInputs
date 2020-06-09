#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "PerformanceTimer.h"

namespace GesturesToInputs {
    PerformanceTimer::PerformanceTimer() {
        cv::namedWindow("Frame Time", cv::WINDOW_AUTOSIZE);
    }
    
    void PerformanceTimer::Start()
    {
        startPoint = std::chrono::high_resolution_clock::now();
    }
    
    void PerformanceTimer::End()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count();
        frameTime.setTo(0);
        cv::putText(frameTime, std::to_string(duration) + "ms", cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(128));
        cv::imshow("Frame Time", frameTime);
    }
}