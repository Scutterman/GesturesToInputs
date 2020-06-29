# Setup
- Get gpu to calculate and store box positions
- Store gesture data using tracker indexes instead of tracker names

# Each Frame
Shader 0: Convert to RGB from whatever format the webcam throws out
Shader 1: Convert to HSV

Shader 2: Thresholding
- morphological opening
- morphological closing

Shader 3: Largest blob position detection
- Prevent Susurration indexes from wrapping around to the right-hand side of the grid
- Ensure bottomLeft Susurration index does not go beyond the bottom of the grid

Shader 4: Gesture detection
- INPUT: SSBO containing GestureData
- INPUT: SSBO containing array of GestureRuleData
- INPUT: SSBO containing array of TrackerData
- Process one gesture rule per thread (global_work_group.x = gesture index, global_work_group.y = gesture rule index)
    - gestures[gestureIndex] = 2
    - memoryBarrier()
    - check tracker horizontal / vertical / orientation values to see if gesture is detected
    - uint gestureFound = (detected) ? 1 : 0;
    - atomicMin(gestures[gestureIndex].found, gestureFound);
- If the first gesture to finish processing is detected, atomicMin will set gesture.found from 2 to 1
- atomicMin for all other gestures detected before a failed detection will keep gesture.found at 1
- If a gesture is not detected, atomicMin will set gesture.found from either 2 or 1 to 0
- For all gestures after a failure, atomicMin will keep gesture.found at 0

Finally
- download SSBO GestureData from gpu to cpu
- send inputs for any gesture marked as found
