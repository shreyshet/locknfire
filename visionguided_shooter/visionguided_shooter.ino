#include <Servo.h>

#include <Stepper.h>

 
#define shooter_off HIGH
#define shooter_on LOW

const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);  // Adjust pins

 

Servo myservo;  // create servo object to control a servo

// twelve servo objects can be created on most boards

 

int pos = 0;    // variable to store the servo position
int ss = 0;
int sw = 0;
int prev_sw = 0;
unsigned long lastDebounceTime = 0; // Last time the button state was toggled
unsigned long debounceDelay = 50;   // Debounce time in milliseconds

int p1 = 0;
int p2 = 160;

 

int count = 0;


int button_Pin = 4;
int servo_Pin = 5;
int shooter_Pin = 3;
int LED_PIN = 13;

 

void setup() {

  myservo.attach(servo_Pin);  // attaches the servo on pin 9 to the servo object

  pinMode(button_Pin, INPUT_PULLUP);
  pinMode(shooter_Pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  myStepper.setSpeed(50);
  pinMode(LED_BUILTIN, OUTPUT);
}

 

void loop() {

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(shooter_Pin, shooter_off);
  // Servo
  // Stepper
  if (Serial.available()) {
    char command = Serial.read();

    if (command == 'L') {
      myStepper.step(-10);  // Move left
      digitalWrite(LED_BUILTIN, LOW);
      count = 0;
    }

    else if (command == 'l') {
      myStepper.step(-2);  // Move left
      digitalWrite(LED_BUILTIN, LOW);
      count = 0;
    }

    else if (command == 'R') {
      myStepper.step(10);   // Move right
      digitalWrite(LED_BUILTIN, HIGH);
      count = 0;
      digitalWrite(LED_BUILTIN, HIGH);
        
    }

    else if (command == 'r') {
      myStepper.step(2);   // Move right
      digitalWrite(LED_BUILTIN, HIGH);
      count = 0;
      

    } else if (command == 'S') {
      count++;
      if(count >= 10){
        //ignore
      }

      else if(count >= 9){
        delay(1000);
        triggerShooter(10, 1);
        triggerReload();
        triggerShooter(10, 0);
      }
    }
  }

 

    //myStepper.step(-100);
    //delay(2000);
    //Serial.write("negative");
    //myStepper.step(+100);
    //Serial.write("positive");
    //delay(200);
}




// Function to trigger the relay for a specified duration

void triggerShooter(unsigned long duration, int state) {
  if(state == 1){
      digitalWrite(shooter_Pin, shooter_on);  // Turn relay ON
      //delay(duration);
  }
  else{
  digitalWrite(shooter_Pin, shooter_off);   // Turn relay OFF
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
