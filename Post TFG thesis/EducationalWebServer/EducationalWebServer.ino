#include <ESP8266WiFi.h>
#include <DMotor_mod.h>

#define ledPin D2
#define voltagePin D6

AF_Stepper motorR(256, 1);
AF_Stepper motorL(256, 2);

float wheelRadius = 32.0; //in mm
int chosenSpeed = 100;

char chosenDirection;
float chosenLenght = 19.5;
float chosenAngle = 90;

//const char* ssid = "TP-Link_6E7E";
//const char* password = "09207189";

const char* ssid = "SSID";
const char* password = "password";

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
  steps = angle_ * (256 * 8 / (2 * 3.1416 * wheelRadius / 10)) * 2 * 3.1416 * (15.6 /2) / 360;
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
  
  if (sensorVal == 0) {
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

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE html>");
    client.println("<html lang='es'>");
    client.println("  <head>");
    client.println("    <title> LLUBot </title>");
    client.println("    <style>");
    client.println("          body { background-color: #7c05a3; text-align: center; font: Verdana; font-size: 25px; color: white;}");
    client.println("    </style>");
    client.println("  </head>");
    client.println("  <body>");
    client.println("      <p> Low battery, please turn the LLUBot off and charge it completely.</p>");
    client.println("  </body>");
    client.println("</html>");
  }
  else {
  
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
    int mov = 0;
    int pos = request.lastIndexOf('-');
  
    if (request.indexOf("/MOTOR=FORWARD") != -1) {
      value = 1;
      request.remove(0,pos+1);
      mov = request.toInt();
      }
    if (request.indexOf("/MOTOR=BACKWARD") != -1) {
      value = 2;
      request.remove(0,pos+1);
      mov = request.toInt();
    }
    if (request.indexOf("/MOTOR=RIGHT") != -1) {
      value = 3;
      request.remove(0,pos+1);
      mov = request.toInt();
    }
    if (request.indexOf("/MOTOR=LEFT") != -1) {
      value = 4;
      request.remove(0,pos+1);
      mov = request.toInt();
    } 
  
   
   
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE html>");
    client.println("<html lang='es'>");
    client.println("  <head>");
    client.println("    <title> LLUBot </title>");
    client.println("    <style>");
    client.println("          body { background-color: #7c05a3; text-align: center; font: Verdana; font-size: 25px; color: white;}");
    client.println("          .main button {  height: 50px; width: 50%; font-size: 18px; }");
    client.println("          .main { display: block; }");
    client.println("          .app1 { display: none; }");
    client.println("          .app2 { display: none; }");
    client.println("          .app3 { display: none; }");
    client.println("          button { font-size: 18px; }");
    client.println("          a:visited { color: white; }");
    client.println("    </style>");
    client.println("  </head>");
    client.println("  <body>");
    client.println("    <div class='main'>");
    client.println("      <h1> LLUBot learning machine </h1><br>");
    client.println("      <button id='btn1'> 1. Introduction </button><br><br>");
    client.println("      <button id='btn2'> 2. Let's learn applied maths </button><br><br>");
    client.println("      <button id='btn3'> 3. Let's move the LLUBot </button><br><br>");
    client.println("    </div>");
    client.println("    <div class='app1'>");
    client.println("      <h1> Introduction </h1><br>");
    client.println("      <p> This robot is a prototype for the bachelor degree finalization of the Electronics Industrial and Automatical Engineering Degree of Luisana Lara. It is thought to be used in educational environments, as a tool for learning concepts of Electronics, Mechanical Design, Programming, as well as others that are included in the discipline of Robotics.</p>");
    client.println("      <img src='https://luisanalarab.files.wordpress.com/2020/07/img_20200708_195609.jpg?w=324&h=182&zoom=2'><br>");
    client.println("      <button class='back'> Inicio </button>");
    client.println("    </div>");
    client.println("    <div class='app2'>");
    client.println("      <h1> First activity: application of Maths </h1><br>");
    client.println("      <p> When we are first tought of geometry in school, we never think that they can be really used in our daily life. And as a surprise, when using technology, all the engineers and technicians have to know a bit of geometry in order to make a Roomba useful, a videogame fun and a speaker easy to carry around, for example. </p>");
    client.println("      <p> It is soon when we first knew the geometry of a circle, as the area and the perimeter depending on the radius of the circle, as seen in the picture below. </p>");
    client.println("      <img src='https://i1.wp.com/www.aplustopper.com/wp-content/uploads/2016/08/area-of-a-circle-2.jpeg?resize=869%2C428&ssl=1'>");
    client.println("      <p> Did you know that the perimeter or circumference is necessary for the localization of a mobile robot as a Roomba (those cleaning robots famous for carrying cats around the house randomly)? Well, a mobile robot needs to know where it is in order to be independent, which is fundamental for this types of robots. But the real question is why and how the perimeter is used. </p>");
    client.println("      <h2> Motors: brief introduction </h2>");
    client.println("      <p> The fun thing about us humans moving around in a place is that, generally speaking, we automatically do it with our legs. But imagine we had to build ourselves into a moving machine: then we'd have to choose our 'legs', and this means two motors, one for each leg.  </p>");
    client.println("      <button class='back'> Inicio </button>");
    client.println("    </div>");
    client.println("    <div class='app3'>");
    client.println("      <h1> Second activity: Let's move the LLUBot </h1><br>");
    client.println("      <p> Introduce the number of cm to move Forward: <input type='text' id='input1'/> Click <a id='a1' href=''> here</a> to go. </p>");
    client.println("      <p> Introduce the number of cm to move Backward: <input type='text' id='input2'/> Click <a id='a2' href=''> here</a> to go. </p>");
    client.println("      <p> Introduce the number of degrees to turn Right: <input type='text' id='input3'/> Click <a id='a3' href=''> here</a> to go. </p>");
    client.println("      <p> Introduce the number of degrees to turn Left: <input type='text' id='input4'/> Click <a id='a4' href=''> here</a> to go. </p>");
    client.println("      <button class='back'> Inicio </button>");
    client.println("    </div>");
    client.println("    <script>");
    client.println("            let main = document.querySelector('.main');");
    client.println("            let app1 = document.querySelector('.app1');");
    client.println("            let app2 = document.querySelector('.app2');");
    client.println("            let app3 = document.querySelector('.app3');");
    client.println("            let btn1 = document.getElementById('btn1');");
    client.println("            let btn2 = document.getElementById('btn2');");
    client.println("            let btn3 = document.getElementById('btn3');");
    client.println("            let btnBack = document.querySelectorAll('.back');");
    client.println("            btn1.addEventListener('click', () => {");
    client.println("              main.style.display = 'none';");
    client.println("              app1.style.display = 'block';");
    client.println("            });");
    client.println("            btn2.addEventListener('click', () => {");
    client.println("              main.style.display = 'none';");
    client.println("              app2.style.display = 'block';");
    client.println("            });");
    client.println("            btn3.addEventListener('click', () => {");
    client.println("              main.style.display = 'none';");
    client.println("              app3.style.display = 'block';");
    client.println("            });");
    client.println("            btnBack.forEach( btn => {");
    client.println("              btn.addEventListener('click', () => {");
    client.println("                main.style.display = 'block';");
    client.println("                app1.style.display = 'none';");
    client.println("                app2.style.display = 'none';");
    client.println("                app3.style.display = 'none';");
    client.println("              });");
    client.println("            });");
    client.println("            let input1 = document.getElementById('input1');");
    client.println("            let input2 = document.getElementById('input2');");
    client.println("            let input3 = document.getElementById('input3');");
    client.println("            let input4 = document.getElementById('input4');");
    client.println("            let string = '';");
    client.println("            input1.addEventListener('input', () => string = \"/MOTOR=FORWARD-\" + input1.value );");
    client.println("            input2.addEventListener('input', () => string = \"/MOTOR=BACKWARD-\" + input2.value );");
    client.println("            input3.addEventListener('input', () => string = \"/MOTOR=RIGHT-\" + input3.value );");
    client.println("            input4.addEventListener('input', () => string = \"/MOTOR=LEFT-\" + input4.value );");
    client.println("            let link1 = document.getElementById('a1');");
    client.println("            let link2 = document.getElementById('a2');");
    client.println("            let link3 = document.getElementById('a3');");
    client.println("            let link4 = document.getElementById('a4');");
    client.println("            link1.addEventListener('click', () => link1.href = string);");
    client.println("            link2.addEventListener('click', () => link2.href = string);");
    client.println("            link3.addEventListener('click', () => link3.href = string);");
    client.println("            link4.addEventListener('click', () => link4.href = string);");
    client.println("    </script>");
    client.println("  </body>");
    client.println("</html>");
   
    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
  
    if(value == 1) {
      goStraight('F', mov);  
    } if(value == 2) {
      goStraight('B', mov);  
    } if(value == 3) {
      turn('R', mov);  
    } if(value == 4) {
      turn('L', mov);  
    } else {
      
    }
  }
}
