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
    - If one of the edge pixels is white, check the pixels of the other sample to see if they are contiguous

First pass input:
- Sample X Index
- Sample Y Index
- vec2 imageSize // width, height
- vec2 sampleSize // width, height

First pass output:
Array of:
- vec4 sampleBoundingBox  // left, top, right, bottom
- vec4 contiguous // left, top, right, bottom
- int area
- int objectIndex
- vec2 padding

// It might be better to do this as a compute shader with only one thread to save pulling data back to the cpu.
// Could I use memory barriers to do this in the first pass?
After first pass is complete, loop through all samples on the cpu and make a list of objects which includes a list of connected samples within that object
- Each sample will have top, left, bottom, right, area values
- Each object will have one or more samples
- Each frame will have zero or more objects

Second pass:
Each GPU thread gets an object index and a sample value
The object index points to a vertex storing top, left, bottom, right, area values for the whole object
The thread will atomic add the sample area to the object area
The thread will atomic min the top and left sample values with the respective object values
The thread will atomic max the right and bottom sample values with the respective object values

Second pass input:
- Object Index X

Second pass output:
Array of:
- vec4 objectBoundingBox
- int area
- vec3 padding

Third Pass:
CPU loops through objects and selects the index of the one with the highest area.
Should only be a few objects so this shouldn't eat up too much tiem.