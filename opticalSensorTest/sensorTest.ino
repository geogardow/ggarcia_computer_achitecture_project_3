#define OPTICAL_SENSOR_LEFT 8
#define OPTICAL_SENSOR_RIGHT 9
#define LEFT_OUTPUT 10
#define RIGHT_OUTPUT 11
 
void setup() {
  Serial.begin(9600);   		//iniciar puerto serie
  pinMode(OPTICAL_SENSOR_LEFT, INPUT);	//definir pin como entrada
  pinMode(OPTICAL_SENSOR_RIGHT, INPUT);	//definir pin como entrada
  pinMode(LEFT_OUTPUT, OUTPUT);
  pinMode(RIGHT_OUTPUT, OUTPUT);

}
 
void loop(){
  int leftReading = 0;
  float rightReading = 0;

  leftReading = digitalRead(OPTICAL_SENSOR_LEFT);  //lectura digital de pin
  rightReading = digitalRead(OPTICAL_SENSOR_RIGHT);  //lectura digital de pin

  digitalWrite(LEFT_OUTPUT, leftReading);
  digitalWrite(RIGHT_OUTPUT, rightReading);

  delay(1000);
}
