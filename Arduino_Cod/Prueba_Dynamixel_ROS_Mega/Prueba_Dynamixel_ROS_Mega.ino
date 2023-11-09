#include <ros.h>
#include <geometry_msgs/Twist.h>

float VL;
float VR;
int vl;
int vr;
uint8_t vll;
uint8_t vrr;
String buff;

ros::NodeHandle  nh;

void dxl_cb(const geometry_msgs::Twist &msg){
  VL = ((10.0*msg.linear.x) - (msg.angular.z*50.0/1.8)) / 2.0;
  VR = ((10.0*msg.linear.x) + (msg.angular.z*50.0/1.8)) / 2.0;
  vl=(int)VL;
  vr=(int)VR;
  if(vl > 50){
    vl=50;
  }
  else if(vl < -50){
    vl=-50.0;
  }
  if(vr > 50.0){
    vr=50;
  }
  else if(vr < -50){
    vr=-50;
  }
  vll=map(vl,-50,50,1,255);
  vrr=map(vr,-50,50,1,255);
  buff="";
  buff+=vll;
  buff+=vrr;
  Serial1.println(buff); 
  digitalWrite(13,1);
}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &dxl_cb);

void setup() {
  Serial.begin(57600);
  Serial1.begin(115200);
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.subscribe(sub);
  pinMode(13,OUTPUT);
}

void loop() {
  nh.spinOnce();
  delay(1); 
}