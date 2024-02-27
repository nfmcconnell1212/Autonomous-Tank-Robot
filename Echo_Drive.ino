//Libraries
#include <NewPing.h>

//Front Ultrasonic Sensor
#define Front_trig 13
#define Front_echo 12
//Left Line Sensor
#define Left_sensor A1
//Right Line Sensor
#define Right_sensor A0
//Cefnter Line Sensor
#define Center_sensor A2

//Max distance for Ultrsonic sensor to sense
#define maximum_distance 35

//Motors
//LEFT
#define Motor1_dir1 23
#define Motor1_dir2 22
#define Motor1_PWMA 26
//RIGHT
#define Motor2_dir1 25
#define Motor2_dir2 24
#define Motor2_PWMB 27

const int motorSpeed = 130;

//Delays for turning
int delay_180 = 1400;
int delay_90 = 700;

//Define Front Ultrasonic Sensor
NewPing Fsonar(Front_trig, Front_echo, maximum_distance);
int Front_dist = 0;

void setup() {
  //Line Sensors
  pinMode(Left_sensor, INPUT);
  pinMode(Right_sensor, INPUT);
  pinMode(Center_sensor, INPUT);
  //Ultrasonic Sensors
  pinMode(Front_trig, OUTPUT);
  pinMode(Front_echo, INPUT);

  //Motors
  pinMode(Motor1_dir1, OUTPUT);
  pinMode(Motor1_dir2, OUTPUT);
  pinMode(Motor1_PWMA, OUTPUT);
  pinMode(Motor2_dir1, OUTPUT);
  pinMode(Motor2_dir2, OUTPUT);
  pinMode(Motor2_PWMB, OUTPUT);
  
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  //Set PWMAs high
  analogWrite(Motor1_PWMA, abs(motorSpeed));
  analogWrite(Motor2_PWMB, abs(motorSpeed));

  //Get Front Distane
  Front_dist = Fsonar.ping_cm();
  delay(50);

  //Value Calibration
  int Right_sensor_val = analogRead(Right_sensor);
  int Left_sensor_val = analogRead(Left_sensor);
  int Center_sensor_val = analogRead(Center_sensor);
  String values = "Right: " + String(Right_sensor_val) + " Left: " + String(Left_sensor_val) + " " + "Center: " + String(Center_sensor_val);
  Serial.println(values);

  Serial.print("F_d = ");
  Serial.println(Front_dist);

  //Front Ultrasonic Sensor 
  //If front ultrasonic sensor sees something, turn around
  if(Front_dist < 30 && Front_dist > 0){
    rotateRight();
    Serial.println("Turn Around");
    delay(2500);
    moveStop();
    delay(500);
  }

  //Line detected by both sensors
  if(analogRead(Right_sensor)<=400 && analogRead(Left_sensor)<=400){
    Serial.println("Rotate 90");
    rotate90Right();
    delay(1400);
    moveStop();
    delay(100);
  }
  //Line detected by right sensor
  else if(analogRead(Right_sensor)>=400 && analogRead(Left_sensor)<=500){
    Serial.println("Rotate Right");
    //Rotate right
    rotateRight();
    delay(500);
    moveStop();
    delay(100);
  }
  //:ine detected by Left sensor
  else if(analogRead(Right_sensor)<=200 && analogRead(Left_sensor)>=450){
    //Rotate left
    Serial.println("Rotate Left");
    rotateLeft();
    delay(500);
    moveStop();
    delay(100);
  }
  //Corner detected by center
  else if(analogRead(Center_sensor)<=350 && analogRead(Right_sensor)>= 450 && analogRead(Left_sensor)>= 450){
    Serial.println("Corner, turn around");
    moveBackward();
    delay(750);
    rotateRight();
    delay(750);
  }
  //Otherwise just move straight
  else{
    moveForward();
  }
}

void moveStop(){
  digitalWrite(Motor1_dir1, LOW);
  digitalWrite(Motor1_dir2, LOW);
  digitalWrite(Motor2_dir1, LOW);
  digitalWrite(Motor2_dir2, LOW);
}

void moveForward(){
  digitalWrite(Motor1_dir1, LOW);
  digitalWrite(Motor1_dir2, HIGH);
  digitalWrite(Motor2_dir1, LOW);
  digitalWrite(Motor2_dir2, HIGH);
}

void rotateRight(){
  digitalWrite(Motor1_dir1, LOW);
  digitalWrite(Motor1_dir2, HIGH);
  digitalWrite(Motor2_dir1, HIGH);
  digitalWrite(Motor2_dir2, LOW);
}

void rotate90Right(){
  digitalWrite(Motor1_dir1, LOW);
  digitalWrite(Motor1_dir2, HIGH);
  digitalWrite(Motor2_dir1, HIGH);
  digitalWrite(Motor2_dir2, LOW);
}

void rotateLeft(){
  digitalWrite(Motor1_dir1, HIGH);
  digitalWrite(Motor1_dir2, LOW);
  digitalWrite(Motor2_dir1, LOW);
  digitalWrite(Motor2_dir2, HIGH);
}

void moveBackward(){
  digitalWrite(Motor1_dir1, HIGH);
  digitalWrite(Motor1_dir2, LOW);
  digitalWrite(Motor2_dir1, HIGH);
  digitalWrite(Motor2_dir2, LOW);
}
