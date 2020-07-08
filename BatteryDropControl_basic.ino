#define ledPin D2
#define voltagePin D9

void setup() {
  //start serial connection
  Serial.begin(115200);
  pinMode(voltagePin, INPUT);
  pinMode(ledPin, OUTPUT);

}

void loop() {
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(voltagePin);
  //print out the value of the pushbutton
  Serial.println(!sensorVal);

  // Keep in mind the pull-up means the input pin's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin the LED in D2 when the
  // voltage has dropped, and off when it has not:
  if (sensorVal == 0) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }
}
