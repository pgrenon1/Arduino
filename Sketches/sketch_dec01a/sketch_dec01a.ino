int sensor = 2;
int lights = 3;
int button = 4;
int val = 0;
enum Mode { OFF, ON, BLINK, MOTION, MOTION_BLINK };
Mode mode = OFF;
int state = 0;
int previousState = 0;

void setup() {
  pinMode(lights, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
  Serial.begin(9600);
  mode = BLINK;
}

void loop() {
  ChangeState();
  
  switch (mode) {
    case OFF:
      digitalWrite(lights, LOW);
      break;
    case ON:
      digitalWrite(lights, HIGH);
      break;
    case BLINK:
      UpdateBlink();
      break;
    case MOTION:
      UpdateMotion();
      break;
  }

  ApplyState();
}

void ChangeState(){
  
}

void ApplyState() {
  if (state == previousState) {
    return;
  }

  if (state == 0) {
    digitalWrite(lights, LOW);
  }
  else {
    digitalWrite(lights, HIGH);
  }

  previousState = state;
}

void UpdateMotion() {
  val = digitalRead(sensor);

  if (val == HIGH) {
    state = 1;

    delay(300000);
  }
  else {
    state = 0;
  }
}

void UpdateBlink() {
  if (state == 0) {
    delay(1000);
    state = 1;
  }
  else {
    delay(1000);
    state = 0;
  }
}
