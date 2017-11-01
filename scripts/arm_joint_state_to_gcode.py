#!/usr/bin/env python
#  translation of 5 axis joint state to gcode for modified marlin firmware running on bcn3d moveo arm.

import rospy
from std_msgs.msg import Int16, Header
from sensor_msgs.msg import JointState
import serial
import time
import numpy as np

s = serial.Serial('/dev/ttyACM0',250000)
print 'Opening Serial Port - Success: {}'.format(s.is_open)

# gcode_command = 'T1\nG0 E2 F50\n'
# print(gcode_command)
# s.write( gcode_command)

def joint_handler(data):
    print(data)
    joint_states = np.column_stack((data.name, data.position))

    # Stream g-code
    for joint in joint_states:
        joint_axis = ""
        command_prefix = ""

        joint_link = joint[0]
        joint_value=round(float(joint[1]) * 3.14,3)
        #print("{} - {}".format(joint_link, joint_value))
        if joint_link == 'arm_shoulder_joint':
            joint_radius = 90
            X_value = joint_value

        elif joint_link == 'arm_upper_joint':
            joint_radius = 230
            Y_value = joint_value

        elif joint_link == 'arm_elbow_joint':
            joint_radius = 230
            Z_value = joint_value

        elif joint_link == 'arm_wrist_rotate_joint':
            joint_radius = 40
            E0_value = joint_value

        elif joint_link == 'arm_wrist_tilt_joint':
            joint_radius = 40
            E1_value=joint_value


    s.write('T0\n')
    gcode_command = 'G0 X{} Y{} Z{} E{} F100\n'.format(X_value, Y_value, Z_value, E0_value)
    print(gcode_command)
    s.write( gcode_command)
    # print s.read()
    s.write( 'T1\n')
    # print s.read()
    s.write( 'G0 E{} F100\n'.format(E1_value))
    # print s.read()
    return

def listener():
    # Wake up
    s.write("\r\n\r\n") # Hit enter a few times to wake the bot
    time.sleep(2)   # Wait for bot to initialize
    s.flushInput()  # Flush startup text in serial input
    s.write('G90\n')

    s.write('G92 X0 Y0 Z0\n')
    s.write('M82\n')
    s.write('M121\n')

    rospy.init_node('arm', anonymous=True)
    rate = rospy.Rate(10) # 100hz

    #rospy.Subscriber("/move_group/fake_controller_joint_states", JointState, joint_handler)
    print 'handle it'

    rospy.Subscriber("/joint_states", JointState, joint_handler)

    while not rospy.is_shutdown():
        #rate.sleep()
        rospy.spin()

    # SHUTDOWN
    # Close serial port
    #s.close()

if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
