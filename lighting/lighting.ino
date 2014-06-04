// analogue pins for potentiometers
const int  RED_POT = A0;
const int  GREEN_POT = A1;
const int  BLUE_POT = A2;
const int  WHITE_POT = A3;
const int  UV_POT = A4;
const int  IR_POT = A5;

// digital pins for LEDs
const int  RED_LED = 11;
const int  GREEN_LED = 10;
const int  BLUE_LED = 9;
const int  WHITE_LED = 6;
const int  UV_LED = 5;
const int  IR_LED = 3;

// cycle frequency
const int LOOP_DELAY = 5;

int INPUTS[] = {RED_POT, GREEN_POT, BLUE_POT, WHITE_POT, UV_POT, IR_POT};
int LEDS[] = {RED_LED, GREEN_LED, BLUE_LED, WHITE_LED, UV_LED, IR_LED};
const int CONTROL_COUNT = 6;


void setup() {
  for (int i = 0; i < CONTROL_COUNT; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < CONTROL_COUNT; i++) {
    //debug();
    analogWrite(LEDS[i], analogRead(INPUTS[i])/4);
    delay(5);
  }
  delay(LOOP_DELAY);
}

void debugInput(int pin, String name){
    int sensorReading = analogRead(pin);
    sensorReading = sensorReading / 4;
    Serial.print(name);
    Serial.print(":");
    Serial.println(sensorReading);
}

void debug(){
   Serial.println("-----");
  debugInput(RED_POT, "RED_POT");
  debugInput(GREEN_POT, "GREEN_POT");
  debugInput(BLUE_POT, "BLUE_POT");
  debugInput(WHITE_POT, "WHITE_POT");
  debugInput(UV_POT, "UV_POT");
  debugInput(IR_POT, "IR_POT");
}
