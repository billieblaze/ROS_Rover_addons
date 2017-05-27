#!/usr/bin/env python
import rospy
import json
from std_msgs.msg import String, Bool, Int16
from sensor_msgs.msg import NavSatFix, Range
from rover.msg import RangeLocation

current_location = NavSatFix()
pub_range = rospy.Publisher('/range_location', RangeLocation, queue_size=10)
pub_location = rospy.Publisher('/current_location', NavSatFix, queue_size=10)

def chatter(data):
    rospy.loginfo(rospy.get_caller_id() + "ping %s", data)

def range_message(data):
    #rospy.loginfo(rospy.get_caller_id() + "range sensor data %s", data)
    #rospy.loginfo(data)
    msg = RangeLocation()
    msg.location = current_location
    msg.range = data
    pub_range.publish(msg)


def set_location(data):
    #rospy.loginfo(data)
    current_location = data
    pub_location.publish(data)

def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("chatter", String, chatter)
    rospy.Subscriber("/range_data", Range, range_message)
    rospy.Subscriber("/mavros/global_position/global", NavSatFix, set_location)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
