                                                                                                                                                                                                        #include <ESP8266WiFi.h>
#include <DMotor_mod.h>

#define ledPin D2
#define voltagePin D6

AF_Stepper motorR(256, 1);
AF_Stepper motorL(256, 2);

float wheelRadius = 31.0; //in mm
int chosenSpeed = 100;

char chosenDirection;
float chosenLenght = 19.5;
float chosenAngle = 90;

const char* ssid = "TP-Link_6E7E";
const char* password = "09207189";

//const char* ssid = "SSID";
//const char* password = "password";

 
WiFiServer server(80);

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
    //Running the LLUBot:
    for(int i = 0; i < steps; i++){
      motorR.step(1, FORWARD, SINGLE); 
      motorL.step(1, FORWARD, SINGLE); 
    }
    Serial.print(" ");
    Serial.println("________________________________________");
  }
  if(way_ == 'B') {
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
  int steps = 0;

  Serial.println(chosenDirection);
  Serial.println();

  //Choosing the cm to introduce as steps in the motor motion:
  steps = angle_ * (256 * 8 / (2 * 3.1416 * wheelRadius / 10)) * 2 * 3.1416 * (16 /2) / 360;
  Serial.println(steps);
  Serial.println();

  //Choosing a direction for the motors:
  if(way_ == 'R') {
    //Running the LLUBot:
    for(int i = 0; i < steps; i++){
      motorR.step(1, BACKWARD, SINGLE); 
      motorL.step(1, FORWARD, SINGLE); 
    }
    Serial.print(" ");
    Serial.println("________________________________________");
  }
  if(way_ == 'L') {
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

  pinMode(voltagePin, INPUT);
  pinMode(ledPin, OUTPUT);
  
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

  // First read the voltage value for the correct functioning of the LLUBot.
  int sensorVal = digitalRead(voltagePin);
  
  if (sensorVal == 1) {
    digitalWrite(ledPin, LOW);
    
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
      value = 1;
    }
    if (request.indexOf("/MOTOR=BACKWARD") != -1) {
      value = 2;
    }
    if (request.indexOf("/MOTOR=RIGHT") != -1) {
      value = 3;
    }
    if (request.indexOf("/MOTOR=LEFT") != -1) {
      value = 4;
    } 
 
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang='es'>");
    client.println("  <head>");
    client.println("    <title> LLUBot </title>");
    client.println("    <style>");
    client.println("      body { background-color: #7c05a3; text-align: center; font: Verdana; font-size: 25px; color: white;}");
    client.println("    </style>");
    client.println("  </head>");
    client.println("  <body>");
    client.print  ("    <h1> The LLUBot has: ");
 
      if(value == 1) {
        client.print(" Run Forward.");  
      } if (value == 2) {
        client.print(" Run Backward.");  
      } if (value == 3) {
        client.print(" Turned Right.");  
      } if (value == 4) {
        client.print(" Turned Left.");  
      } else {
        client.print(" Stopped.");
      }
  
    client.println("   <p> Click <a href=\"/MOTOR=FORWARD\">here</a> to run the LLUBot forward 19.5 cm.</p>");
    client.println("   <p> Click <a href=\"/MOTOR=BACKWARD\">here</a> to run the LLUBot backward 19.5 cm.</p>");
    client.println("   <p> Click <a href=\"/MOTOR=RIGHT\">here</a> to turn the LLUBot 90 degrees to its right.</p>");
    client.println("   <p> Click <a href=\"/MOTOR=LEFT\">here</a> to turn the LLUBot 90 degrees to its left.</p><br>");

    if(sensorVal==0){
      client.println(" <p> The battery has to be charged.</p>");
      client.println("</html>");
    }
    else {
      client.println("</html>");
      }
 
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
    
  } else {
    digitalWrite(ledPin, HIGH);

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
    if (request.indexOf("/MOTOR=BATTERY") != -1) {
    // void because there is no action needed
    }
    
 
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
      
    client.println("<br><br>");
    client.println("The battery has to be charged.");
    client.println("</html>");
 
    delay(1);
    Serial.println("Client disconnected");
    client.println("Click <a href=\"/MOTOR=BATTERY\">here</a> to recharge the page.<br>");
    Serial.println("");
  }
}
