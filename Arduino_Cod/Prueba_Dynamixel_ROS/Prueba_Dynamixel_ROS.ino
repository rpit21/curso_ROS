#include <Dynamixel2Arduino.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Empty.h>

#define DXL_SERIAL Serial1
#define DXL_DIR_PIN  -1
#define DXL_IDL 2
#define DXL_IDR 1

float VL;
float VR;

const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

ros::NodeHandle  nh;

void dxl_cb(const geometry_msgs::Twist &msg){
  float VL = msg.linear.x*182.0 - msg.angular.z*28.0;
  float VR = msg.linear.x*182.0 + msg.angular.z*28.0;
  if(VL > 40.0){
    VL=40.0;
  }
  else if(VL < -40.0){
    VL=-40.0;
  }
  if(VR > 40.0){
    VR=40.0;
  }
  else if(VR < -40.0){
    VR=-40.0;
  }
  dxl.setGoalVelocity(DXL_IDL, VL, UNIT_RPM);
  dxl.setGoalVelocity(DXL_IDR, -VR, UNIT_RPM); 
}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &dxl_cb);

std_msgs::Int32 current_L;
std_msgs::Int32 current_R;
std_msgs::Float32 vel_L;
std_msgs::Float32 vel_R;
ros::Publisher current_L_pub("/current_L", &current_L);
ros::Publisher current_R_pub("/current_R", &current_R);
ros::Publisher vel_L_pub("/vel_L", &vel_L);
ros::Publisher vel_R_pub("/vel_R", &vel_R);

void setup() {
  Serial.begin(115200);
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  
  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxl.ping(DXL_IDL);
  dxl.ping(DXL_IDR);
  
  dxl.torqueOff(DXL_IDL);
  dxl.setOperatingMode(DXL_IDL,  OP_VELOCITY);
  dxl.torqueOn(DXL_IDL);
  dxl.torqueOff(DXL_IDR);
  dxl.setOperatingMode(DXL_IDR,  OP_VELOCITY);
  dxl.torqueOn(DXL_IDR);

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(current_L_pub);
  nh.advertise(current_R_pub);
  nh.advertise(vel_L_pub);
  nh.advertise(vel_R_pub);
}

void loop() {
  current_L.data = dxl.getPresentCurrent(DXL_IDL, UNIT_MILLI_AMPERE);
  current_R.data = dxl.getPresentCurrent(DXL_IDR, UNIT_MILLI_AMPERE);
  vel_L.data = dxl.getPresentVelocity(DXL_IDL, UNIT_RPM);
  vel_R.data = -dxl.getPresentVelocity(DXL_IDR, UNIT_RPM);
  current_L_pub.publish( &current_L );
  nh.spinOnce();
  current_R_pub.publish( &current_R );
  nh.spinOnce();
  vel_L_pub.publish( &vel_L );
  nh.spinOnce();
  vel_R_pub.publish( &vel_R );
  nh.spinOnce();
  delay(100); 
}
