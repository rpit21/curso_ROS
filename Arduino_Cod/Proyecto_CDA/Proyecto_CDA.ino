
#define LF 9
#define LB 8
#define RF 10
#define RB 11
#define TRIG 2
#define ECO 3
#define TRIGL 4
#define ECOL 5
#define TRIGR 6
#define ECOR 7

String buff="";
byte error=0;

int16_t vx[5]={128,128,128,128,128};
int16_t vy[10]={128,128,128,128,128,128,128,128,128,128};
int16_t VX=128;
int16_t VY=128;
int16_t VL=128;
int16_t VR=128;
uint8_t lf=0;
uint8_t lb=0;
uint8_t rf=0;
uint8_t rb=0;

uint16_t DURACION;
uint16_t DISTANCIA;
uint16_t DISTANCIA_1=75;
uint16_t DURACIONL;
uint16_t DISTANCIAL;
uint16_t DURACIONR;
uint16_t DISTANCIAR;

void setup() {
  Serial.begin(9600); 
  Serial1.begin(9600); 

  buff+=(char)128;
  buff+=(char)128;

  analogWrite(LF,0);
  analogWrite(LB,0);
  analogWrite(RF,0);
  analogWrite(RB,0);

  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  pinMode(TRIGL, OUTPUT);
  pinMode(ECOL, INPUT);
  pinMode(TRIGR, OUTPUT);
  pinMode(ECOR, INPUT);
  /*
  digitalWrite(TRIG, HIGH);    
  delay(1);       
  digitalWrite(TRIG, LOW);    
  DURACION = pulseIn(ECO, HIGH);  
  DISTANCIA_1 = DURACION / 58;
  */
}

void loop() {
  while(error<=20){
    if (Serial1.available()){
      buff=Serial1.readStringUntil('\n');
      error=0;
    }
    else{
      error+=1;
    }
    /*
    digitalWrite(TRIG, HIGH);    
    delay(1);       
    digitalWrite(TRIG, LOW);    
    DURACION = pulseIn(ECO, HIGH);  
    DISTANCIA = DURACION / 58;//58.2
    while(DISTANCIA > (DISTANCIA_1 + 30)){
      digitalWrite(TRIGL, HIGH);
      delay(1);       
      digitalWrite(TRIGL, LOW);    
      DURACIONL = pulseIn(ECOL, HIGH);  
      DISTANCIAL = DURACIONL / 58;//58.2
      digitalWrite(TRIGR, HIGH);    
      delay(1);       
      digitalWrite(TRIGR, LOW);    
      DURACIONR = pulseIn(ECOR, HIGH);  
      DISTANCIAR = DURACIONR / 58;//58.2
      //Aqui el cambio de direccion
      digitalWrite(TRIG, HIGH);    
      delay(1);       
      digitalWrite(TRIG, LOW);    
      DURACION = pulseIn(ECO, HIGH);  
      DISTANCIA = DURACION / 58;//58.2
      Serial.print(DISTANCIAL);
      Serial.print(' ');
      Serial.print(DISTANCIA);
      Serial.print(' ');
      Serial.println(DISTANCIAR);
    }
    DISTANCIA_1 = DISTANCIA;
    */
    //vy[0]=map(DISTANCIA,0,150,0,255);
    vy[0]=(uint8_t)buff[0];
    vx[0]=(uint8_t)buff[1];

    VY=0;
    for(uint8_t i=0;i<=9;i++){
      VY+=vy[i];
    }
    for(uint8_t i=9;i>=1;i--){
      vy[i]=vy[i-1];
    }
    VY=VY/10;
    VX=0;
    for(uint8_t i=0;i<=4;i++){
      VX+=vx[i];
    }
    for(uint8_t i=4;i>=1;i--){
      vx[i]=vx[i-1];
    }
    VX=VX/5;
    
    if(VY > 255){
      VY=255;
    }
    else if(VY)
    if(VX > 255){
      VX=255;
    }

    //VY=(uint8_t)buff[0];
    //VX=(uint8_t)buff[1];

    VL=VY+(VX-127);
    VR=VY-(VX-127);
    if(VL >= 148){
      if(VL > 255){
        lf=255;
      }
      else{
        lf=(VL-128)*2;
        lf=map(lf,0,255,100,255);
      }
      lb=0;
    }
    else if(VL <= 108){
      lb=(127-VL)*2;
      lb=map(lb,0,255,100,255);
      lf=0;
    }
    else{
      lf=0;
      lb=0;
    }
    if(VR >= 148){
      if(VR > 255){
        rf=255;
      }
      else{
        rf=(VR-128)*2;
        rf=map(rf,0,255,100,255);
      }
      rb=0;
    }
    else if(VR <= 108){
      rb=(127-VR)*2;
      rb=map(rb,0,255,100,255);
      rf=0;
    }
    else{
      rf=0;
      rb=0;
    }
    analogWrite(LF,lf);
    analogWrite(LB,lb);
    analogWrite(RF,rf);
    analogWrite(RB,rb);
    
    Serial.print(DISTANCIA);
    Serial.print(' ');
    Serial.print(buff);
    Serial.print(' ');
    Serial.print(VL);
    Serial.print(' ');
    Serial.print(VR);
    Serial.print(' ');
    Serial.print(' ');
    Serial.print(lf);
    Serial.print(' ');
    Serial.print(lb);
    Serial.print(' ');
    Serial.print(rf);
    Serial.print(' ');
    Serial.println(rb);
    
    //delay(10);
  }
  Serial.println("Error de Conexión");
  buff="";
  buff+=(char)128;
  buff+=(char)128;
  error=0;
}
