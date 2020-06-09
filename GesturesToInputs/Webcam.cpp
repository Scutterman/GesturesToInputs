#include "Webcam.h"

namespace GesturesToInputs {
    Webcam::Webcam() {
        initialised = cap.open(0);
    }
    WebcamFrame Webcam::next() {
        return WebcamFrame(cap, mirroredCapture);
    }
}