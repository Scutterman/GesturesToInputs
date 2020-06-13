#include "ContinuousMouseMovement.h"
#include <Windows.h>

namespace GesturesToInputs {
#ifndef CONTINUOUS_MOUSE_MOVEMENT
#define CONTINUOUS_MOUSE_MOVEMENT
    std::mutex mutex;
#endif // !CONTINUOUS_MOUSE_MOVEMENT

    void ContinuousMouseMovement::halt()
    {
        shouldHalt = true;
    }
    
    void ContinuousMouseMovement::run()
    {
        for (;;) {
            if (shouldHalt) {
                break;
            }
            
            int x = getX();
            int y = getY();
            
            if (x != 0 || y != 0) {
                ip.type = INPUT_MOUSE;
                ip.mi.time = 0;
                ip.mi.mouseData = 0;
                ip.mi.dwExtraInfo = 0;
                ip.mi.dx = x;
                ip.mi.dy = y;
                ip.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;
                SendInput(1, &ip, sizeof(INPUT));
            }
            Sleep(10);
        }
    }
    
    int ContinuousMouseMovement::getX()
    {
        std::lock_guard<std::mutex> lock(mutex);
        int x = this->x;
        return x;
    }
    int ContinuousMouseMovement::getY()
    {
        std::lock_guard<std::mutex> lock(mutex);
        int y = this->y;
        return y;
    }

    void ContinuousMouseMovement::setX(int x)
    {
        std::lock_guard<std::mutex> lock(mutex);
        this->x = x;
    }
    
    void ContinuousMouseMovement::setY(int y)
    {
        std::lock_guard<std::mutex> lock(mutex);
        this->y = y;
    }
}
