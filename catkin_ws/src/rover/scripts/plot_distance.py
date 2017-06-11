#!/usr/bin/env python
import rospy
import json
import math
from std_msgs.msg import String, Bool, Int16
from sensor_msgs.msg import Range
import matplotlib.pyplot as plt

time = []
distance_left = []
distance_front = []
distance_right = []

angle = 15

circle_max = plt.Circle((100,100),100, color='b', fill=False)
circle_min = plt.Circle((100,100),50, color='b', fill=False)

fig, ax = plt.subplots()
home = (100,100)
ax.add_artist(circle_max)
ax.add_artist(circle_min)
#ax.axis('off')
ax.spines['left'].set_position('center')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('center')
ax.spines['top'].set_color('none')
ax.spines['left'].set_smart_bounds(True)
ax.spines['bottom'].set_smart_bounds(True)
#ax.xaxis.set_ticks_position('bottom')
#ax.yaxis.set_ticks_position('left')

def plot_point(point, angle, length):
    '''
     point - Tuple (x, y)
     angle - Angle you want your end point at in degrees.
     length - Length of the line you want to plot.

     Will plot the line on a 10 x 10 plot.
     '''

     # unpack the first point
    x, y = point
    
    # give it a little room for the min sensor distance 
    lenth = length + 10 

    # find the end point
    endy = length * math.sin(math.radians(angle))
    endx = length * math.cos(math.radians(angle))


    # plot the points
    ax.set_ylim([0, 200])   # set the bounds to be 10, 10
    ax.set_xlim([0, 200])
    ax.plot([x, 100-endx], [y, 100-endy])

    fig.savefig('/var/www/html/plot.png')


def range_message(data):
    # #rospy.loginfo(data)
    if data.header.frame_id == '/fl':
        rospy.loginfo('fl: %s' % data.range)
        plot_point(home, 230, data.range)
        
    if data.header.frame_id == '/ff':
        rospy.loginfo('ff: %s' % data.range)
        plot_point(home, 270, data.range)

    if data.header.frame_id == '/fr':
        rospy.loginfo('fr: %s' % data.range)
        plot_point(home, 310, data.range)


def listener():
    rospy.init_node('plot_distance', anonymous=True)
    rospy.Subscriber("/range_data", Range, range_message)
    rospy.spin()

if __name__ == '__main__':
    listener()
