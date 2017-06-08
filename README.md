# ROS Rover Addons
Using ROS on my NAVIO2 board to create custom sensor and motor interfaces that feed into a python app (flask / celery / redis). This will feed a websocket enabled webapp in realtime to perform mapping functions.

## Installation

```
# enable ROS on navio board
echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc

# setup afp share - https://www.raspberrypi.org/forums/viewtopic.php?f=36&t=26826

#install deps
sudo apt-get install -y ros-indigo-rosserial ros-indigo-rosserial-python ros-indigo-rosserial-arduino arduino-mk nginx
sudo apt-get install -y build-essential python-dev python-distlib python-setuptools python-pip python-wheel libzmq-dev libgdal-dev
sudo apt-get install -y xsel xclip libxml2-dev libxslt-dev python-lxml python-h5py python-numexpr python-dateutil python-six python-tz python-bs4 python-html5lib python-openpyxl python-tables python-xlrd python-xlwt cython python-sqlalchemy python-xlsxwriter python-jinja2 python-boto python-gflags python-googleapi python-httplib2 python-zmq libspatialindex-dev
sudo pip install bottleneck rtree
sudo apt-get install -y python-numpy python-matplotlib python-mpltoolkits.basemap python-scipy python-sklearn python-statsmodels python-pandas
sudo apt-get install -y ros-indigo-rosbridge-server
# install portAudio 
sudo apt-get install -y libasound-dev
cd ~ 
wget http://www.portaudio.com/archives/pa_stable_v190600_20161030.tgz
tar -zxf pa_stable_v190600_20161030.tgz
cd portaudio
./configure --without-jack --without-oss --with-alsa
make && make install

#install mqtt, then build the mqtt_bridge(https://github.com/groove-x/mqtt_bridge)
sudo apt-get install mosquitto mosquitto-clients
git clone https://github.com/groove-x/mqtt_bridge ~/ROS_Rover_addons/catkin_ws/src/mqtt_bridge
cd ~/ROS_Rover_addons/catkin_ws/src/mqtt_bridge
sudo pip install -r requirements.txt  
sudo apt-get install ros-indigo-rosbridge-server
cmake install .

# clone the ROS workspace to /home/pi folder
git clone https://github.com/billieblaze/ROS_Rover_addons.git
#ln -s ~/ROS_Rover_addons/catkin_ws/ ./catkin_ws
cd ~/ROS_Rover_addons/src/rover
sudo pip install -r rquirements.txt 

cd ~/ROS_rover_addons/arduino/libraries
rm -rf ros_lib
rosrun rosserial_arduino make_libraries.py .
sudo cp -r  ~/arduino/libraries/ros_lib /usr/share/arduino/libraries 


```


## todo
* start on boot: https://github.com/TurtleBot-Mfg/ros-system-daemon-groovy
