/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <DynamixelShield.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
  #define DEBUG_SERIAL soft_serial
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAM_ZERO)
  #define DEBUG_SERIAL SerialUSB    
#else
  #define DEBUG_SERIAL Serial
#endif

const uint8_t DXLL_ID = 1;
const uint8_t DXLR_ID = 2;
const float DXL_PROTOCOL_VERSION = 2.0;

DynamixelShield dxlL;
DynamixelShield dxlR;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup() {
  // put your setup code here, to run once:
  
  // For Uno, Nano, Mini, and Mega, use UART port of DYNAMIXEL Shield to debug.
  //DEBUG_SERIAL.begin(115200);
  Serial.begin(115200);
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxlL.begin(57600);
  dxlR.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxlL.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxlR.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxlL.ping(DXLL_ID);
  dxlR.ping(DXLR_ID);
  // Turn off torque when configuring items in EEPROM area
  dxlL.torqueOff(DXLL_ID);
  dxlL.setOperatingMode(DXLL_ID, OP_POSITION);
  dxlL.torqueOn(DXLL_ID);
  dxlR.torqueOff(DXLR_ID);
  dxlR.setOperatingMode(DXLR_ID, OP_POSITION);
  dxlR.torqueOn(DXLR_ID);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Please refer to e-Manual(http://emanual.robotis.com/docs/en/parts/interface/dynamixel_shield/) for available range of value. 
  // Set Goal Position in RAW value
  dxlL.setGoalPosition(DXLL_ID, 4095);
  dxlR.setGoalPosition(DXLR_ID, 0);
  delay(2000);
  // Print present position in raw value
  /*DEBUG_SERIAL.print("Present Position(raw) : ");
  DEBUG_SERIAL.println(dxl.getPresentPosition(DXL_ID));
  delay(1000);*/
  Serial.print("Present Position(raw) : ");
  Serial.println(dxlL.getPresentPosition(DXLL_ID));
  // Set Goal Position in DEGREE value
  dxlL.setGoalPosition(DXLL_ID, 0, UNIT_DEGREE);
  dxlR.setGoalPosition(DXLR_ID, 360, UNIT_DEGREE);
  delay(1000);
  dxlL.setGoalPosition(DXLL_ID, 180, UNIT_DEGREE);
  dxlR.setGoalPosition(DXLR_ID, 180, UNIT_DEGREE);
  delay(1000);
  // Print present position in degree value
  /*DEBUG_SERIAL.print("Present Position(degree) : ");
  DEBUG_SERIAL.println(dxl.getPresentPosition(DXL_ID, UNIT_DEGREE));
  delay(1000);*/
}
