#define TRIGGER 12
#define ECHO 13
#define OUTPUT_0 15
#define OUTPUT_1 18

void setup() {
  Serial.begin(9600);//iniciailzamos la comunicación
  pinMode(TRIGGER, OUTPUT); //pin como salida
  pinMode(ECHO, INPUT);  //pin como entrada
  digitalWrite(TRIGGER, LOW);//Inicializamos el pin con 
  pinMode(OUTPUT_0, OUTPUT); //pin como salida
  pinMode(OUTPUT_1, OUTPUT); //pin como salida
}

void loop()
{

  long time; //timepo que demora en llegar el eco
  long distance; //distancia en centimetros

  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(TRIGGER, LOW);
  
  time = pulseIn(ECHO, HIGH); //obtenemos el ancho del pulso
  distance = time/59;         //escalamos el tiempo a una distancia en cm
  
  if (distance > 5){
    digitalWrite(OUTPUT_0, HIGH);
    digitalWrite(OUTPUT_1, LOW);

  } else {
    digitalWrite(OUTPUT_0, LOW);
    digitalWrite(OUTPUT_1, HIGH);
  }
  
  delay(100);          //Hacemos una pausa de 100ms
}
