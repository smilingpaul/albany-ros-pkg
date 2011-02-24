#!/usr/bin/env python

""" Simple Arm Test for Nelson
    Voodoo control of right arm, using left arm.
    Copyright 2010 Michael E. Ferguson """

import roslib; roslib.load_manifest('nelson')
import rospy

from sensor_msgs.msg import JointState
from std_msgs.msg import Float64
from arbotix_msgs.srv import *

mapping = { "l_shoulder_lift_joint":"r_shoulder_lift_joint", "l_shoulder_pan_joint":"r_shoulder_pan_joint", "l_elbow_flex_joint":"r_elbow_flex_joint", "l_gripper_joint":"r_gripper_joint" }
outputs = dict()
relax = dict()
values = dict()
pubs = dict()

def moveArms( msg ):
    """ We use this callback to update the arm output positions every time 
        the joint_states topic is published to. The actual output is done 
        within the main loop. """
    for read in mapping.keys():
        write = mapping[read] 
        index = msg.name.index(read)
        values[write] = msg.position[index]

def cleanup():
    """ We use this callback to relax all servos when our node exits. """
    for servo in relax.keys():
        relax[servo]()

if __name__ == '__main__':
    # initialize and listen to joint_states topic
    rospy.init_node('arm_test')
    rospy.Subscriber('joint_states', JointState, moveArms)
    for joint in mapping.values():
        pubs[joint] = rospy.Publisher(joint+'/command', Float64)
    
    # this is so we can cleanup by relaxing arms when we exit
    for servo in mapping.values():
        relax[servo] = rospy.ServiceProxy(servo+'/relax',Relax) 
    for servo in mapping.keys():       
        relax[servo] = rospy.ServiceProxy(servo+'/relax',Relax) 
        relax[servo]()
    rospy.on_shutdown(cleanup)

    # logging information is a good idea
    rospy.loginfo('arm_test.py initialized')

    # Let this run at approximately 15hz
    # rospy.Rate handles all timing for us
    r = rospy.Rate(15)
    while not rospy.is_shutdown():
        for joint in values.keys():
            msg = Float64(values[joint])
            pubs[joint].publish(msg)
        r.sleep()

