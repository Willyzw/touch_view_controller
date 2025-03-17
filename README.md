# RViz Touchscreen Plugin
This package provides a custom RViz view controller that extends the standard ThirdPersonFollower view controller to support touchscreen pinch-to-zoom gestures. This plugin was created with support from Claude 3.7 Sonnet.

## Features
- All standard ThirdPersonFollower features
- Touchscreen pinch-to-zoom support
- Compatible with ROS Melodic

## Installation
1. Clone this repository into your catkin workspace's src directory:
   ```bash
   cd ~/catkin_ws/src
   git clone https://github.com/Willyzw/touch_view_controller.git
   ```
2. Build the package:
   ```bash
   cd ~/catkin_ws
   catkin_make
   ```
3. Source your workspace:
   ```bash
   source ~/catkin_ws/devel/setup.bash
   ```

## Usage
1. Launch RViz:
2. Select "TouchscreenThirdPersonFollower" from the Views panel dropdown.
3. Use pinch gestures on your touchscreen to zoom in and out.

## Contributing
Contributions are welcome! Please feel free to submit a Pull Request.
