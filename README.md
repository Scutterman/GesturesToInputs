# How to use
1. Install OpenCV according to their [instructions](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html), remembering to set the environment variable as shown at the bottom of the page.
1. Build and run the program to bring up the program windows
    - Webcam preview: a video feed from the first camera detected on the PC, overlayed with the grid used to determine where a marker is on-screen for use in gesture processing
    - Green marker configuration window
    - Two red marker configuration windows (red occupies both the highest and lowest points of the HSV hue spectrum, so has to have two configurations)
    - A red and a green "threshold" window that shows white for any object detected as a marker
    - Output information box containing any gestures detected
    - A window showing how long each gesture takes to process from before the frame is captured to when the input is delivered
1. If one of the markers is not detected, or there is too much noise, the marker might need to be re-configured
    - Reduce all "low" sliders for the marker to the lowest position and all "high" sliders for the marker to the highest position.
        - For the red controls, one window will need all controls to be set to the highest position while the first one is calibrated to see the marker at the lower range of the red hue
    - Gradually move the "Low Hue" slider up until just before the shape of the marker in the threashold window starts to fade to black
    - Gradually move the "High Hue" slider down until just before the shape of the marker in the threashold window starts to fade to black
    - Repeat for each of the other four sliders
    - Test to make sure the marker can be seen in all four corners of the screen and the centre
        - Red markers may not appear in al four locations until the second configuration has been completed
    - For a red marker, repeat the process with the second control window to calibrate the upper range of the red hue
1. Open Notepad and ensure it has keyboard focus as you move the markers around the screen - letters will appear as the gestures are picked up and send inputs
1. Open the game and make the gestures to contol the player character
1. Press the escape button when the webcam window has focus to close down gesture control

# Troubleshooting
1. Ensure the area seen by the webcam is well lit and evenly lit.
1. Ensure you're not wearing any colours too close to the marker colours, and nothing in the background is too similar similar to the marker colours.

# How this works
## GPU
A YUY2 formatted webcam frame is captured using the `MediaFoundationWebcam` class and is fed into the shader pipeline.

`Convert_YUY2.comp` decodes it into a full RGB image (one shader invocation for every two pixels to be converted) and `ConvertToHSV.comp` convertes it to HSV format (one shader invocation per pixel).

`Threshold.comp` will receive the hsv frame and an array of trackers and determine which, if any, tracker detected each pixel as being the correct marker colour (one shader invocation per pixel will loop over each tracker to find a match for that pixel).

Each tracker array element contains upper and lower HSV colours that mark the detection range, plus the colour to turn a pixel which is detected by that tracker.

A rough form of morphological opening and closing is performed on the results to reduce false positives.

`ObjectBoundingBoxSearch_Pass1.comp` (one shader invocation per sample per tracker) works on "samples" within the image. A sample is a grid of pixels within the image, and is counted as "detected" for a tracker if at least half the pixels match the tracker's colour.

Due to the thresholding, all pixles will either be one of the tracker colours or be empty (the HSV value `(0,0,0,0)`). The shader makes a number of "passes" to determine whether ajoining samples are also detected by the same tracker and how big the entire contiguous area is.

Finally, each tracker is assigned information about the largest of the congiguous areas detected in its colour so that this information can be used in the gesture detection.

`DetectGestures.comp` (one shader incocation for every rule of every gesture) receives the tracker information as well as a set of rules for each gesture. It returns whether each gesture has been detected based on the location and orientation of each tracker.

Back on the CPU, the `GestureInput` related to each gesture is passed to the `Gesture` class along with whether the gesture was detected by the GPU. It uses the Windows Development Kit `SendInput` method to send or cancle a direct input to whatever window has the focus, dpending on whether the gesture was detected this frame and whether it was detected last frame.

If the focused window is a game, the end result is that each gesture acts to control the game just like a direct keyboard, mouse, or controller input would.

## CPU
Each instance of the `Tracker` class represents one marker you want to track.
It has min and max thresholds for Hue, Saturation, and Value that represent what the colour range of the marker should be.
An OpenCV webcam instance, controlled by the "Webcam" class, provides a frame of video that is fed to the `Tracker.track()` method.
This then calls `Tracker.isolateColours()`, which uses OpenCV methods to:
- Convert the frame to HSV format
- Turn all pixels whose values are inside the colour range white, and everything else black
- erode, dialate, dilate, erode to clean up any unwanted noise and small objects

The track method can now use the OpenCV `moments()` function to work out the x and y co-ordinates of the centre of the (hopefully only) white area that represents the marker.
These co-ordinates are then reduced down into whether the marker is at the top, middle, or bottom of the screen and whether it is on the far left, middle left, centre, middle right, or far right (placing it in any one of fifteen different grid squares).

The process is repeated for all markers and then the individual position of a marker, or its relative position to another marker, can be used by the `GestureDetection` class to work out whether the markers are positioned such that they form a gesture that is linked to a keyboard input. This is all handled by the `GestureInput` class, which has one insance per gesture and contains a set of `GestureRule`s that have to be met in order for the gesture to be detected. It also contains the last state of the gesture (detected or not) and information on what keyboard or mouse input needs to be sent to the system if the gesture is detected.

The `Gesture` class receives the `GestureInput` and whether the gesture was detected this frame. It uses the Windows Development Kit `SendInput` method to send a direct input to whatever window has the focus. If this happens to be a game, the result is that a gesture controls the game just like a direct keyboard input would.
