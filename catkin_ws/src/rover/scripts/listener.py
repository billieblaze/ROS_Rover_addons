#!/usr/bin/env python
import rospy
import json 
from std_msgs.msg import String, Bool, Int16
from sensor_msgs.msg import NavSatFix

current_location = NavSatFix()
pub_details = rospy.Publisher('/sensor/details', String, queue_size=10)

def chatter(data):
    rospy.loginfo(rospy.get_caller_id() + "ping %s", data)

def sensor(data):
    rospy.loginfo(rospy.get_caller_id() + "sensor data %s", data)
    #rospy.loginfo(current_location)
    pub_details.publish(json.dumps({"location": {"latitude": current_location.latitude, "longitude": current_location.longitude, "altitude": current_location.altitude},  "sensor": data.data}))

def distance(data):
    rospy.loginfo(rospy.get_caller_id() + "distance sensor data %s", data)
    #rospy.loginfo(current_location)
    #pub_details.publish(json.dumps({"location": {"latitude": current_location.latitude, "longitude": current_location.longitude, "altitude": current_location.altitude},  "sensor": data.data}))

def location(data):
    #rospy.loginfo(data)
    current_location = data
    

    
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("chatter", String, chatter)
    rospy.Subscriber("/sensor/hit", Bool, sensor)
    rospy.Subscriber("/sensor/distance", Int16, distance)
    rospy.Subscriber("/mavros/global_position/global", NavSatFix, location)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()