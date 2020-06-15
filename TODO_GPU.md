# Setup
- Get gpu to calculate and store box positions
- Store gesture data using tracker indexes instead of tracker names

# Each Frame
- load frame to gpu
    - convert frame to HSV
    - for each tracker in parallel
        - for each tracked colour in parallel
            - threshold
            - morphological opening
            - morphological closing
        - add all tracked colours
        - find largest contiguous white area
        - find width and height (furthest right point - furthest left point, uppermost point - lowest point)
        - find centre (half width and height added to top-left)
        - calculate horizontal / vertical / orientation values as ints
    - for each gesture in parallel
        - check if gesture is detected
    - compile list of input type / value for all detected gestures
- pull input list from gpu
- send all gesture instructions

# Tracker width and centre position
Getting the position of the trackers efficiently may be difficult.

We need to draw a box around all contiguous white areas pixels and then use the top-left and bottom-right corners to work out the rough centre.

This is easy enough in serial because we can have one process look for a white pixel and walk around it.

It's hard to have a gpu "walk" around the area if each shader invocation has no knowledge of the state of other pixels
and it's difficult to prevent two invocations from repeating the same work.

The best way might be to sample the image into small areas and do several passes to join contiguous areas.

First pass:
Sample the image into 10x10 squares and send each sample to the gpu
    - Mark the position of the leftmost (tlX), topmost (tlY), rightmost (brX), and bottommost (brY) white pixel in the sample
    - Keep a count of the number of white pixels in the sample as its area
    - If one of the edge pixels is white, mark as potentially contiguous with another sample

Second Pass:
Group samples in pairs.
One set horizontal to check right/left contiguous connections and either keep or reset the flag.
One set vertical to check bottom/top.
Maybe keep the discard flag for no-connections area-too-small.
After this pass is complete, loop through all samples on the cpu and make a list of objects which includes a list of connected samples within that object
- Each sample will have top, left, bottom, right, area values
- Each object will have one or more samples
- Each frame will have zero or more objects

Third Pass:
Each GPU thread gets an object index and a sample value
The object index points to a vertex storing top, left, bottom, right, area values for the whole object
The thread will atomic add the sample area to the object area
The thread will atomic min the top and left sample values with the respective object values
The thread will atomic max the right and bottom sample values with the respective object values

Fourth Pass:
Each thread gets an object index and does an atomic max between its area and a global area.
This somehow needs to surface the object index of the max area