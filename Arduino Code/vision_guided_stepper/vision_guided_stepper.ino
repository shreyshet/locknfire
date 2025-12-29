#include <Servo.h>
#include <Stepper.h>

 

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);  // Adjust pins
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position
int ss = 0;
int sw = 1;
int prev_sw = 0;
unsigned long lastDebounceTime = 0; // Last time the button state was toggled
unsigned long debounceDelay = 50;   // Debounce time in milliseconds
unsigned long previousMillis = 0;
unsigned long interval = 100UL;

const int heartbeatPin = 13;   // Internal LED
const int dataLedPin = 12;      // External LED for Serial check
unsigned long lastHeartbeat = 0;
unsigned long dataLedTimer = 0;
bool heartbeatState = LOW;

int p1 = 10;
int p2 = 170;
int count = 0;
int button_Pin = 4;
int servo_Pin = 5;
int shooter_Pin = 3;
int LED_PIN = 13;
int magsize = 5;
int flag = 0;
int stepper_stepsize_large = 6;
int stepper_stepsize_small = 2; 

void setup() {
  
  delay(2000);
  pinMode(heartbeatPin, OUTPUT);
  pinMode(dataLedPin, OUTPUT);
  
  myservo.attach(servo_Pin);  // attaches the servo on pin 9 to the servo object
  pinMode(button_Pin, INPUT_PULLUP);
  pinMode(shooter_Pin, OUTPUT);
  digitalWrite(shooter_Pin, HIGH);
  Serial.begin(9600);
  myStepper.setSpeed(50);
  flag = magsize;

  // Quick startup flash
  for(int i=0; i<3; i++) {
    digitalWrite(dataLedPin, HIGH); delay(100);
    digitalWrite(dataLedPin, LOW); delay(100);
  }
  
}

 

void loop() {
  // DEBUG Actuators
  sw = digitalRead(button_Pin);
  if (sw == 0){
    delay(1000);
    myStepper.step(stepper_stepsize_large);
    myStepper.step(-stepper_stepsize_large);
    triggerShooter(10, 1);
    triggerReload();
    triggerShooter(10, 0);
  }

  //// --- HEARTBEAT LOGIC ---
  // Blinks every 500ms to show the code is running
  if (millis() - lastHeartbeat > 500) {
    heartbeatState = !heartbeatState;
    digitalWrite(heartbeatPin, heartbeatState);
    lastHeartbeat = millis();
  }
  
  //digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(shooter_Pin, HIGH);
  
  if (Serial.available()) {
    char command = Serial.read();
    digitalWrite(dataLedPin, HIGH);
    dataLedTimer = millis();
    if (command == 'L' or command == 'l' or command == 'R' or command == 'r' or command == 'F'){
      handleCommand(command);
    } 
   }
   if (millis() - dataLedTimer > 50) {
    digitalWrite(dataLedPin, LOW);
  }
}

void handleCommand(char command) {
  // Function to handle legit commands from arduino
  
  if (command == 'L') {
        myStepper.step(-stepper_stepsize_large);  // Move left large
        //delay(200); 
      }
      else if (command == 'l') {
        myStepper.step(-stepper_stepsize_small);  // Move left small 
        //delay(200);
      }
      else if (command == 'R') {
        myStepper.step(stepper_stepsize_large);   // Move right Large 
        //delay(200);
      }
      else if (command == 'r') {
        myStepper.step(stepper_stepsize_small);   // Move right small
        //delay(200);
      } 
      else if (command == 'S') {
        //delay(200);
      }
      else if (command == 'F'){
          triggerShooter(10, 1);
          triggerReload();
          triggerShooter(10, 0);
          delay(200);
      }
}


// Function to trigger the relay for a specified duration
void triggerShooter(unsigned long duration, int state) {
  if(state == 1){
      digitalWrite(shooter_Pin, LOW);  // Turn relay ON
      //delay(duration);
  }
  else{
  digitalWrite(shooter_Pin, HIGH);   // Turn relay OFF
  }
}

 

void triggerReload()
{
  for (pos = p2; pos >= p1; pos -= 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
  for (pos = p1; pos <= p2; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
}
