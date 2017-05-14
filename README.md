# ROS Rover Addons
Using ROS on my NAVIO2 board to create custom sensor and motor interfaces that feed into a python app (flask / celery / redis). This will feed a websocket enabled webapp in realtime to perform mapping functions.

## Installation

```
git clone https://github.com/billieblaze/ROS_Rover_addons.git
ln -s ~/ROS_Rover_addons/catkin_ws/ ./catkin_ws

sudo apt-get install ros-indigo-rosserial
sudo apt-get install ros-indigo-rosserial-python
sudo apt-get install ros-indigo-rosserial-arduino
sudo apt-get install arduino-mk

cd ~/ROS_rover_addons/arduino/libraries
rm -rf ros_lib
rosrun rosserial_arduino make_libraries.py .
sudo cp -r  ~/arduino/libraries/ros_lib /usr/share/arduino/libraries 
```


## todo
* start on boot: https://github.com/TurtleBot-Mfg/ros-system-daemon-groovy