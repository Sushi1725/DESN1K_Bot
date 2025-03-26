/* Food Delivery Robot | King Crab | Team 8

   This is the code for our food delivery robot.

*/

#include <Pixy2.h>
#include <SPI.h>
Pixy2 pixy; // Pixycam takes pins 10, 11, 12, and 13

#include <Servo.h>
Servo servo; // Servo on pin 9

const int dirA = 4;   // Direction control on digital pin 4
const int pwmA = 5;   // PWM signal on digital pin 5 <-- has pwm

const int dirB = 2;   // Direction control on digital pin 2
const int pwmB = 3;   // PWM signal on digital pin 3 <-- has pwm


//The most important method in the Arduino library is getBlocks(), which returns the number of objects Pixy has detected. You can then look in the
//pixy.ccc.blocks[] array for information about each detected object (one array member for each detected object.) Each array member (i) contains the following fields:
//
//pixy.ccc.blocks[i].m_signature The signature number of the detected object (1-7 for normal signatures)
//pixy.ccc.blocks[i].m_x The x location of the center of the detected object (0 to 316)
//pixy.ccc.blocks[i].m_y The y location of the center of the detected object (0 to 208)
//pixy.ccc.blocks[i].m_width The width of the detected object (1 to 316)
//pixy.ccc.blocks[i].m_height The height of the detected object (1 to 208)
//pixy.ccc.blocks[i].m_angle The angle of the object detected object if the detected object is a color code (-180 to 180).
//pixy.ccc.blocks[i].m_index The tracking index of the block
//pixy.ccc.blocks[i].m_age The number of frames the block has been tracked.
//pixy.ccc.blocks[i].print() A member function that prints the detected object information to the serial port

// Function Prototypes
int stateMachine_calculateBalls();
int stateMachine();


void setup() {
  pinMode(dirA, OUTPUT); //pin 4
  pinMode(pwmA, OUTPUT); //pin 5
  pinMode(dirB, OUTPUT); //pin 2
  pinMode(pwmB, OUTPUT); //pin 3

  pixy.init();
  servo.attach(9); //servo on pin 9

  Serial.begin(115200);
  Serial.print("Starting...\n");
}

void loop() {
  
  int newVar = stateMachine_calculateBalls();
  Serial.println(newVar);
  

}


  if (pixy.ccc.numBlocks) { // if there are any balls in the FOV --> numBlocks is the amount of blocks    
      if (pixy.ccc.blocks[i].m_signature == 4) { // if signature is 4 (base colour)
        // go to base????
      }


int stateMachine_calculateBalls() { // calculate the net points of balls in the claws
  int netPoints = 0;

  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) { // if there are any balls in the FOV --> numBlocks is the amount of blocks    
    for (int i = 0; i < pixy.ccc.numBlocks; i++) { // for each ball in FOV,
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();

      if (pixy.ccc.blocks[i].m_signature == 1 || pixy.ccc.blocks[i].m_signature == 2) { // if signature (ball) is 1 or 2 (green/blue)
        netPoints ++;
      } else if (pixy.ccc.blocks[i].m_signature == 3) { // if signature is 3 (red)
        netPoints += 2;
      }
    }
  }
  return netPoints;
}

int stateMachine() {
  int netPoints = stateMachine_calculateBalls();
  
  if (netPoints < 0) {
    //discard (to some other base)  
  } else if (netPoints == 0) {
    //dont care and discard  OR  continue grabbing balls
  } else if (netPoints > 0) {
    // go deposit  OR  continue grabbing balls
  }
}

void tiltPixy() {
  //get the (x,y) of balls onscreen
  //if they get to the bottom of framee and leave, tilt

  int pos = 0;
  servo.write(pos);
  delay(15);
}

void motorForwards() {
  int motorSpeed = 128; // 50% duty cycle (128 out of 255)
  digitalWrite(dirA, HIGH); // forwards direction
  digitalWrite(dirB, HIGH); // forwards direction
  analogWrite(pwmA, motorSpeed);
  analogWrite(pwmB, motorSpeed);

  delay(15);
}
