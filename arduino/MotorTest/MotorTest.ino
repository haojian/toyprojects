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
  servo1.attach(10);
  //servo2.attach(10);

  //servo12.attach(8);

  // turn on motor #2
  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);
  motor3.setSpeed(200);
  motor3.run(RELEASE);
  motor4.setSpeed(200);
  motor4.run(RELEASE);
}

int i;

// Test the DC motor, stepper and servo ALL AT ONCE!
void loop() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  servo1.write(i-255);
  for (i=0; i<255; i++) {
    servo1.write(i);
    //servo12.write(i);
    motor1.setSpeed(i);  
    motor2.setSpeed(i);  
    motor3.setSpeed(i);  
    motor4.setSpeed(i);  
    delay(3);
 }
 
  for (i=255; i!=0; i--) {
    servo1.write(i-255);
    //servo12.write(i-255);

    motor1.setSpeed(i);  
    motor2.setSpeed(i);  
    motor3.setSpeed(i);  
    motor4.setSpeed(i);  
    delay(3);
 }
 /*
  motor.run(BACKWARD);
  for (i=0; i<255; i++) {
    servo1.write(i);
    motor.setSpeed(i);  
    delay(3);
    stepper.step(1, FORWARD, DOUBLE);
 }
 
  for (i=255; i!=0; i--) {
    servo1.write(i-255);
    motor.setSpeed(i);  
    stepper.step(1, BACKWARD, DOUBLE);
    delay(3);
 }
 */
}
