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

/** Please refer to each DYNAMIXEL eManual(http://emanual.robotis.com) for supported Operating Mode
 * Operating Mode
 *  1. OP_POSITION                (Position Control Mode in protocol2.0, Joint Mode in protocol1.0)
 *  2. OP_VELOCITY                (Velocity Control Mode in protocol2.0, Speed Mode in protocol1.0)
 *  3. OP_PWM                     (PWM Control Mode in protocol2.0)
 *  4. OP_EXTENDED_POSITION       (Extended Position Control Mode in protocol2.0, Multi-turn Mode(only MX series) in protocol1.0)
 *  5. OP_CURRENT                 (Current Control Mode in protocol2.0, Torque Mode(only MX64,MX106) in protocol1.0)
 *  6. OP_CURRENT_BASED_POSITION  (Current Based Postion Control Mode in protocol2.0 (except MX28, XL430))
 */

#include <DynamixelShield.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
  #define DEBUG_SERIAL Serial//soft_serial
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAM_ZERO)
  #define DEBUG_SERIAL SerialUSB    
#else
  #define DEBUG_SERIAL Serial
#endif

const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;

DynamixelShield dxl;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

int i=0;
void setup() {
  // put your setup code here, to run once:
  
  // For Uno, Nano, Mini, and Mega, use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);

  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_EXTENDED_POSITION);
  dxl.torqueOn(DXL_ID);
  dxl.torqueOff(2);
  dxl.setOperatingMode(2, OP_EXTENDED_POSITION);
  dxl.torqueOn(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Position Control Mode in protocol2.0, Joint Mode in protocol1.0
  // Turn off torque when configuring items in EEPROM area
  
  // Extended Position Contorl Mode in protocol2.0, Multi-turn Mode(only MX series) in protocol1.0
  
  /*
  for(int i=0; i==10000; i=i+100){
    if(dxl.setGoalPosition(DXL_ID, 0)){
      delay(100);
      DEBUG_SERIAL.println(dxl.getPresentPosition(DXL_ID));
    }
    Serial.println(i);
    //while(dxl.readControlTableItem(122, DXL_ID)){}
    delay(100);
  }
  */
  dxl.setGoalPosition(1, i);
  dxl.setGoalPosition(2, -i);
  i+=100;
  delay(200);
  if(i==5000){
    i=0;
    dxl.setGoalPosition(1, i);
    dxl.torqueOff(2);
    dxl.setOperatingMode(2, OP_PWM);
    dxl.torqueOn(2);
    dxl.setGoalPWM(2, 885);
    delay(4000);
    
    dxl.reboot(2);
    delay(10);
    dxl.torqueOff(2);
    dxl.setOperatingMode(2, OP_EXTENDED_POSITION);
    dxl.torqueOn(2);
    dxl.setGoalPosition(2, i);
    while(dxl.readControlTableItem(122, 1)){}
  }
  
/*
  // PWM Contorl Mode in protocol2.0
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_PWM);
  dxl.torqueOn(DXL_ID);
  if(dxl.setGoalPWM(DXL_ID, 885)){
    delay(1000);
    DEBUG_SERIAL.print("Present PWM : ");
    DEBUG_SERIAL.println(dxl.getPresentPWM(DXL_ID)); DEBUG_SERIAL.println();
  }
  */
}
