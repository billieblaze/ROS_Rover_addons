#!/usr/bin/env python
#  custom rc stuff
import rospy
from std_msgs.msg import Int16, Header
from sensor_msgs.msg import JointState
from mavros_msgs.msg import RCIn
from mavros_msgs.srv import *

armed = False

last_arm_position = 0;

pub_move_arm = rospy.Publisher('/arm/move', Int16, queue_size=10)
pub_arm_state = rospy.Publisher('joint_states', JointState, queue_size=10)

def rc_handler(data):
    global last_arm_position
    global armed

    #print(data)

    # CHANNEL 6 = SWEEP STEPPER MOTOR, this will follow camera movements as well so be sure to map them to this channel
    if last_arm_position != data.channels[5]:
        #SEND MESSAGE TO MOVE ARM TO POSITION
        rc_value = data.channels[5]

        pub_move_arm.publish(rc_value)
        last_arm_position = rc_value


        arm_state = JointState()
        arm_state.header = Header()
        arm_state.header.stamp = rospy.Time.now()
        arm_state.name = ['arm']
        arm_state.position = [rc_value]  #todo: convert to rads
        arm_state.velocity = []
        arm_state.effort = []
        pub_arm_state.publish(arm_state)

    # # CHANNEL 9 = ENABLE DETECTOR STEPPER MOTOR
    # if data.channels[8] and data.channels[8] <  1100:
    #     #SEND MESSAGE TO START OSCILLATION
    #     print('Enable arm: %d' % data.channels[8])

    # if data.channels[8] and data.channels[8] > 1100:
    #     print('Disable arm: %d' % data.channels[8])

    # CHANNEL 9 = ARMING
    if armed == False and len(data.channels) > 8 and data.channels[8] <  1100:
        print('arm')
        armed = True
        armService = rospy.ServiceProxy('/mavros/cmd/arming', mavros_msgs.srv.CommandBool)
        armService(True)
    
    if armed == True and len(data.channels) > 8 and data.channels[8] >  1100:
        print('disarm')
        armed = False
        armService = rospy.ServiceProxy('/mavros/cmd/arming', mavros_msgs.srv.CommandBool)
        armService(False)

    last_rc_data = data.channels

def listener():

    rospy.init_node('rc', anonymous=True)
    rate = rospy.Rate(100) # 100hz
    rospy.Subscriber("/mavros/rc/in", RCIn, rc_handler)

    while not rospy.is_shutdown():

        rate.sleep()

if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
