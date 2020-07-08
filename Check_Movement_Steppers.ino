#include <DMotor_mod.h>

//Initializate the two motors Right and Left
AF_Stepper motorR(256, 1);
AF_Stepper motorL(256, 2);

void setup() {

  motorD.setSpeed(100);   
  motorI.setSpeed(100);  

void loop() {
  // Using single coil steps
  motorR.step(5, FORWARD, SINGLE); 
  motorL.step(5, FORWARD, SINGLE); 
}
