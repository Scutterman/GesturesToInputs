#pragma once
#include "glad/glad.h"

struct ThresholdData {
    float lowColour[4];
    float highColour[4];
    float trackerColour[4];

    ThresholdData(float _lowColour[4], float _highColour[4], float _trackerColour[4]) {
        lowColour[0] = _lowColour[0];
        lowColour[1] = _lowColour[1];
        lowColour[2] = _lowColour[2];
        lowColour[3] = _lowColour[3];
        highColour[0] = _highColour[0];
        highColour[1] = _highColour[1];
        highColour[2] = _highColour[2];
        highColour[3] = _highColour[3];
        trackerColour[0] = _trackerColour[0];
        trackerColour[1] = _trackerColour[1];
        trackerColour[2] = _trackerColour[2];
        trackerColour[3] = _trackerColour[3];
    }
};

struct ObjectSearchData {
    GLuint boundingBox[4];
    GLuint objectTrackerIndex;
    GLuint padding[3];
};

struct TrackerData {
    float colour[4];
    unsigned int area = 0;
    unsigned int horizontalPosition = 0;
    unsigned int verticalPosition = 0;
    unsigned int orientation = 0;
    unsigned int isVisibleInFrame = 0;
    unsigned int padding[3];

    TrackerData(float _colour[4]) {
        colour[0] = _colour[0];
        colour[1] = _colour[1];
        colour[2] = _colour[2];
        colour[3] = _colour[3];
        padding[0] = padding[1] = padding[2] = 0;
    }
};
