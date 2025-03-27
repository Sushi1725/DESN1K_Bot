
#include <Servo.h>
Servo servo; // Servo on pin 9

void tiltPixy() {
  //get the (x,y) of balls onscreen
  //if they get to the bottom of framee and leave, tilt

  int pos = 0;
  servo.write(pos);
  delay(15);
}

void setup() {
  servo.attach(9); //servo on pin 9

  Serial.begin(115200);
  Serial.print("Starting...\n");
//  servo.write(15);
}

void loop() {
//  int pos = 0;
//  // sweep the servo from 0 to 180 degrees in steps
//  // of 1 degrees
//  for (pos = 15; pos <= 50; pos += 1) {
//    // tell servo to go to position in variable 'pos'
//    servo.write(pos);
//    // wait 15 ms for servo to reach the position
//    delay(15); // Wait for 15 millisecond(s)
//  }
//  for (pos = 50; pos >= 15; pos -= 1) {
//    // tell servo to go to position in variable 'pos'
//    servo.write(pos);
//    // wait 15 ms for servo to reach the position
//    delay(15); // Wait for 15 millisecond(s)
//  }


servo.write(50);
delay(2000);
servo.write(30);
delay(2000);

}
