#!/usr/bin/env python
import rospy
import json
from std_msgs.msg import String, Bool, Int16
from sensor_msgs.msg import NavSatFix, Range

current_location = NavSatFix()
pub_details = rospy.Publisher('/range_location', String, queue_size=10)

def chatter(data):
    rospy.loginfo(rospy.get_caller_id() + "ping %s", data)

def range_message(data):
    rospy.loginfo(rospy.get_caller_id() + "range sensor data %s", data)
    rospy.loginfo(data)
    pub_details.publish(json.dumps({"location": {"latitude": current_location.latitude, "longitude": current_location.longitude, "altitude": current_location.altitude},  "message": { "range": data.range, "direction": data.header.frame_id}}))

def set_location(data):
    rospy.loginfo(data)
    current_location = data

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
