#include <Dynamixel2Arduino.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>

#define DXL_SERIAL Serial1
#define DXL_DIR_PIN  -1
#define DXL_IDL 1
#define DXL_IDR 2

float VL;
float VR;

const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

ros::NodeHandle  nh;

void dxl_cb(const geometry_msgs::Twist &msg){
  float VL = (msg.linear.x*10.0 - msg.angular.z*50.0/1.8) / 2;
  float VR = (msg.linear.x*10.0 + msg.angular.z*50.0/1.8) / 2;
  if(VL > 50.0){
    VL=50.0;
  }
  else if(VL < -50.0){
    VL=-50.0;
  }
  if(VR > 50.0){
    VR=50.0;
  }
  else if(VR < -50.0){
    VR=-50.0;
  }
  dxl.setGoalVelocity(DXL_IDL, VL, UNIT_RPM);
  dxl.setGoalVelocity(DXL_IDR, VR, UNIT_RPM); 
}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &dxl_cb);

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

}

void loop() {
  nh.spinOnce();
  delay(1); 
}
