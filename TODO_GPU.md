# Setup
- Get gpu to calculate and store box positions
- Store gesture data using tracker indexes instead of tracker names

# Each Frame
All Shaders:
    - Convert everything to use [0...1] ranges not [0...255]
Shader 1: Convert to HSV
    - Process one pixel per thread (global_work_group.x = pixel.x, global_work_group.y = pixel.y)
Shader 2: Thresholding
    - Process one pixel per tracker per thread (global_work_group.x = pixel.x, global_work_group.y = pixel.y, global_work_group.z = trackedColour)
        - threshold
        - morphological opening
        - morphological closing
Shader 3: Blob detection
    - Process one sample per tracker per thread (global_work_group.x = sample.x, global_work_group.y = sample.y, global_work_group.z = tracker)
        - currently implemented in shaders/ObjectBoundingBoxSearch_Pass1.comp for a single tracker
        - Multiply shader data size by the number of trackers
        - Initialise shader data before dispatch
        - Pass in tracker colour along with shader data and check x, y, and z values of pixel against tracker colour
        - Use local work groups so memoryBarrier() works correctly
        - Return area and bounding box as pixels and not samples
        - Prevent Susurration indexes from wrapping around to the right-hand side of the grid
        - Ensure bottomLeft Susurration index does not go beyond the bottom of the grid
Shader 4: Marker calculations
    - Process one tracker per thread (global_work_group.x = tracker)
        - find largest blob
        - find width and height (furthest right point - furthest left point, uppermost point - lowest point)
        - find centre (half width and height added to top-left)
        - calculate horizontal / vertical / orientation values as ints
Shader 5: Gesture detection
    - Process one gesture per thread (global_work_group.x = gesture)
        - check tracker horizontal / vertical / orientation values to see if gesture is detected
        - Add input type / value to a global list if gesture detected
- pull input list from gpu to cpu
- send all gesture instructions
- Some shaders may perform well enough on CPU that GPU is not required. Perf testing during development will determine this.
