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

Finally
- download SSBO GestureData from gpu to cpu (done)
- send inputs for any gesture marked as found
