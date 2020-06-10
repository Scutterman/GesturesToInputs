# How to use
1. Install OpenCV according to their [instructions](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html), remembering to set the environment variable as shown at the bottom of the page.
1. Run the program to bring up the program windows
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

# Planned Features
## Miscellaneous
- A "no marker detected" state that resets the output keys and takes no action until a marker is detected again
- Automatically create two ColourValues if any of the "low" sliders are higher than the "high" slider - one for "low" to "max" and one for "min" to "high"
- Allow saving the current marker positions so they can be used next time the program is run
- Shape tracking to augment or replace colour tracking

## Configuration Window
A window that allows better control of the program
- Adjusting the size of the erode / dilate, and the minimum area allowed for  tracking
- Choosing a webcam from a list
- Selecting a target window & subwindow so input is not sent if it is not in focus
- Adding an arbritary number of markers (with distinct names) and chosing one or more colour ranges per marker, with inline threshold box
- Auto-configuring a marker by prompting it to be positioned inside outlines placed at each corner and the centre and then auto-adjusting the inputs until just before the outline is not filled with white. Determine whether the inputs overlap sufficiently to allow a single range or whether several ranges are required
- Allow custom gesture inputs to be added with one or more rules, including position of named marker and relation between two named markers (horizontally or vertically less than, equal, more than)
- Allow for "gesture profiles" to be saved so different games can have their own set of inputs and can be chosen from a dropdown list
