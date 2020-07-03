# Each Frame
Shader 0: Convert to RGB from whatever format the webcam throws out
Shader 1: Convert to HSV
Shader 2: Thresholding
Shader 3: Largest blob position detection
Shader 4: Gesture detection

Finally
- create `GestureDetection` class that handles `calculateInstructions()` for the CPU
- Rework `Gesture` class to be singleton which receives `std::vector<GestureInput> inputs` and `std::vector<bool> detected` to send the gestures
- Use new `Gesture` class to allow the GPU to send keyboard and mouse inputs
