const int Trigger1 = 10;   //Pin digital 2 para el Trigger del sensor
const int Echo1 = 12;   //Pin digital 3 para el echo del sensor

const int Trigger2 = 1;   //Pin digital 2 para el Trigger del sensor
const int Echo2 = 0;   //Pin digital 3 para el echo del sensor

const int Trigger3 = 3;   //Pin digital 2 para el Trigger del sensor
const int Echo3 = 2;   //Pin digital 3 para el echo del sensor

void setup() {
  Serial.begin(9600);//iniciailzamos la comunicación
 
  pinMode(Trigger3, OUTPUT); //pin como salida
  pinMode(Trigger2, OUTPUT); //pin como salida
  pinMode(Trigger1, OUTPUT); //pin como salida
  
  pinMode(Echo3, INPUT);  //pin como entrada
  pinMode(Echo2, INPUT);  //pin como entrada
  pinMode(Echo1, INPUT);  //pin como entrada

}

void loop() {
  //long d1, d2, d3; //distancia en centimetros

  //digitalWrite(Trigger1, HIGH);
  //digitalWrite(Trigger2, HIGH);

  int d3 = medirDistancia(Trigger3, Echo3);
  int d2 = medirDistancia(Trigger2, Echo2);
  int d1 = medirDistancia(Trigger1, Echo1);
 
  int s1 = analogRead(A0);
  int s2 = analogRead(A1);
  Serial.print(", Distancia S1: ");
  Serial.print(s1);      //Enviamos serialmente el valor de la distancia
  Serial.print("ADC");
 
  Serial.print(", Distancia S2: ");
  Serial.print(s2);      //Enviamos serialmente el valor de la distancia
  Serial.print("ADC");
  
  Serial.print("Distancia U1: ");
  Serial.print(d1);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");

  Serial.print(", Distancia U2: ");
  Serial.print(d2);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");

  Serial.print(", Distancia U3: ");
  Serial.print(d3);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  delay(100);          //Hacemos una pausa de 100ms
  }


  int medirDistancia(int trigPin, int echoPin) {
  // Establecer el pin de trigger en bajo durante 2 microsegundos
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Enviar un pulso ultrasónico de 10 microsegundos al pin de trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir la duración del pulso en el pin echo para calcular la distancia
  unsigned long duration = pulseIn(echoPin, HIGH);
  
  // Calcular la distancia en centímetros (aproximadamente)
  int distance = duration * 0.034 / 2; // Fórmula: velocidad del sonido en el aire (343 m/s) / 2

  return distance;
}
