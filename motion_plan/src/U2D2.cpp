#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/Twist.h>
#include "dynamixel_sdk/dynamixel_sdk.h"

#define MAX_VEL 167.0

using namespace dynamixel;

// Control table address
#define ADDR_OPERATING_MODE   11
#define ADDR_TORQUE_ENABLE    64
#define ADDR_GOAL_POSITION    116
#define ADDR_GOAL_VELOCITY    104
#define ADDR_PRESENT_POSITION 132
#define ADDR_PRESENT_CURRENT  126
#define ADDR_PRESENT_VELOCITY 128

// Protocol version
#define PROTOCOL_VERSION      2.0             // Default Protocol version of DYNAMIXEL X series.

// Default setting
#define DXL_IDL				        2 
#define DXL_IDR				        1
#define BAUDRATE              57600           // Default Baudrate of DYNAMIXEL X series
#define DEVICE_NAME           "/dev/ttyUSB1"  // [Linux] To find assigned port, use "$ ls /dev/ttyUSB*" command

PortHandler * portHandler;
PacketHandler * packetHandler;

void dxl_cb(const geometry_msgs::Twist &msg)
{
  float VL = msg.linear.x*MAX_VEL - msg.angular.z*MAX_VEL;
  float VR = msg.linear.x*MAX_VEL + msg.angular.z*MAX_VEL;
  
  if(VL > MAX_VEL){
    VL=MAX_VEL;
  }
  else if(VL < -MAX_VEL){
    VL=-MAX_VEL;
  }
  if(VR > MAX_VEL){
    VR=MAX_VEL;
  }
  else if(VR < -MAX_VEL){
    VR=-MAX_VEL;
  }
  
  uint8_t dxl_error = 0;
  int dxl_comm_result = COMM_TX_FAIL;

  int32_t DXL_L_VEL = VL;
  int32_t DXL_R_VEL = -VR;
  
  dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_IDL, ADDR_GOAL_VELOCITY, DXL_L_VEL, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to set velocity! for DXL_L Result: %d", dxl_comm_result);
  }
  dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_IDR, ADDR_GOAL_VELOCITY, DXL_R_VEL, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to set velocity! for DXL_R Result: %d", dxl_comm_result);
  }
}

int dynamixel_config(){
	uint8_t dxl_error = 0;
  int dxl_comm_result = COMM_TX_FAIL;

  portHandler = PortHandler::getPortHandler(DEVICE_NAME);
  packetHandler = PacketHandler::getPacketHandler(PROTOCOL_VERSION);

  if (!portHandler->openPort()) {
    ROS_ERROR("Failed to open the port!");
    return -1;
  }
  if (!portHandler->setBaudRate(BAUDRATE)) {
    ROS_ERROR("Failed to set the baudrate!");
    return -1;
  }

  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_IDL, ADDR_TORQUE_ENABLE, 0, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to disable torque for Dynamixel ID %d", DXL_IDL);
    return -1;
  }
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_IDL, ADDR_OPERATING_MODE, 1, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to configure operating mode for Dynamixel ID %d", DXL_IDL);
    return -1;
  }
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_IDL, ADDR_TORQUE_ENABLE, 1, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to enable torque for Dynamixel ID %d", DXL_IDL);
    return -1;
  }

  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_IDR, ADDR_TORQUE_ENABLE, 0, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to disable torque for Dynamixel ID %d", DXL_IDR);
    return -1;
  }
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_IDR, ADDR_OPERATING_MODE, 1, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to configure operating mode for Dynamixel ID %d", DXL_IDR);
    return -1;
  }
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_IDR, ADDR_TORQUE_ENABLE, 1, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS) {
    ROS_ERROR("Failed to enable torque for Dynamixel ID %d", DXL_IDR);
    return -1;
  }
}

int main(int argc, char ** argv)
{
  dynamixel_config();

  ros::init(argc, argv, "U2D2");
  ros::NodeHandle nh;

  ros::Subscriber cmd_vel_sub = nh.subscribe("/cmd_vel", 10, &dxl_cb);
  ros::Publisher vel_L_pub =nh.advertise<std_msgs::Int32>("/vel_L", 10);
  ros::Publisher vel_R_pub =nh.advertise<std_msgs::Int32>("/vel_R", 10);
  ros::Publisher current_L_pub =nh.advertise<std_msgs::Int32>("/current_L", 10);
  ros::Publisher current_R_pub =nh.advertise<std_msgs::Int32>("/current_R", 10);
  ros::Rate loop_rate(1);
  
  while (ros::ok()){
    uint8_t dxl_error = 0;
    int dxl_comm_result = COMM_TX_FAIL;
    
    std_msgs::Int32 vel_L;
    std_msgs::Int32 vel_R;
    int32_t vel_l = 0;
    int32_t vel_r = 0;
    
    dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_IDL, ADDR_PRESENT_VELOCITY, (uint32_t *)&vel_l, &dxl_error);
    vel_L.data = 0.229*vel_l;		
    ROS_INFO("getVelocity : DXL_L -> Velocity:%d", vel_L.data);
    vel_L_pub.publish(vel_L);
    ros::spinOnce();
	
    dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, DXL_IDR, ADDR_PRESENT_VELOCITY, (uint32_t *)&vel_r, &dxl_error);
    vel_R.data = -0.229*vel_r;		
    ROS_INFO("getVelocity : DXL_R -> Velocity:%d", vel_R.data);
    vel_R_pub.publish(vel_R);
    ros::spinOnce();
    
    std_msgs::Int16 current_L;
    std_msgs::Int16 current_R;
	  int16_t current_l = 0;
    int16_t current_r = 0;
    
    dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_IDL, ADDR_PRESENT_CURRENT, (uint16_t *)&current_l, &dxl_error);
    current_L.data = 2.69*current_l;		
    ROS_INFO("getCurrnt : DXL_L -> Current:%d", current_L.data);
    current_L_pub.publish(current_L);
    ros::spinOnce();
	
    dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_IDR, ADDR_PRESENT_CURRENT, (uint16_t *)&current_r, &dxl_error);
    current_R.data = -2.69*current_r;		
    ROS_INFO("getCurrent : DXL_R -> Current:%d", current_R.data);
    current_R_pub.publish(current_R);
    ros::spinOnce();
      
    loop_rate.sleep();
  }
  portHandler->closePort();
  return 0;
}
