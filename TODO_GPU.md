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

First pass input:
- Sample X Index
- Sample Y Index
- vec2 imageSize // width, height
- vec2 sampleSize // M, N - larger sample size is faster but less accurate
- vec2 sampleMax // number of columns and rows of samples in the image
- int numberOfpasses // Higher number of passes = higher chance at successfully working out the bounding box but higher processing time. May be possible to best-guess on CPU based on number of sample rows and columns

- Array Buffer [MxN] of:
    - vec4 objectBoundingBox // minimum left and minimum top, maximum right and maximum bottom of all contiguous areas
    - int isOn
    - int area
    - int objectTopLeftSampleIndex
    - int isObjectTopLeft

First pass execution:

To update bounding box when interacting with a contiguous sample:
- objectBoundingBox.x = atomicMin(objectBoundingBox.x, contiguousSample.objectBoundingBox.x)
- objectBoundingBox.y = atomicMin(objectBoundingBox.y, contiguousSample.objectBoundingBox.y)
- objectBoundingBox.z = atomicMax(objectBoundingBox.z, contiguousSample.objectBoundingBox.z)
- objectBoundingBox.w = atomicMax(objectBoundingBox.w, contiguousSample.objectBoundingBox.w)
- contiguousSample.x = atomicMin(objectBoundingBox.x, contiguousSample.objectBoundingBox.x)
- contiguousSample.y = atomicMin(objectBoundingBox.y, contiguousSample.objectBoundingBox.y)
- contiguousSample.z = atomicMax(objectBoundingBox.z, contiguousSample.objectBoundingBox.z)
- contiguousSample.w = atomicMax(objectBoundingBox.w, contiguousSample.objectBoundingBox.w)

Sample the image into NxM size rectangles and send each sample to the gpu
    - calculate sample index = Sample X Index + (Sample Y Index * sampleMax.columns)
    - Set isObjectTopLeft = 0, isOn = 0, area = 0, objectTopLeftSampleIndex = sample index
    - Set the objectBoundingBox in the output to be (Sample X Index, Sample Y Index, Sample X Index + M, Sample Y Index + N)
    - Loop all pixels, count white and black pixels
    - If there are more white than black pixels, set isOn = 1
Memory Barrier
    - Loop for numberOfpasses interations:
        - For each sample sample to topLeft, top, left, and bottomLeft
            - If isOn and surrounding sample isOn then update bounding box
        - Memory Barrier
    - Set object sample index using objectBoundingBox.x and objectBoundingBox.y to calculate sample index of top-left position of object
    - Set approximate area based on top left and bottom right co-ordinates
    - If approximate area is less than threshold then set area to 0, isOn to 0
    - If isOn set isObjectTopLeft = 1 for sample at objectBoundingBox.x, objectBoundingBox.y // may not be a part of this object because left and top minimums may have come from different samples

On CPU:
Loop through sample:
    - Draw bounding box on image for every sample with isObjectTopLeft = 1
    - Track largestArea and largestAreaSampleIndex
If largestAreaSampleIndex > -1 then object centre and object orientation can be worked out based on its objectBoundingBox
