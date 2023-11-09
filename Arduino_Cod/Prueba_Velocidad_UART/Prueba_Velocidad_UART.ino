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

DynamixelShield dxl;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

String buff="";
byte error=0;
byte VY;
byte VX;
short int VL;
short int VR;

void setup() {
  // put your setup code here, to run once:
  
  // For Uno, Nano, Mini, and Mega, use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  Serial.begin(9600);
  Serial1.begin(9600);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXLL_ID);
  dxl.ping(DXLR_ID);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXLL_ID);
  dxl.setOperatingMode(DXLL_ID, OP_VELOCITY);
  dxl.torqueOn(DXLL_ID);
  dxl.torqueOff(DXLR_ID);
  dxl.setOperatingMode(DXLR_ID, OP_VELOCITY);
  dxl.torqueOn(DXLR_ID);
 
  buff+=(char)128;
  buff+=(char)128;
}

void loop() {
  // put your main code here, to run repeatedly:
  while(error<=100){
    if (Serial1.available()){
      buff=Serial1.readStringUntil('\n');
      error=0;
    }
    else{
      error+=1;
    }
    VY=(uint8_t)buff[0];
    VX=(uint8_t)buff[1];
    VL=map(VY+(VX-127),0,255,-265,265);
    VR=map(VY-(VX-127),0,255,-265,265);
    if(VL>265){VL=265;}
    else if(VL<-265){VL=-265;}
    else if((VL>-20)&&(VL<20)){VL=0;}
    if(VR>265){VR=265;}
    else if(VR<-265){VR=-265;}
    else if((VR>-20)&&(VR<20)){VR=0;}
    dxl.setGoalVelocity(DXLL_ID, VL);
    dxl.setGoalVelocity(DXLR_ID,-VR);
    
    //dxlL.setGoalVelocity(DXL_ID, 61.0, UNIT_RPM);
    /*
    Serial.println(dxlL.getPresentVelocity(DXLL_ID));
    Serial.println(dxlL.getPresentVelocity(DXLL_ID, UNIT_RPM));
    Serial.println(dxlL.getPresentVelocity(DXLL_ID, UNIT_PERCENT));
    */
    /*
    Serial.print(VY);
    Serial.print(' ');
    Serial.print(VX);
    Serial.print(' ');
    Serial.print(VL);
    Serial.print(' ');
    Serial.print(VR);
    Serial.print('\n');
    */
  }
  //Serial.println("Error de Conexión");
  buff="";
  buff+=(char)128;
  buff+=(char)128;
  error=0;
  // Please refer to e-Manual(http://emanual.robotis.com) for available range of value. 
}
