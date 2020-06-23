# Setup
- Get gpu to calculate and store box positions
- Store gesture data using tracker indexes instead of tracker names

# Each Frame
All Shaders:
- Use local groups instead of global
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

Shader 1: Convert to HSV

Shader 2: Thresholding
- morphological opening
- morphological closing

Shader 3: Blob detection
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

Finally
- pull input list from gpu to cpu
- send all gesture instructions
- Some shaders may perform well enough on CPU that GPU is not required. Perf testing during development will determine this.
