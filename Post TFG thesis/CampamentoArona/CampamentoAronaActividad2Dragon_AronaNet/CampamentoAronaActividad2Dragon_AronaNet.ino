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
float calculatedSteps = 0;

//const char* ssid = "TP-Link_6E7E";
//const char* password = "09207189";

const char* ssid = "AronaNet";
const char* password = "AronaNet";

WiFiServer server(80);

void movement (char way, float steps){
  char way_ = way;
  float steps_ = steps;
  if(way_ == 'F') {
    //Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, FORWARD, SINGLE); 
      motorL.step(1, FORWARD, SINGLE); 
    }
  }
  if(way_ == 'B') {
    //Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, BACKWARD, SINGLE); 
      motorL.step(1, BACKWARD, SINGLE); 
    }
  }
  if(way_ == 'R') {
    //Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, BACKWARD, SINGLE); 
      motorL.step(1, FORWARD, SINGLE); 
    }
  }
  if(way_ == 'L') {
    //Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, FORWARD, SINGLE); 
      motorL.step(1, BACKWARD, SINGLE);
    }
  }
  else {
  
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
  movement(way_, steps);
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
  movement(way_, steps);
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
  int sensorVal = 1; // digitalRead(voltagePin);
  
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
    client.println("<html>");
    client.println("  <head>");
    client.println("    <title> LLUBot </title>");
    client.println("    <style>");
    client.println("      body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana;}");
    client.println("        .main button {  height: 50px; width: 50%; font-size: 25px; }");
    client.println("        .buttonforward {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}");
    client.println("        .buttonright {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}");
    client.println("        .buttondown {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}");
    client.println("        .buttonleft {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}");
    client.println("        .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}");
    client.println("        .buttongo {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}");
    client.println("        .app1 button { font-size: 25px; }");
    client.println("      h1 {font-size: 50px;}");
    client.println("      a {color: white; text-decoration: none}");
    client.println("      body {font-size: 30px;}");
    client.println("    </style>");
    client.println("  </head>");
    client.println("  <body>");
    client.println("    <div class='main'>");
    client.println("      <h1> LLUBot en el Laboratorio de Verano de Ciencia y Tecnolog&iacutea de Arona</h1>");
    client.println("      <p> Pulsar el siguiente bot&oacuten para entrar en la explicaci&oacuten de esta actividad.</p>");
    client.println("      <button id='btn1'> Actividad 2: Explicaci&oacuten </button><br><br>");
    client.println("      <div class= 'buttonforward'> Avanzar <input class='num' type='text' id='input1'/> micropasos <br></br><button class= 'buttongo'><a id='a1' href='' color: white> Go! </a></button></div><br></br>");
    client.println("      <div class= 'buttonleft'> Girar <input class='num' type='text' id='input4'/> micropasos a la izquierda <br></br><button class= 'buttongo'><a id='a4' href=''> Go! </a></button></div><br></br>");
    client.println("      <div class= 'buttonright'> Girar <input class='num' type='text' id='input3'/> micropasos a la derecha <br></br><button class= 'buttongo'><a id='a3' href=''> Go! </a></button></div><br></br>");
    client.println("      <div class= 'buttondown'> Retroceder <input class='num' type='text' id='input2'/> micropasos <br></br><button class= 'buttongo'><a id='a2' href=''> Go! </a></button></div><br></br><br></br>");
    client.println("    <img src='https://luisanalarab.files.wordpress.com/2020/07/cuadrc3adcula-mal-hecha.png?w=1024' width='700' height='700'>"); 
    client.println("    </div>");
    client.println("    <div class='app1'>");
    client.println("      <h1> Actividad 2 </h1>");
    client.println("      <h2> Actividad 2.1: &iquestPara qu&eacute usamos las mates en Rob&oacutetica? </h2>");
    client.println("        <p> Geometr&iacutea de un c&iacuterculo: di&aacutemetro y per&iacutemetro. </p>");
    client.println("          <img src='https://www.pequeocio.com/wp-content/uploads/2019/02/perimetro-circunferencia.jpg'>");
    client.println("        <p> Di&aacutemetro de las ruedas de un LLUBot = 3,2 cm. </p>");
    client.println("        <p> &iquestCu&aacutento mide el per&iacutemetro? </p>");
    client.println("      <h2> Actividad 2.2: LLUBot y sus motores </h1>");
    client.println("        <p> Motores: steppers (paso a paso) y su funcionamiento con 4 pasos. </p>");
    client.println("          <img src='https://media.monolithicpower.com/wysiwyg/1_11.png'>");
    client.println("        <p> El motor de los LLUBots tiene 256 pasos para realizar una vuelta completa y cada &oacuterden de movimiento es 1/8 paso, llamado micropaso, por lo que, &iquestcu&aacutentos micropasos hacen falta para que d&eacute una vuelta entera el motor? </p>");
    client.println("        <p> Por tanto, &iquestcu&aacutel es el n&uacutemero de micropasos que hacen que el LLUBot avance 1 cm? </p>");
    client.println("        <p> Si tenemos 2 motores, &iquestc&oacutemo hacemos para que gire hacia la derecha o hacia la izquierda? </p>");
    client.println("      <button class='back'> Inicio </button>");
    client.println("      <h2> Actividad 2.3: LLUBot y sus giros </h1>");
    client.println("        <p> Para calcular el &aacutengulo de giro del LLUBot hace falta conocer la distancia entre sus ruedas, como vemos a continuaci&oacuten: </p>");
    client.println("          <img src='https://luisanalarab.files.wordpress.com/2020/07/relacic3b3n_giro_robot_mc3b3vil.png'>");
    client.println("        <p> Si ya sabemos cu&aacutentos micropasos son 1 cm, sabemos la relaci&oacuten de giro. Y si sabemos que el per&iacutemetro del centro de giro de nuestro LLUBot es como vemos arriba son los 360&deg totales de giro, y b = 15,2 cm... </p>");
    client.println("        <p> &iquestCu&aacutentos micropasos hay que dar para que gire 90&deg? </p>");
    client.println("      <button class='back'> Inicio </button>");
    client.println("    </div>");
    client.println("    <script>");
    client.println("       let main = document.querySelector('.main');");
    client.println("       let app1 = document.querySelector('.app1');");
    client.println("       let btnBack = document.querySelectorAll('.back');");
    client.println("       btn1.addEventListener('click', () => {");
    client.println("          main.style.display = 'none';");
    client.println("          app1.style.display = 'block';");
    client.println("       });");
    client.println("       btnBack.forEach( btn => {");
    client.println("          btn.addEventListener('click', () => {");
    client.println("            main.style.display = 'block';");
    client.println("            app1.style.display = 'none';");
    client.println("          });");
    client.println("       });");
    client.println("       let input1 = document.getElementById('input1');");
    client.println("       let input2 = document.getElementById('input2');");
    client.println("       let input3 = document.getElementById('input3');");
    client.println("       let input4 = document.getElementById('input4');");
    client.println("       let string = '';");
    client.println("       input1.addEventListener('input', () => string = \"/MOTOR=FORWARD-\" + input1.value );");
    client.println("       input2.addEventListener('input', () => string = \"/MOTOR=BACKWARD-\" + input2.value );");
    client.println("       input3.addEventListener('input', () => string = \"/MOTOR=RIGHT-\" + input3.value );");
    client.println("       input4.addEventListener('input', () => string = \"/MOTOR=LEFT-\" + input4.value );");
    client.println("       let link1 = document.getElementById('a1');");
    client.println("       let link2 = document.getElementById('a2');");
    client.println("       let link3 = document.getElementById('a3');");
    client.println("       let link4 = document.getElementById('a4');");
    client.println("       link1.addEventListener('click', () => link1.href = string);");
    client.println("       link2.addEventListener('click', () => link2.href = string);");
    client.println("       link3.addEventListener('click', () => link3.href = string);");
    client.println("       link4.addEventListener('click', () => link4.href = string);");
    client.println("    </script>");
    client.println("  </body>");
    client.println("</html>");
 
   
    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
  
    if(value == 1) {
      movement('F', mov);  
    } if(value == 2) {
      movement('B', mov);  
    } if(value == 3) {
      movement('R', mov);  
    } if(value == 4) {
      movement('L', mov);  
    } else {
      
    }
  }
}
