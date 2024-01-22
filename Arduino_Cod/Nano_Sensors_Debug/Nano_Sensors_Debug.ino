#include <QTRSensors.h>

#define Trigger1  10
#define Echo1     11
#define Trigger2  9
#define Echo2     8
#define Trigger3  15
#define Echo3     14

#define Battery   A5
#define Sharp1    A6
#define Sharp2    A7

#define Rc1       2
#define Rc2       3
#define Rc3       4
#define Rc4       5
#define Rc5       6
#define Rc6       7

#define B_low     350//790-800
#define B_high    385//910-920
#define U_lim     30//30
#define S_lim     100
#define R1_lim    450//320-340||510-520
#define R2_lim    290//200-240||320-330
#define R3_lim    300//190-200||320-340
#define R4_lim    340//230-240||360-380
#define R5_lim    260//190-210||280-290
#define R6_lim    320//280-300||372-380

QTRSensors qtr;

int bat = 0;
byte bat_msg = 0;
byte ciclo = 1;
byte modo = 0;
bool U[3];
bool S[2];
bool R[6];
uint16_t d1,d2,d3;
float linear = 0;
float angular = 0;
uint16_t sensorValues[6];

void setup() {
  Serial.begin(9600);
  
  pinMode(Trigger1, OUTPUT);
  pinMode(Trigger2, OUTPUT);
  pinMode(Trigger3, OUTPUT);
  
  pinMode(Echo1, INPUT);
  pinMode(Echo2, INPUT);
  pinMode(Echo3, INPUT);

  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){Rc1, Rc2, Rc3, Rc4, Rc5, Rc6}, 6);
  delay(500);
}

void loop() {
  //Lectura de sensores
  uint16_t s1 = analogRead(Sharp1);
  uint16_t s2 = analogRead(Sharp2);
  qtr.read(sensorValues);
  if(modo <= 3){
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
  if(modo <= 2){
    if(ciclo == 1){
      d1 = medirDistancia(Trigger1, Echo1);
      if(d1 <= U_lim){
        U[0] = 1;
        modo = 2;
      }
      else{
        modo = 0;
      }
      ciclo = 2;
    }
    else if(ciclo == 2){
      d2 = medirDistancia(Trigger2, Echo2);
      if(d2 <= U_lim){
        U[1] = 1;
        modo = 2;
      }
      else{
        modo = 0;
      }
      ciclo = 3;
    }
    else{
      d3 = medirDistancia(Trigger3, Echo3);
      if(d3 <= U_lim){
        U[2] = 1;
        modo = 2;
      }
      else{
        modo = 0;
      }
      ciclo = 1;
    }
  }
  if(modo <= 1){
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
      modo = 1;
    }
    else{
      modo = 0;
    }
  }
  bat = 5*map(analogRead(Battery),B_low,B_high,0,20);
  if(bat > 100){bat=100;}
  if(bat < 0){bat=0;}
  bat_msg = bat;
  /*
  //publicacion de datos por serial
  Serial.print("Sharps:");
  Serial.print(s1);
  Serial.print(" ");
  Serial.print(s2);
  Serial.print(" Ultrasonidos:");
  Serial.print(d1);
  Serial.print(" ");
  Serial.print(d2);
  Serial.print(" ");
  Serial.print(d3);
  Serial.print(" Reflectivos:");
  for (uint8_t i = 0; i < 6; i++){
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  for (uint8_t i = 0; i < 6; i++){
    Serial.print(R[i]);
    Serial.print(" ");
  }
  */
  //Asignacion de accion de control basada en el modo y la activacion
  if(modo == 0){
    linear = 0.0;
    angular = 0.0;
  }
  else if(modo == 1){
    //accion de control
    if(R[0] || R[1] || R[2]){
      linear = 0.0;
      angular = 0.5;
    }
    else if(R[3] || R[4]){
      linear = 0.0;
      angular = -0.5;
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
    if(U[0]){
      linear = -0.5;
      angular = -0.5;
    }
    else if(U[1]){
      linear = -0.5;
      angular = 0.0;
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
    if(S[0] && S[1]){
      linear = -0.5;
      angular = 0.0;
    }
    else if(S[0]){
      linear = -0.5;
      angular = -0.25;
    }
    else if(S[1]){
      linear = -0.5;
      angular = 0.25;
    }
    //reinicio de variables
    for (uint8_t i = 0; i < 2; i++){
      S[i] = 0;
    }
  }
  
  //Salida de modo y señal de control
  Serial.print("Modo:");
  Serial.print(modo);
  Serial.print(" Linear:");
  Serial.print(linear);
  Serial.print(" Angular:");
  Serial.print(angular);
  
  Serial.print(" Battery: ");
  //Serial.println(analogRead(Battery));
  Serial.println(bat_msg);
  modo = 0;
  linear = 0.0;
  angular = 0.0;
  bat = 0;
  
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
