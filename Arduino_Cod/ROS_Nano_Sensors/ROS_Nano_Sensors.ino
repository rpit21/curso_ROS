#include <ros.h>
#include <std_msgs/Byte.h>
#include <geometry_msgs/Twist.h>
#include <QTRSensors.h>

#define Trigger1  10
#define Echo1     11
#define Trigger2  9
#define Echo2     8
#define Trigger3  15
#define Echo3     14

#define Sharp1    A6
#define Sharp2    A7

#define Rc1       2
#define Rc2       3
#define Rc3       4
#define Rc4       5
#define Rc5       6
#define Rc6       7

#define U_lim     30
#define S_lim     100
#define R1_lim    200 //120|160-204
#define R2_lim    125 //80-120
#define R3_lim    260 //200-290
#define R4_lim    300 //250-340
#define R5_lim    160 //120-160
#define R6_lim    300 //250-350*

QTRSensors qtr;

byte modo = 0;
bool U[3];
bool S[2];
bool R[6];
float linear = 0;
float angular = 0;
uint16_t sensorValues[6];

ros::NodeHandle  nh;

std_msgs::Byte modo_msg;
geometry_msgs::Twist cmd_vel_sensores;

ros::Publisher modo_pub("/modo", &modo_msg);
ros::Publisher cmd_vel_sensores_pub("/cmd_vel_sensores", &cmd_vel_sensores);

void setup() {
  //Serial.begin(115200);//iniciailzamos la comunicación
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  
  pinMode(Trigger1, OUTPUT);
  pinMode(Trigger2, OUTPUT);
  pinMode(Trigger3, OUTPUT);
  
  pinMode(Echo1, INPUT);
  pinMode(Echo2, INPUT);
  pinMode(Echo3, INPUT);

  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){Rc1, Rc2, Rc3, Rc4, Rc5, Rc6}, 6);
  delay(500);

  nh.advertise(modo_pub);
  nh.advertise(cmd_vel_sensores_pub);
}

void loop() {
  //Lectura de sensores
  uint16_t s1 = analogRead(Sharp1);
  uint16_t s2 = analogRead(Sharp2);
  uint16_t d1 = medirDistancia(Trigger1, Echo1);
  uint16_t d2 = medirDistancia(Trigger2, Echo2);
  uint16_t d3 = medirDistancia(Trigger3, Echo3);
  qtr.read(sensorValues);
  //Comprobacion de limites para asignacion de modo
  if(modo >= 0){
    if(s1 <= S_lim){
      S[0] = 1;
    }
    if(s2 <= S_lim){
      S[1] = 1;
    }
    if(S[0] || S[1]){
     modo = 3;
    }
    else{
      modo = 0;
    }
  }
  if(modo == 0){
    if(d1 <= U_lim){
      U[0] = 1;
    }
    if(d2 <= U_lim){
      U[1] = 1;
    }
    if(d3 <= U_lim){
      U[2] = 1;
    }
    if(U[0] || U[1] || U[2]){
      modo = 2;
    }
    else{
      modo = 0;
    }
  }
  if(modo == 0){
    if(sensorValues[0] >= R1_lim){
      R[0] = 1;
    }
    if(sensorValues[1] >= R2_lim){
      R[1] = 1;
    }
    if(sensorValues[2] >= R3_lim){
      R[2] = 1;
    }
    if(sensorValues[3] >= R4_lim){
      R[3] = 1;
    }
    if(sensorValues[4] >= R5_lim){
      R[4] = 1;
    }
    if(sensorValues[5] >= R6_lim){
      R[5] = 1;
    }
    if(R[0] || R[1] || R[2] || R[3] || R[4] || R[5]){
      //modo = 1;                                     //Desactivado hasta calibrar bien
    }
    else{
      modo = 0;
    }
  }

  //Asignacion de accion de control basada en el modo y la activacion
  if(modo == 0){
    linear = 0.0;
    angular = 0.0;
  }
  else if(modo == 1){
    //accion de control
    if(R[0] || R[1] || R[2]){
      linear = 0.0;
      angular = -0.5;
    }
    else if(R[3] || R[4]){
      linear = 0.0;
      angular = 0.5;
    }
    else if(R[5]){
      linear = 0.5;
      angular = 0.0;
    }
    //reinicio de variables
    for (uint8_t i = 0; i < 6; i++){
      R[i] = 0;
    }
  }
  else if(modo == 2){
    //accion de control
    if(U[1] || (U[0] && U[2])){
      linear = -0.5;
      angular = 0.0;
    }
    else if(U[0]){
      linear = -0.5;
      angular = -0.5;
    }
    else if(U[2]){
      linear = -0.5;
      angular = 0.5;
    }
    //reinicio de variables
    for (uint8_t i = 0; i < 3; i++){
      U[i] = 0;
    }
  }
  else if(modo == 3){
    //accion de control
    linear = -0.5;
    angular = 0.0;
    //reinicio de variables
    for (uint8_t i = 0; i < 2; i++){
      S[i] = 0;
    }
  }

  //publicacion de modo de funcionamiento y accion de control
  modo_msg.data = modo;
  modo_pub.publish(&modo_msg);
  modo = 0;
  nh.spinOnce();
  
  cmd_vel_sensores.linear.x = linear;
  cmd_vel_sensores.angular.z = angular;
  cmd_vel_sensores_pub.publish(&cmd_vel_sensores);
  linear = 0.0;
  angular = 0.0;
  nh.spinOnce();
  
  //delay para evitar saturacion de mensajes
  delay(100);
  }


int medirDistancia(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long duration = pulseIn(echoPin, HIGH);
  uint16_t distance = duration * 0.0343 / 2; // Fórmula: velocidad del sonido en el aire (343 m/s) / 2
  return distance;
}
