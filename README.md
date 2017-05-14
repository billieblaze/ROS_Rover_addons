sudo apt-get install ros-indigo-rosserial
sudo apt-get install ros-indigo-rosserial-python
sudo apt-get install ros-indigo-rosserial-arduino
sudo apt-get install arduino-mk

cd ~/arduino/libraries
rm -rf ros_lib
rosrun rosserial_arduino make_libraries.py .
sudo cp -r  ~/arduino/libraries/ros_lib /usr/share/arduino/libraries 