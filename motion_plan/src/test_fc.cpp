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

void cmd_vel_sensores_cb(const geometry_msgs::Twist &msg){
  if((modo != 0) && (modo != 5)){
    geometry_msgs::Twist twist;
    twist.angular.z = msg.angular.z;
    twist.linear.x = msg.linear.x;
    vel_pub.publish(twist);
  }
}

void cmd_vel_control_cb(const geometry_msgs::Twist &msg){
  if(modo == 0){
    geometry_msgs::Twist twist;
    twist.angular.z = msg.angular.z;
    twist.linear.x = msg.linear.x;
    vel_pub.publish(twist);
  }
	
}

void modo_cb(const std_msgs::Byte &msg){
  if(modo != 5){
    modo = msg.data;
  }
}

void isr(void){
  modo = 5;
  geometry_msgs::Twist twist;
  twist.angular.z = 0.0;
  twist.linear.x = 0.0;
  vel_pub.publish(twist);
  ros::spinOnce();
  ROS_INFO("Pausa de 1 segundo por fin de carrera activado.");
  ros::Duration(1.0).sleep();
  ROS_INFO("Pausa finalizada.");
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
  /*
  wiringPiISR (BL, INT_EDGE_FALLING, &isr);
  wiringPiISR (BLL, INT_EDGE_FALLING, &isr);
  wiringPiISR (L, INT_EDGE_FALLING, &isr);
  wiringPiISR (FLL, INT_EDGE_FALLING, &isr);
  wiringPiISR (FL, INT_EDGE_FALLING, &isr);
  wiringPiISR (FR, INT_EDGE_FALLING, &isr);
  wiringPiISR (FRR, INT_EDGE_FALLING, &isr);
  wiringPiISR (R, INT_EDGE_FALLING, &isr);
  wiringPiISR (BRR, INT_EDGE_FALLING, &isr);
  wiringPiISR (BR, INT_EDGE_FALLING, &isr);
  */  
  ros::Subscriber cmd_vel_sensores_sub = nh.subscribe("/cmd_vel_sensores",10,cmd_vel_sensores_cb);
  ros::Subscriber cmd_vel_control_sub = nh.subscribe("/cmd_vel_control",10,cmd_vel_control_cb);
  ros::Subscriber modo_sub = nh.subscribe("/modo",10,modo_cb);
  vel_pub =nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  
  ros::Rate loop_rate(100);
  
  while (ros::ok()){
    /*
    if(!digitalRead(BL)){
			isr();
		}
    else if(!digitalRead(BLL)){
      isr();
    }
    else if(!digitalRead(L)){
      isr();
    }
    else if(!digitalRead(FLL)){
      isr();
    }
    else if(!digitalRead(FL)){
      isr();
    }
    else if(!digitalRead(FR)){
      isr();
    }
    else if(!digitalRead(FRR)){
      isr();
    }
    else if(!digitalRead(R)){
      isr();
    }
    else if(!digitalRead(BRR)){
      isr();
    }
    else if(!digitalRead(BR)){
      isr();
    }
    */
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
    
    std::cout<<"FC:"<<FC[0]<<FC[1]<<FC[2]<<FC[3]<<FC[4]<<FC[5]<<FC[6]<<FC[7]<<FC[8]<<FC[9]<<"\n";
    
    if(!FC[0]||!FC[1]||!FC[2]||!FC[3]||!FC[4]||!FC[5]||!FC[6]||!FC[7]||!FC[8]||!FC[9]){
      isr();
    }
    
    ros::spinOnce();
    loop_rate.sleep();
  }
}
