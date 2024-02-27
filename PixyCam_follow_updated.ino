//including the libraries
#include <Pixy2.h>
#include <Stepper.h>

//defining pins and variables
#define Left_sensor A1
#define Right_sensor A0
#define Right_pwma 8
#define Left_pwma 9

//Stepper Motor
const int stepsPerRev = 512;

//Initiate Stepper Motors
Stepper myStepper(stepsPerRev, 2, 3, 8, 9);
Stepper trigStepper(stepsPerRev, 6, 7, 10, 11);
int current_step;

//Initiate PixyCam and PixyCam Variables
Pixy2 pixy;
int cont = 0;
int signature, x, y, width, height;
float cx, cy, area;
float deadzone = 0.25; //set deadzone for Target block
float turn;
float direction;

void setup() {  
  //Setting pin types
  pinMode(Right_sensor, INPUT);
  pinMode(Left_sensor, INPUT);
  pinMode(Left_pwma, OUTPUT);
  pinMode(Right_pwma, OUTPUT);

  //Begin serial communication
  Serial.begin(115200); //PixyCam serial
  Serial.print("Starting...\n");

  //Initiate Pixy
  pixy.init();

  //Home position for stepper
  current_step = 0;
}

void loop(){
    //pixycheck finds the x value of the "block" and returns how far awaay
  turn = pixyCheck();
  Serial.print(turn);
  Serial.print(" ");
  
  //deadzone is set above and its how close to 0 you would consider "center"
  if(turn > -deadzone && turn < deadzone){
    bool can = true;
    turn = 0; 
  }

  //Turn Camera Right
  if(turn > 0 && turn != 360){
    Serial.print("Turn Right ");
    myStepper.setSpeed(20);
    myStepper.step(20);
    current_step += 20;
    Serial.println(current_step);
  }
  //Turn Camera Left
  else if(turn < 0 && turn != 360){
    Serial.print("Turn Left ");
    myStepper.setSpeed(20);
    myStepper.step(-20);
    current_step -= 20;
    Serial.println(current_step);
  }
  //360 is an arbitrary value for when there are no blocks detected
  else if(turn == 360){
    Serial.println("No blocks ");
    //these if statements are for returning to home
    //For however far away the stepper is from zero it will go back to it
    if(current_step > 0){
      Serial.println("Return Home 1");
      for(int i = current_step; i-=20; i==0){
        myStepper.setSpeed(20);
        myStepper.step(-20);
        current_step -= 20;
        Serial.println(current_step);
      }
      //Needed to correct to true 0
      myStepper.step(-current_step);
      Serial.println(current_step);
      current_step = 0;
    }
    if(current_step < 0){
      Serial.println("Return Home 2");
      Serial.println(current_step);
      for(int i = current_step; i+=20; i==0){
        myStepper.setSpeed(20);
        myStepper.step(20);
        current_step += 20;
      }
      myStepper.step(-current_step);
      Serial.println(current_step);
      Serial.println(-current_step);
      current_step = 0;
    }
    else{
      Serial.println("End");
      Serial.println(current_step);
    }
  }
  //Fires when turn = 0
  else{
    Serial.println("Center, FIRE");
    Serial.println(current_step);
    delay(300);
    trigStepper.setSpeed(25);
    trigStepper.step(stepsPerRev);
  }
  //Bounds to prevent the stepper from rotating too far
  if(current_step > 260 || current_step < -260){
    Serial.println("Too far returning to home");
    if(current_step > 0){
      for(int i = current_step; i-=10; i==0){
        myStepper.step(-10);
        current_step -= 10;
        Serial.println(current_step);
      }
    }
    else{
      for(int i = current_step; i+=10; i==0){
        myStepper.step(10);
        current_step += 10;
        Serial.println(current_step);
      }
    }  }
  //}
  delay(100);
}

//PixyCheck program that finds block and returns the distance to center of block
float pixyCheck(){
  uint16_t blocks = pixy.ccc.getBlocks();  //Get Blocks

  if(blocks) {    
    //digitalWrite(interupt_wire, HIGH);
    signature = pixy.ccc.blocks[0].m_signature; //signature of object
    height = pixy.ccc.blocks[0].m_height; //height of the object
    width = pixy.ccc.blocks[0].m_width; //width of the object
    x = pixy.ccc.blocks[0].m_x;//x value of the object
    y = pixy.ccc.blocks[0].m_y;//y value of the object
    cx = (x + (width / 2)); //center x position
    cy = (y + (height / 2)); //center y position
    cx = mapfloat(cx, 0, 320, -1, 1); //map x value to -1 and 1 to help with computation
    cy = mapfloat(cy, 0, 320, -1, 1); //map y value to -1 and 1 to help with computation
    //Serial.print("CX = ");
    //Serial.print(cx);
    //Serial.print(" CY = ");
    //Serial.println(cy);
  }
  else{
    //digitalWire(interupt_wire, LOW);
    //cont += 1;
    //if (cont == 100) {
      //cont = 0;
      cx = 360;
    //}
  }
  return cx; //send back the x location of the object
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max){
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}


