#include <ESP8266WiFi.h>
#include <DMotor_mod.h>

AF_Stepper motorR(256, 1);
AF_Stepper motorL(256, 2);

float wheelRadius = 31.0; //in mm
int chosenSpeed = 100;

char chosenDirection;
float chosenLenght = 19.5;
float chosenAngle = 90;

const char* ssid = "SSID";
const char* password = "password";

 
int ledPin = D5;
WiFiServer server(80);

void goStraight (char way, float cm){
  char way_ = way;
  float cm_ = cm;
  //String motorsDirection;
  int steps = 0;

  Serial.println(chosenDirection);
  Serial.println();

  //Choosing the cm to introduce as steps in the motor motion:
  steps = cm_ * 256 * 8 / (2 * 3.1416 * wheelRadius / 10);
  Serial.println(steps);
  Serial.println();

  //Choosing a direction for the motors:
  if(way_ == 'F') {
    //motorsDirection = "FORWARD";
    //Running the LLUBot:
    for(int i = 0; i < steps; i++){
      motorR.step(1, FORWARD, SINGLE); 
      motorL.step(1, FORWARD, SINGLE); 
    }
    Serial.print(" ");
    Serial.println("________________________________________");
  }
  if(way_ == 'B') {
    //motorsDirection = "BACKWARD";
    //Running the LLUBot:
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

void turn (char way, float angle){
  char way_ = way;
  float angle_ = angle;
  //String motorsDirection;
  int steps = 0;

  Serial.println(chosenDirection);
  Serial.println();

  //Choosing the cm to introduce as steps in the motor motion:
  steps = angle_ * (256 * 8 / (2 * 3.1416 * wheelRadius / 10)) * 2 * 3.1416 * (16 /2) / 360;
  Serial.println(steps);
  Serial.println();

  //Choosing a direction for the motors:
  if(way_ == 'R') {
    //motorsDirection = "FORWARD";
    //Running the LLUBot:
    for(int i = 0; i < steps; i++){
      motorR.step(1, BACKWARD, SINGLE); 
      motorL.step(1, FORWARD, SINGLE); 
    }
    Serial.print(" ");
    Serial.println("________________________________________");
  }
  if(way_ == 'L') {
    //motorsDirection = "BACKWARD";
    //Running the LLUBot:
    for(int i = 0; i < steps; i++){
      motorR.step(1, FORWARD, SINGLE); 
      motorL.step(1, BACKWARD, SINGLE); 
    }
    Serial.print(" ");
    Serial.println("________________________________________");
  }
  else {
  }
}
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  //Steppers speed
  motorR.setSpeed(chosenSpeed);
  motorL.setSpeed(chosenSpeed);  
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  int value = 0;
  if (request.indexOf("/MOTOR=FORWARD") != -1) {
    //goStraight('F', chosenLenght);
    value = 1;
  }
  if (request.indexOf("/MOTOR=BACKWARD") != -1) {
    //goStraight('B', chosenLenght);
    value = 2;
  }
  if (request.indexOf("/MOTOR=RIGHT") != -1) {
    //goStraight('R', chosenAngle);
    value = 3;
  }
  if (request.indexOf("/MOTOR=LEFT") != -1) {
    //goStraight('L', chosenAngle);
    value = 4;
  } 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  //client.print("The LLUBot waits for your instruction:");
  client.print("The LLUBot is now: ");
 
  if(value == 1) {
    client.print("Running Forward");  
  } if (value == 2) {
    client.print("Running Backward");  
  } if (value == 3) {
    client.print("Turning Right");  
  } if (value == 4) {
    client.print("Turning Left");  
  } else {
    client.print("Stopped");
  }
  
  client.println("<br><br>");
  client.println("Click <a href=\"/MOTOR=FORWARD\">here</a> to run the LLUBot forward 19.5 cm<br>");
  client.println("Click <a href=\"/MOTOR=BACKWARD\">here</a> to run the LLUBot backward 19.5 cm<br>");
  client.println("Click <a href=\"/MOTOR=RIGHT\">here</a> to turn the LLUBot 90 degrees to its right<br>");
  client.println("Click <a href=\"/MOTOR=LEFT\">here</a> to turn the LLUBot 90 degrees to its left<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

  if(value == 1) {
    goStraight('F', chosenLenght);  
  } if(value == 2) {
    goStraight('B', chosenLenght);  
  } if(value == 3) {
    turn('R', chosenAngle);  
  } if(value == 4) {
    turn('L', chosenAngle);  
  } else {
    
  }
}
