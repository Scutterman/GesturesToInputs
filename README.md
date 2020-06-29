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
Each instance of the "Tracker" class represents one marker you want to track.
It has min and max thresholds for Hue, Saturation, and Value that represent what the colour range of the marker should be.
An OpenCV webcam instance, controlled by the "Webcam" class, provides a frame of video that is fed to the Tracker.track() method.
This then calls Tracker.isolateColours(), which uses OpenCV methods to:
- Convert the frame to HSV format
- Turn all pixels whose values are inside the colour range white, and everything else black
- erode, dialate, dilate, erode to clean up any unwanted noise and small objects
- return the black and white frame back to the track() method

The track method can now use the OpenCV "moments" function to work out the x and y co-ordinates of the centre of the (hopefully only) white area that represents the marker.
These co-ordinates are then reduced down into whether the marker is at the top, middle, or bottom of the screen and whether it is on the far left, middle left, centre, middle right, or far right (placing it in any one of fifteen different grid squares).

The process is repeated for all markers and then the individual position of a marker, or its relative position to another marker, can be used by the "Gesture" class to work out whether the markers are positioned such that they form a gesture that is linked to a keyboard input.

The Windows Development Kit "SendInput" method is used by Gesture._SendInput to send a direct input to whatever window has the focus.
If this happens to be a game, the result is that a gesture controls the game just like a direct keyboard input would.

# Roadmap
## Profiles
- Allow saving the current marker colour values to disk so they can be used next time the program is run
- Allow saving gesture profiles to disk so different games can have different profiles

## Webcam
- See if we can detect when a camera disconnects and reconnects - look into exception "The video recording device is no longer present"
- See if Media Foundation can disable webcam features such as "low light compensation" that kill the framerate.
- More conversions - eventually all formats returned by Media Foundation should be usable
- Intelligently picking the best webcam format from all available options instead of picking the first that matches

## Shaders
Use local groups instead of global
- *MaxInstances variables can be used to calculate appropriate values for local x, y, and z to ensure they don't overflow allowed invocations
- prepend a string containing #DEFINE macros to the beginning of each shader to "dynamically" set the local x, y, and z values
- Each shader may need to put loops in place to ensure all work gets executed
    - e.g: in a 64x64 image with 1024 max executions, local_size_x=32, local_size_y=32, local_size_z=1, xRepeat = 2, xRepeatOffset = 32, yRepeat = 2, yRepeatOffset = 2
    ```
        for (int xOffset = 0; xOffset < xRepeat * xRepeatOffset; xOffset += xRepeatOffset) {
            for (int yOffset = 0; yOffset < yRepeat * yRepeatOffset; yOffset += yRepeatOffset) {
                ivec2 coords = ivec2(gl_LocalInvocationID.x + xOffset, gl_LocalInvocationID.y + yOffset)
                /** Now process the pixel **/
            }
        }
    ```
    - This should ensure that gl_LocalInvocationID(0,0,0) will process pixel (0,0), (0,32), (32,0), and finally (32, 32)
    - gl_LocalInvocationID(31,31,0) will process pixel (31,31), (31,63), (63,0), and finally (63, 63)
    - All of the in-between invocations will process the pixels in between

## Configuration Window
A window that allows better control of the program
- User input adjustments to the webcam red/green/blue/lightness levels
- Adjusting the size of the erode / dilate, and the minimum area allowed for  tracking
- Choosing a webcam from a list
- Webcam settings such as low light compensation, autofocus, image levels, etc.
- Selecting a target window & subwindow so input is not sent if it is not in focus ([see here](https://docs.microsoft.com/en-gb/windows/win32/api/winuser/nf-winuser-getforegroundwindow))
- Adding an arbritary number of markers (with distinct names) and chosing one or more colour ranges per marker, with inline threshold box
- Auto-configuring a marker by prompting it to be positioned inside outlines placed at each corner and the centre and then auto-adjusting the inputs until just before the outline is not filled with white. Determine whether the inputs overlap sufficiently to allow a single range or whether several ranges are required
- Allow custom gesture inputs to be added with one or more rules, including position of named marker and relation between two named markers (horizontally or vertically less than, equal, more than)
- Allow for "gesture profiles" to be saved so different games can have their own set of inputs and can be chosen from a dropdown list
- Switching between a DirectInput / DirectX target and a normal keyboard target
- Keyboard localisation
- "No GPU" mode for when the GPU isn't very powerful or the game makes high demands of it
    - Will need to replace all OpenCV code with custom code working on bytes from the MediaFoundationWebcam class because the OpenCV methods in Tracker::isolateColours() run extremely slowly (8ms - 15ms for each of six methods means the method takes ~65ms)

## Android App
Investigate whether it would be possible to port this code to a mobile app.
OpenGL ES is much more resource limited (particularly how many textures / buffers can be bound at once) so it might be tricky.
The program on the PC would create a local web server that the app connects to.
When the phone connects, it receives a list of markers and gestures.
The app would send over a list of detected gesture indexes after processing each frame so the PC can send the correct inputs to the game.