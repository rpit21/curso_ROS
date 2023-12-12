#include <ros/ros.h>
#include <wiringPi.h>	
#include <geometry_msgs/Twist.h>
#include <std_msgs/Byte.h>
#include <iostream>
#include <sstream>

#define BL  26
#define BLL 19
#define L   13
#define FLL 6
#define FL  5
#define FR  1
#define FRR 12
#define R   16
#define BRR 20
#define BR  21

ros::Publisher vel_pub;
int modo;
bool FC[10];

float linear_sensores;
float angular_sensores;
float linear_control;
float angular_control;

void cmd_vel_sensores_cb(const geometry_msgs::Twist::ConstPtr& msg){
  linear_sensores = msg->linear.x;
  angular_sensores = msg->angular.z;
}

void cmd_vel_control_cb(const geometry_msgs::Twist::ConstPtr& msg){
  linear_control = msg->linear.x;
  angular_control = msg->angular.z;
}

void modo_cb(const std_msgs::Byte::ConstPtr& msg){
  if(modo != 5){
    modo = msg->data;
  }
  //std::cout<<"MODO:"<<modo<<"\n";
}

void isr(void){
  modo = 5;
  geometry_msgs::Twist twist;
  twist.angular.z = 0.0;
  twist.linear.x = 0.0;
  vel_pub.publish(twist);
  ros::spinOnce();
  ROS_INFO("Pausa de 1 segundo por fin de carrera activado");
  ros::Duration(1.0).sleep();
  ROS_INFO("Fin pausa por fin de carrera activado");
  modo = 0;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "sensors_node");
  ros::NodeHandle  nh;
  
  wiringPiSetupGpio();
	
  pinMode(BL,INPUT);
  pinMode(BLL,INPUT);
  pinMode(L,INPUT);
  pinMode(FLL,INPUT);
  pinMode(FL,INPUT);
  pinMode(FR,INPUT);
  pinMode(FRR,INPUT);
  pinMode(R,INPUT);
  pinMode(BR,INPUT);
  pinMode(BRR,INPUT);
  
  pullUpDnControl(BL, PUD_UP);
  pullUpDnControl(BLL, PUD_UP);
  pullUpDnControl(L, PUD_UP);
  pullUpDnControl(FLL, PUD_UP);
  pullUpDnControl(FL, PUD_UP);
  pullUpDnControl(FR, PUD_UP);
  pullUpDnControl(FRR, PUD_UP);
  pullUpDnControl(R, PUD_UP);
  pullUpDnControl(BRR, PUD_UP);
  pullUpDnControl(BR, PUD_UP);

  ros::Subscriber cmd_vel_sensores_sub = nh.subscribe("/cmd_vel_sensores",10,cmd_vel_sensores_cb);
  ros::Subscriber cmd_vel_control_sub = nh.subscribe("/cmd_vel_control",10,cmd_vel_control_cb);
  ros::Subscriber modo_sub = nh.subscribe("/modo",10,modo_cb);
  vel_pub =nh.advertise<geometry_msgs::Twist>("/cmd_vel", 20);
  
  ros::Rate loop_rate(100);
  
  while (ros::ok()){
    geometry_msgs::Twist cmd_vel_msg;
    
    FC[0] = digitalRead(BL);
    FC[1] = digitalRead(BLL);
    FC[2] = digitalRead(L);
    FC[3] = digitalRead(FLL);
    FC[4] = digitalRead(FL);
    FC[5] = digitalRead(FR);
    FC[6] = digitalRead(FRR);
    FC[7] = digitalRead(R);
    FC[8] = digitalRead(BRR);
    FC[9] = digitalRead(BR);
    
    //std::cout<<"FC:"<<FC[0]<<FC[1]<<FC[2]<<FC[3]<<FC[4]<<FC[5]<<FC[6]<<FC[7]<<FC[8]<<FC[9]<<"\n";
    
    if(!FC[0]||!FC[1]||!FC[2]||!FC[3]||!FC[4]||!FC[5]||!FC[6]||!FC[7]||!FC[8]||!FC[9]){
      isr();
    }
    else if(modo==0){
      cmd_vel_msg.angular.z = angular_control;
      cmd_vel_msg.linear.x = linear_control;
      vel_pub.publish(cmd_vel_msg);
      ros::spinOnce();  
    }
    else{
      cmd_vel_msg.angular.z = angular_sensores;
      cmd_vel_msg.linear.x = linear_sensores;
      vel_pub.publish(cmd_vel_msg);
      ros::spinOnce();
    }
    loop_rate.sleep();
  }
}
