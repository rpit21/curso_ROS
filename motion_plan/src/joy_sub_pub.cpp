#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <iostream>

ros::Publisher vel_pub;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
  geometry_msgs::Twist twist;
  twist.angular.z = joy->axes[3];
  twist.linear.x = joy->axes[1];
  vel_pub.publish(twist);
  //std::cout<<"velocidad "<<twist.angular.z <<"  "<<twist.linear.x<<std::endl;
	
	}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "joy_sub_pub");
  ros::NodeHandle  nh;
  ros::Subscriber sub = nh.subscribe("joy",10,joyCallback);
  vel_pub =nh.advertise<geometry_msgs::Twist>("/cmd_vel_control", 10);

  ros::spin();
}
