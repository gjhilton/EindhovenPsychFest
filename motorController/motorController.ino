// quick and dirty code - needs refactoring

#include <Servo.h>

const int POT_A_PIN = 0;
const int POT_B_PIN = 1;
const int POT_DURATION_PIN = 2;

const int SWITCH_PIN_A = 2;
const int SWITCH_PIN_B = 3;

const int SWITCH_PIN_LOOP_TYPE = 4;

const int SERVO_PIN = 9;
const int SERVO_MAX = 0;
const int SERVO_MIN = 120;

const int LOOP_DELAY = 10;

const int DURATION_MIN = 500;
const int DURATION_MAX = 8000;

/////////////////////////////////////////////////////////////////////////////////////////////////
// STATE VARIABLES
////////////////////////////////////////////////////////////////////////////////////////////////

boolean is_cycling = false;
boolean a_to_b = false;

unsigned long cycleStartTime = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////
// DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////////////

enum Mode {OFF, A, B, CYCLE};
Servo servo;

/////////////////////////////////////////////////////////////////////////////////////////////////
// ARDUINO LIFECYCLE
////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  pinMode(SWITCH_PIN_A, INPUT);
  pinMode(SWITCH_PIN_B, INPUT);
  pinMode(SWITCH_PIN_LOOP_TYPE, INPUT);
  servo.attach(SERVO_PIN);
}

void loop() {
  int current_mode = getCurrentMode();
  switch (current_mode) {
  case OFF:
      is_cycling = false;
      break;
    case A:
      servoTo(analogRead(POT_A_PIN));
      is_cycling = false;
      break;
    case B:
      servoTo(analogRead(POT_B_PIN));
      is_cycling = false;
      break;
    case CYCLE:
      cycle();
      break;
  }
  delay(LOOP_DELAY);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////////////

void cycle(){
  
  //Serial.println("---------------------------");
  
  // if we've only just switched into cycle mode
  if (is_cycling == false){
    newCycle();
  }
  
  float duration = getCurrentDuration();
  unsigned long elapsed = millis() - cycleStartTime;
  
  //Serial.print("elapsed ");
  //Serial.println(elapsed);
  
  //Serial.print("duration ");
  //Serial.println(duration);
  
  // if we've reached the end of a cycle, reverse direction and start again
  if (elapsed > duration){
    newCycle();
    elapsed = 0;
  }
  
  //float a_val = 1023;
  //float b_val = 0;
  
  //float a_val = 0;
  //float b_val = 1023;
  
  float a_val = analogRead(POT_A_PIN);
  float b_val = analogRead(POT_B_PIN);
  
  float start; 
  float change;
  
  if (a_to_b) {
    start = a_val;
    change = b_val-a_val;
  } else {
    start = b_val;
    change = a_val-b_val;
  }
  
  int pos = int(inlineLinearInterp(elapsed, start, change, duration));
  servoTo(pos);
}

void newCycle(){
  int cycleLoopRewind = digitalRead(SWITCH_PIN_LOOP_TYPE);
  if (cycleLoopRewind == HIGH){
     // backwards and forwards 
     Serial.println ("NEW CYCLE: backwards and forwards");
     a_to_b = !(a_to_b); // reverse direction
  } else {
    // rewind to a and repeat
    Serial.println ("NEW CYCLE: rewind and repeat");
    a_to_b = true;
  }
  cycleStartTime = millis();
  is_cycling = true;
}

void servoTo(int analogueValue){
  int p = map(analogueValue, 0, 1023, SERVO_MIN, SERVO_MAX);
  servo.write(p);
}

int getCurrentMode() {
  int a = digitalRead(SWITCH_PIN_A);
  int b = digitalRead(SWITCH_PIN_B);
  if (a == HIGH) {
    if (b == HIGH) {
      return CYCLE;
    } else {
      return A;
    }
  } else {
    if (b == HIGH) {
      return B;
    }
  }
  return OFF;
}

int getCurrentDuration(){
  int duration = map(analogRead(POT_DURATION_PIN), 0, 1023, DURATION_MIN, DURATION_MAX);
  return duration;
}

inline float inlineLinearInterp (float t, float b, float c, float d){
	return c*t/d + b;
}
