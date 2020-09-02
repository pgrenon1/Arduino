/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/
int Led = 13 ; // define LED Interface
int Shock = 3; // define the percussion Sensor Interface
int val ; // define numeric variables val

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode (Led, OUTPUT) ; // define LED as output interface
  pinMode (Shock, INPUT) ; // define knock sensor output interface
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  val = digitalRead (Shock) ; // read digital interface is assigned a value of 3 val
  if (val == HIGH){
      Serial.println(0);
  } else {
      Serial.println(val);
  }
  // print out the state of the button:  Serial.println(val);
  delay(1);        // delay in between reads for stability
}
