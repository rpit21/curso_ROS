#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial1
#define DXL_DIR_PIN  -1
#define DXL_IDL 1
#define DXL_IDR 2

const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

int VL;
int VR;
String buff;

void setup() {
  Serial2.begin(115200);

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

}

void loop() {
  if (Serial2.available() > 0) {
        buff=Serial2.readStringUntil('\n');
        VL=map((uint8_t)buff[0],1,255,-885,885);
        VR=map((uint8_t)buff[1],1,255,-885,885);
        dxl.setGoalPWM(DXL_IDL, VL);
        dxl.setGoalPWM(DXL_IDR, -VR);
    }
}
