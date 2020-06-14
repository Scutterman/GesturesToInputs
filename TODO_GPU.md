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
