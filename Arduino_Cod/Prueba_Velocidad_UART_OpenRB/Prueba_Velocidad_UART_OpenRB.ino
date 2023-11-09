#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial1
#define DXL_DIR_PIN  -1
#define DXL_IDL 2
#define DXL_IDR 1

const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

byte error=0;
byte VY;
byte VX;
int VL;
int VR;
String buff;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxl.ping(DXL_IDL);
  dxl.ping(DXL_IDR);
  
  dxl.torqueOff(DXL_IDL);
  dxl.setOperatingMode(DXL_IDL,  OP_PWM);
  dxl.torqueOn(DXL_IDL);
  dxl.torqueOff(DXL_IDR);
  dxl.setOperatingMode(DXL_IDR,  OP_PWM);
  dxl.torqueOn(DXL_IDR);

  buff+=(char)128;
  buff+=(char)128;
}

void loop() {
  while(error<=100){
    if (Serial2.available()){
      buff=Serial2.readStringUntil('\n');
      Serial.print(buff);
      Serial.print(" ");
      Serial.print(dxl.getPresentCurrent(DXL_IDL, UNIT_MILLI_AMPERE));
      Serial.print(" ");
      Serial.println(dxl.getPresentCurrent(DXL_IDR, UNIT_MILLI_AMPERE));
      error=0;
    }
    else{
      error+=1;
    }
    VY=(uint8_t)buff[0];
    VX=(uint8_t)buff[1];
    VL=map(VY+(VX-127),1,255,-885,885);
    VR=map(VY-(VX-127),1,255,-885,885);

    if(VL>885){VL=885;}
    else if(VL<-885){VL=-885;}
    else if((VL>-50)&&(VL<50)){VL=0;}
    if(VR>885){VR=885;}
    else if(VR<-885){VR=-885;}
    else if((VR>-50)&&(VR<50)){VR=0;}

    dxl.setGoalPWM(DXL_IDL, VL);
    dxl.setGoalPWM(DXL_IDR,-VR);
  }
  buff="";
  buff+=(char)128;
  buff+=(char)128;
  error=0;
}
