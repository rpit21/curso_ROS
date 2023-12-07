#include <ros.h>
//#include <std_msgs/Byte.h>
#include <std_msgs/UInt8MultiArray.h>
//#include <std_msgs/ByteMultiArray.h>
//#include <geometry_msgs/Twist.h>

#define Trigger1  10
#define Echo1     11
#define Trigger2  9
#define Echo2     8
#define Trigger3  15
#define Echo3     14

#define Sharp1    A6
#define Sharp2    A7

#define Reflectivo1 2
#define Reflectivo2 3
#define Reflectivo3 4
#define Reflectivo4 5
#define Reflectivo5 6
#define Reflectivo6 7

#define U_lim     30
#define S_lim     100
#define R_lim     500

byte modo = 0;
float linear = 0;
float angular = 0;

ros::NodeHandle  nh;
/*
void modo_cb(const std_msgs::Byte &msg){
  modo = msg.data;
}
*/
//ros::Subscriber<std_msgs::Byte> modo_sub("/modo", &modo_cb);

std_msgs::UInt8MultiArray ultrasonidos;
//std_msgs::ByteMultiArray sharps;
//std_msgs::ByteMultiArray reflectivos;
//geometry_msgs::Twist cmd_vel_sensores;

ros::Publisher ultrasonidos_pub("/ultrasonidos", &ultrasonidos);
//ros::Publisher sharps_pub("/sharps", &sharps);
//ros::Publisher reflectivos_pub("/reflectivos", &reflectivos);
//ros::Publisher cmd_vel_sensores_pub("/cmd_vel_sensores", &cmd_vel_sensores);

void setup() {
  //Serial.begin(115200);//iniciailzamos la comunicación
  //nh.getHardware()->setBaud(57600);
  nh.initNode();
  
  pinMode(Trigger1, OUTPUT);
  pinMode(Trigger2, OUTPUT);
  pinMode(Trigger3, OUTPUT);
  
  pinMode(Echo1, INPUT);
  pinMode(Echo2, INPUT);
  pinMode(Echo3, INPUT);

  //nh.subscribe(modo_sub);
  nh.advertise(ultrasonidos_pub);
  //nh.advertise(sharps_pub);
  //nh.advertise(reflectivos_pub);
  //nh.advertise(cmd_vel_sensores_pub);
}

void loop() {
  uint16_t d1 = medirDistancia(Trigger3, Echo3);
  uint16_t d2 = medirDistancia(Trigger2, Echo2);
  uint16_t d3 = medirDistancia(Trigger1, Echo1);
  if(d1 <= 255){
    ultrasonidos.data[0] = d1;
  }
  else{
    ultrasonidos.data[0] = 255;
  }
  if(d2 <= 255){
    ultrasonidos.data[1] = d2;
  }
  else{
    ultrasonidos.data[1] = 255;
  }
  if(d3 <= 255){
    ultrasonidos.data[2] = d1;
  }
  else{
    ultrasonidos.data[2] = 255;
  }
  ultrasonidos_pub.publish( &ultrasonidos );
  nh.spinOnce();
  /*
  int s1 = analogRead(Sharp1);
  int s2 = analogRead(Sharp2);
  if(s1 <= S_lim){
    sharps.data[0] = 1;
  }
  else{
    sharps.data[0] = 0;
  }
  if(s2 <= S_lim){
    sharps.data[1] = 1;
  }
  else{
    sharps.data[1] = 0;
  }
  sharps_pub.publish( &sharps );
  nh.spinOnce();

//pendiente implementar reflectivos
  reflectivos.data[0] = 0;
  reflectivos.data[1] = 0;
  reflectivos.data[2] = 0;
  reflectivos.data[3] = 0;
  reflectivos.data[4] = 0;
  reflectivos.data[5] = 0;
  sharps_pub.publish( &sharps );
  nh.spinOnce();
  */
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
