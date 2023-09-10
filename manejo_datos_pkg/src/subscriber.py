#!/usr/bin/env python 3

import rospy
from sensor_msgs.msg import LaserScan 
#from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

anguloActual=0
rangoActual=0
intensidad=0

def callback(data:LaserScan):
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.header)
    lens= int((data.angle_max - data.angle_min) / data.angle_increment)

    for i in range(0,lens):
        anguloActual=(data.angle_min + i * data.angle_increment)*180/3.14159
        rangoActual=data.ranges[i]
        intensidad=data.intensities[i]
        rospy.loginfo("Angulo: %f", anguloActual)
        rospy.loginfo("Rango: %f", rangoActual)
        rospy.loginfo("Intesidad: %f", intensidad)
    
   
def listener():
    rospy.Subscriber("scan", LaserScan, callback)
    rospy.spin()

def talker():
    pub = rospy.Publisher('Evasion', Float32, queue_size=10)
    rate = rospy.Rate(10) # 10hz
    num=0
    while not rospy.is_shutdown():
        num= num + 1.5
        rospy.loginfo(num) # out screeen
        pub.publish(num)
        rate.sleep()

def tratamiento():
    anguloActual

if __name__ == '__main__':
    rospy.init_node('LectorLidar', anonymous=False)
    listener()
