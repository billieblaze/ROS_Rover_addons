execute_process(COMMAND "/home/pi/ROS_Rover_addons/catkin_ws/build/mqtt_bridge/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/pi/ROS_Rover_addons/catkin_ws/build/mqtt_bridge/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
