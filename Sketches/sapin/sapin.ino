#include <timer.h>
#include <avdweb_Switch.h>
#include <math.h>

#define PI 3.14159265

int sensor = 2;
int lights = 3;
int buttonV = 7;
Switch button = Switch(8);
auto timer = timer_create_default();
int val = 0;
enum Mode { OFF, ON, BLINK, MOTION };
Mode mode = OFF;
int state = 0;
int previousState = 0;
int startingTime = 0;

bool isMotionOn;

void setup() {
  pinMode(lights, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
  pinMode(buttonV, OUTPUT);
  digitalWrite(buttonV, LOW);
  Serial.begin(9600);
  mode = ON;
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

void ChangeState() {
  button.poll();

  if (button.pushed()) {
    Serial.write("PUSHED");
    switch (mode) {
      case OFF:
        mode = ON;
        break;
      case ON:
        mode = BLINK;
        break;
      case BLINK:
        mode = MOTION;
        break;
      case MOTION:
        mode = OFF;
        break;
    }
  }
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

  if (val == HIGH && state == 0) {
    state = 1;
    timer.in(180000, MotionEnd);
  }

  timer.tick();
}

bool MotionEnd(void *argument) {
  state = 0;
  return true;
}

bool isTimerStarted = false;
unsigned long counter = 0;
unsigned long previousMillis = 0;
void UpdateBlink() {
  if (!isTimerStarted) {
    counter = 0;
    startingTime = millis();
    isTimerStarted = true;
  }

  if (isTimerStarted) {
    if (counter >= 1000) {
      isTimerStarted = false;
      ToggleLightsState();
    }
    else {
      unsigned long timePassed = millis() - previousMillis;
      previousMillis = millis();
      //      Serial.println(timePassed);
      counter += timePassed;
    }
  }
}

void ToggleLightsState() {
  if (state == 1)
    state = 0;
  else
    state = 1;

}
