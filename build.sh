#! /bin/bash
cd ~/home/pi/ROS_Rover_addons/catkin_ws
catkin_make
catkin_make install
source ~/ROS_Rover_addons/catkin_ws/devel/setup.bash
