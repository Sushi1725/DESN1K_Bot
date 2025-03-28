/*
Food Delivery Robot | King Crab | Team 8

This is the code for our food delivery robot.
*/

#include <Pixy2.h>
#include <SPI.h>
#include <Servo.h>

Pixy2 pixy; // Pixycam takes pins 10, 11, 12, and 13
Servo servo; // Servo on pin 9

const int dirA = 4;     // Direction control on digital pin 4
const int pwmA = 5;     // PWM signal on digital pin 5 <-- has pwm

const int dirB = 2;     // Direction control on digital pin 2
const int pwmB = 3;     // PWM signal on digital pin 3 <-- has pwm

const int trigPin = 8; // trigger doesnt need pwm so pin 8
const int echoPin = 7; // echo doesnt need pwn so pin 7

unsigned long lastTick = 0;
const long tickInterval = 50; // lets set every tick to be 50ms ifykyk

const int redSignature = 3;
const int baseSignature = 4;

// set function prototypes
void setPixyCam(String position);
void motorRotate(int time = 15);


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

// !TODO - Check seeking function, need a way to distinguish between balls and base
void seekObject(int signature){
    // idea is to rotate the robot until the objectis rougly in the middle of the frame
    setPixyCam("UP");
    while (true) {
        pixy.ccc.getBlocks(); // update the blocks each while instance
        Serial.println(pixy.ccc.numBlocks);
        if (pixy.ccc.numBlocks) {
            for (int i = 0; i < pixy.ccc.numBlocks; i++) {
                if (pixy.ccc.blocks[i].m_signature == signature) {
                    // maybe add check here to make sure its in the centre of the frame rougly
                    return;
                } else {
                    motorRotate();
                }
            }
        }
    }
}


int getNetPoints() { // calculate the net points of balls in the claws
    setPixyCam("DOWN");
    int netPoints = 0;

    pixy.ccc.getBlocks();

    if (pixy.ccc.numBlocks) { // if there are any balls in the FOV --> numBlocks is the amount of blocks
        for (int i = 0; i < pixy.ccc.numBlocks; i++) { // for each ball in FOV,
            Serial.print("    block ");
            Serial.print(i);
            Serial.print(": ");
            pixy.ccc.blocks[i].print();

            if (pixy.ccc.blocks[i].m_signature == 1 || pixy.ccc.blocks[i].m_signature == 2) { // if signature (ball) is 1 or 2 (green/blue)
                netPoints++;
            } else if (pixy.ccc.blocks[i].m_signature == redSignature) { // if signature is 3 (red)
                netPoints -= 2;
            }
        }
    }
    return netPoints;
}

// !TODO - get the two pixycam positions and swap between them
void setPixyCam(String position) {
    if (position = "DOWN"){
        //down position
        int pos = 30;
        servo.write(pos);
        delay(15);
    } else if (position = "UP"){
        // up position
        int pos = 50;
        servo.write(pos);
        delay(15);
    }
}

// MOTOR FUNCTIONS
void motorForwards(int time = 15) {
    // move forward time ms (default 15 ms)
    int motorSpeed = 128; // 50% duty cycle (128 out of 255)
    digitalWrite(dirA, HIGH); // forwards direction
    digitalWrite(dirB, HIGH); // forwards direction
    analogWrite(pwmA, motorSpeed);
    analogWrite(pwmB, motorSpeed);

    delay(time); //! TODO - Issue where it can cause hangups in main thread --> use ticks to handle rather than delay

    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);
}

// !TODO - Test & debug motor turning
void motorRotate(int time = 15) {
    // move forward time ms (default 15 ms)
    int motorSpeed = 128; // 50% duty cycle (128 out of 255)
    digitalWrite(dirA, HIGH); // forwards direction
    digitalWrite(dirB, LOW); // reverse direction
    // overall should turn?
    analogWrite(pwmA, motorSpeed);
    analogWrite(pwmB, motorSpeed);

    delay(time); //! TODO - Issue where it can cause hangups in main thread --> use ticks to handle rather than delay


    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);   // causes jittery wheels
}

int getFrontDistance() {
    float duration, distance; // duration and distance are both floats

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    Serial.println(distance);

    return distance;
}

void setup() {    
    // PIN SETUP
    pinMode(dirA, OUTPUT); //pin 4
    pinMode(pwmA, OUTPUT); //pin 5
    pinMode(dirB, OUTPUT); //pin 2
    pinMode(pwmB, OUTPUT); //pin 3

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    pixy.init();

    servo.attach(9); //servo on pin 9

    Serial.begin(115200);
    Serial.println("Starting Crab King...");
}

void loop() {
    unsigned long currentTick = millis(); // get current time in milliseocnds
    Serial.println("here");
    delay(15);  
    if (currentTick - lastTick >= tickInterval){
        // tick elapsed, run some code...

        // check for obstacles
        if (getFrontDistance() <= 10) {
            // stop/run away
            motorRotate(100);
        } else {
            int netPoints = getNetPoints();

            if (netPoints < 0) {
                //discard (to some other base)
                seekObject(1); // find some random base lol - ie. colour signature anything other than base?
            } else if (netPoints == 0) {
                //dont care and discard    OR    continue grabbing balls
                seekObject(1);
            } else if (netPoints > 0) {
                // go deposit    OR    continue grabbing balls
                seekObject(baseSignature);
            }
        }
    }
}
