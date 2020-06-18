# Setup
- Get gpu to calculate and store box positions
- Store gesture data using tracker indexes instead of tracker names

# Each Frame
Shader 1: Convert to HSV
    - Process one pixel per thread (global_work_group.x = pixel.x, global_work_group.y = pixel.y)
Shader 2: Thresholding
    - Process one pixel per tracker per thread (global_work_group.x = pixel.x, global_work_group.y = pixel.y, global_work_group.z = trackedColour)
        - threshold
        - morphological opening
        - morphological closing
        - Ensure the shader knows what tracker the colour belongs to so a single Image2D per tracker can be updated by all tracked colours at once
Shader 3: Blob detection
    - Process one sample per tracker per thread (global_work_group.x = sample.x, global_work_group.y = sample.y, global_work_group.z = tracker)
        - currently implemented in shaders/ObjectBoundingBoxSearch_Pass1.comp for a single tracker
        - Needs adjusting for multiple trackers
        - Check if we need to add "bottom-left" into susurration stage
        - Check to make sure memory barrier works across global work groups
        - Update so it returns area and bounding box as pixels and not samples
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
