int speedValue = 100;
int fanControlPin = 9;
String readString;

void setup()
{
  TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
  //  TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
  //TCCR1B = TCCR1B & B11111000 | B00000100;    // set timer 1 divisor to   256 for PWM frequency of   122.55 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz

  Serial.begin(115200);
  pinMode(fanControlPin, OUTPUT);
}

void loop()
{
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }

  if (readString.length() > 0) {
    Serial.print("Changing speed to: ");
    Serial.println(readString);

    speedValue = readString.toInt();

    analogWrite(fanControlPin, speedValue);
    readString="";
  }
}
