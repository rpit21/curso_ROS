#include <ros/ros.h>
#include <wiringPi.h>	
#include <std_msgs/Byte.h>
#include <std_msgs/Int32.h>
#include <iostream>
#include <sstream>

uint8_t Bat[100];
int16_t Current_L[10];
int16_t Current_R[10];
int32_t Vel_L;
int32_t Vel_R;
int current_L;
int current_R;
int promBat = 0;
int maxCurrent_L = 0;
int maxCurrent_R = 0;
int promCurrent_L = 0;
int promCurrent_R = 0;

void bat_cb(const std_msgs::Byte::ConstPtr& msg){
  for (uint8_t i = 100; i > 0; i--){Bat[i] = Bat[i-1];}
  Bat[0] = msg->data;
  int batSum = 0;
  for (uint8_t i = 0; i < 100; i++){batSum += Bat[i];}
  promBat = batSum/100;
}
void current_L_cb(const std_msgs::Int32::ConstPtr& msg){
  for (uint8_t i = 10; i > 0; i--){Current_L[i] = Current_L[i-1];}
  Current_L[0] = msg->data;
  current_L = msg->data;
  if(msg->data > maxCurrent_L){maxCurrent_L = msg->data;}
  int currentL_Sum = 0;
  for (uint8_t i = 0; i < 10; i++){currentL_Sum += Current_L[i];}
  promCurrent_L = currentL_Sum/10;
}
void current_R_cb(const std_msgs::Int32::ConstPtr& msg){
  for (uint8_t i = 10; i > 0; i--){Current_R[i] = Current_R[i-1];}
  Current_R[0] = msg->data;
  current_R = msg->data;
  if(msg->data > maxCurrent_R){maxCurrent_R = msg->data;}
  int currentR_Sum = 0;
  for (uint8_t i = 0; i < 10; i++){currentR_Sum += Current_R[i];}
  promCurrent_R = currentR_Sum/10;
}
void vel_L_cb(const std_msgs::Int32::ConstPtr& msg){
  Vel_L = msg->data;
}
void vel_R_cb(const std_msgs::Int32::ConstPtr& msg){
  Vel_R = msg->data;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "datos");
  ros::NodeHandle  nh;

  ros::Subscriber bat_sub = nh.subscribe("/bat",10,bat_cb);
  ros::Subscriber current_L_sub = nh.subscribe("/current_L",10,current_L_cb);
  ros::Subscriber current_R_sub = nh.subscribe("/current_R",10,current_R_cb);
  ros::Subscriber vel_L_sub = nh.subscribe("/vel_L",10,vel_L_cb);
  ros::Subscriber vel_R_sub = nh.subscribe("/vel_R",10,vel_R_cb);
  
  ros::Rate loop_rate(10);
  
  for (uint8_t i = 1; i < 100; i++){Bat[i] = 100;}
  
  while (ros::ok()){
    std::cout<<"Bat:"<<promBat<<"\n";
    std::cout<<"Current L:"<<current_L<<"\t"<<"Current R:"<<current_R<<"\t"<<promCurrent_L<<"\t"<<promCurrent_R<<"\n";
    //std::cout<<"Vel L:"<<Vel_L<<" Vel_R: "<<Vel_R<<"\n";
    ros::spinOnce();
    loop_rate.sleep();
  }
}
