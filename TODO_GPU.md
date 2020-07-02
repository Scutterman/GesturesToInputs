# Each Frame
Shader 0: Convert to RGB from whatever format the webcam throws out
Shader 1: Convert to HSV
Shader 2: Thresholding
Shader 3: Largest blob position detection
Shader 4: Gesture detection

Finally
- download SSBO GestureData from gpu to cpu (done)
- send inputs for any gesture marked as found
