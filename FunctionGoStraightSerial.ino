#include <DMotor_mod.h>

AF_Stepper motorR(256, 1);
AF_Stepper motorL(256, 2);

float wheelRadius = 31.0; //in mm
int chosenSpeed = 100;

char chosenDirection;
float chosenQuantity = 0.0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Choose a direction:");
  Serial.println("F to go Forward and B to go Backward");

  motorR.setSpeed(chosenSpeed);   
  motorL.setSpeed(chosenSpeed); 
} 

void loop() {
   if(Serial.available() > 0){
   
     chosenDirection = Serial.read();
     chosenQuantity = 19.5;

     if(chosenDirection == 'F' || chosenDirection == 'B'){
       goStraight(chosenDirection, chosenQuantity);
     }
     Serial.println("Choose a direction:");
     Serial.println("F to go Forward and B to go Backward");
   }
}

void goStraight (char way, float cm){
  char way_ = way;
  float cm_ = cm;
  int steps = 0;

  Serial.println(chosenDirection);
  Serial.println();

  //Choosing the cm to introduce as steps in the motor motion:
  steps = cm_ * 256 * 8 / (2 * 3.1416 * wheelRadius / 10);
  Serial.println(steps);
  Serial.println();

  //Choosing a direction for the motors:
  if(way_ == 'F') {
    //Running the robot:
    for(int i = 0; i < steps; i++){
      motorR.step(1, FORWARD, SINGLE); 
      motorL.step(1, FORWARD, SINGLE); 
    }
    Serial.print(" ");
    Serial.println("________________________________________");
  }
  if(way_ == 'B') {
    //Running the robot:
    for(int i = 0; i < steps; i++){
      motorR.step(1, BACKWARD, SINGLE); 
      motorL.step(1, BACKWARD, SINGLE); 
    }
    Serial.print(" ");
    Serial.println("________________________________________");
  }
  else {
    
  }
}
