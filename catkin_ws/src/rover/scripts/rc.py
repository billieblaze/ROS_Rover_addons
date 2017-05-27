#!/usr/bin/env python
#  custom rc stuff 
import rospy
from std_msgs.msg import String
from mavros_msgs.msg import RCIn
from mavros_msgs.srv import *

last_rc_data = []

def rc_handler(data): 
    global last_rc_data

    if last_rc_data != data:
        
        if data.channels[9] <  1100:
            print('arm')
            armService = rospy.ServiceProxy('/mavros/cmd/arming', mavros_msgs.srv.CommandBool)
            armService(True)
        else:
            print('disarm')
            armService = rospy.ServiceProxy('/mavros/cmd/arming', mavros_msgs.srv.CommandBool)
            armService(False)

        last_rc_data = data.channels

def listener():
    
    rospy.init_node('rc', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    rospy.Subscriber("/mavros/rc/in", RCIn, rc_handler)

    while not rospy.is_shutdown():
        
        rate.sleep()

if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
