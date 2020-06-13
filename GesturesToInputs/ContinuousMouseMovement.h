#pragma once
#include <windows.h>
#include <mutex>

namespace GesturesToInputs {
    class ContinuousMouseMovement
    {
    private:
        int x = 0, y = 0;
        bool shouldHalt = false;
        INPUT ip;

        int getX();
        int getY();
    public:
        void halt();
        void run();
        void setX(int x);
        void setY(int y);
    };
}
