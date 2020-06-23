#include "PerformanceTimer.h"
#include <iostream>

namespace GesturesToInputs {
    PerformanceTimer::PerformanceTimer() {
    }
    
    void PerformanceTimer::Start()
    {
        startPoint = std::chrono::high_resolution_clock::now();
    }
    
    void PerformanceTimer::End()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();
        float duration = std::chrono::duration_cast<std::chrono::microseconds>(endPoint - startPoint).count() / (float)1000;
        std::cout << duration << "ms" << std::endl;
    }
}