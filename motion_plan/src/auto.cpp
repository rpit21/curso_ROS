#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

ros::Publisher vel_pub;
bool aux = 0;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "auto");
  ros::NodeHandle  nh;
 
  vel_pub =nh.advertise<geometry_msgs::Twist>("/cmd_vel_control", 10);
  ros::Rate loop_rate(0.2);
  
  while (ros::ok()){
    geometry_msgs::Twist twist;
    twist.linear.x = 0.0;
    if (aux == 0){
      twist.angular.z = 1.0;
      aux = 1;
    }
    else{
      twist.angular.z = -1.0;
      aux = 0;
    }
    vel_pub.publish(twist);
    ros::spinOnce();
    loop_rate.sleep();
  }
  
}
