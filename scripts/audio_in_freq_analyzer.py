#!/usr/bin/env python
import rospy
import json
import time

import model.fft
from model.sw_hear import SWHear
import numpy as np

from std_msgs.msg import String, Bool, Int16
from sensor_msgs.msg import NavSatFix

current_location = NavSatFix()

pub_details = rospy.Publisher('/sensor/audio', String, queue_size=10)

ear=SWHear()

def set_location(data):
    #rospy.loginfo(data)
    current_location = data

def listener():
    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('audio_in', anonymous=True)
    rospy.Subscriber("/mavros/global_position/global", NavSatFix, set_location)

    t1=0
    plot_sec = .25
    freq = 0
    last_freq = 0

    while True:
        ear.tape_add()
        if (time.time()-t1)>plot_sec:
            t1=time.time()
            #ear.tape_plot()
            print(ear.get_tape())

            tape = ear.get_tape()
            rate = 44100
            samples = np.arange(len(tape))/rate,tape
            #print samples
            #freq = fft.freq_from_fft(samples, rate)
            freq = fft.freq_from_fft(np.nan_to_num(tape), rate)
            print '%f Hz'   %  freq

            if freq != last_freq:
                pub_details.publish(json.dumps({"location": {"latitude": current_location.latitude, "longitude": current_location.longitude, "altitude": current_location.altitude},  "sensor": freq}))
                last_freq = freq


    # ear.tape_forever()
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
    ear.close()
    print("DONE")


if __name__ == '__main__':
    listener()
