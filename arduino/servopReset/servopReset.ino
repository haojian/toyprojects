// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>
#include <Servo.h> 

// DC motor on M2
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// DC hobby servo
Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor party!");
  
  // turn on servo
  servo1.attach(9);
  servo2.attach(10);
}

int i;

// Reset servo ALL AT ONCE!
void loop() {
  servo1.write(90);
  servo2.write(90);
}
