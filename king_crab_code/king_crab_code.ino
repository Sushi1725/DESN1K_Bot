/*
Food Delivery Robot | King Crab | Team 8

This is the code for our food delivery robot.
*/

// %%%%%%%%%%%%%%%%%% Between Each Run, change the following %%%%%%%%%%%%%%%%%%
// setPixyCam()'s angles
// ballIntoClaws()'s frame threshold
// motorForwards()'s motor speeds --> change to account for any curved path
// centerObject()'s threshold
// %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%

#include <Pixy2.h>
#include <SPI.h>
#include <Servo.h>

Pixy2 pixy; // Pixycam takes pins 10, 11, 12, and 13
Servo servo; // Servo on pin 9

#define dirA 4     // Direction control on digital pin 4
#define pwmA 5     // PWM signal on digital pin 5 <-- has pwm

#define dirB 2     // Direction control on digital pin 2
#define pwmB 3     // PWM signal on digital pin 3 <-- has pwm

#define trigPin 8 // trigger doesnt need pwm so pin 8
#define echoPin 7 // echo doesnt need pwm so pin 7

unsigned long lastTick = 0;
const long tickInterval = 50; // lets set every tick to be 50ms ifykyk

#define FRAME_BOTTOM 207  // Pixy2 screen height (bottom of the frame)
// Check if object was near bottom and is now gone
bool wasAtBottom = false;

// %%%%%%%%%% Signatures %%%%%%%%%%
const int green = 1;
const int blue = 2;
const int red = 3;
const int ourBase = 4;
const int rightBase = 5; //base to the right of our home base --> 1st base in baseball
const int oppBase = 6; // base opposite of our home base --> 2nd base in baseball
const int leftBase = 7; //base to the left of our home base --> 3rd base in baseball

// set function prototypes
void setPixyCam(String position);
void motorRotate(int time = 15);
void centerObject(int signature, int i);
void motorForwards(int time = 15);

// !TODO - Check seeking function, need a way to distinguish between balls and base
void seekObject(int signature){
    // idea is to rotate the robot until the objectis rougly in the middle of the frame
    setPixyCam("UP");
    bool seenSignature = false;
    while (true) {
        // check for obstacles
        if (getFrontDistance() <= 20) { // length of claws alrdy 12cm
            // stop/run away
            motorRotate(1000); //maybe change 1000 depending on how much it turns in given time
            motorStop(); //stop rotating
            motorForwards();
        }
      
        pixy.ccc.getBlocks(); // update the blocks each while instance

        if (pixy.ccc.numBlocks) { //if any blocks
            for (int i = 0; i < pixy.ccc.numBlocks; i++) {
                if (pixy.ccc.blocks[i].m_signature == signature) {
                    motorStop();
                    //centerObject(signature, i);
                    //motorForwards();
                    seenSignature = true;
                    // maybe add check here to make sure its in the centre of the frame rougly
                    centerObject(signature, i);
                    return;
                }
            }

            if (!seenSignature) {
              motorRotate(200);
            }
        } else {
            // possible that pixycam just doesnt detect anything at all
            delay(200); //delay half a second to make sure pixycam not being dumb
            pixy.ccc.getBlocks(); // update the blocks each while instance
            if (!pixy.ccc.numBlocks) {
                motorRotate(200); // if still no blocks rotate
            } else {
                for (int i = 0; i < pixy.ccc.numBlocks; i++) {
                    if (pixy.ccc.blocks[i].m_signature == signature) {
                        motorStop();
                        //centerObject(signature, i);
                        //motorForwards();
                        seenSignature = true;
                        // maybe add check here to make sure its in the centre of the frame rougly
                        centerObject(signature, i);
                        return;
                    }
                }

                if (!seenSignature) {
                motorRotate(200);
                }
            }
        }
    }
}

void centerObject(int signature, int blockIndex) {
    Serial.println("Starting centering process...");
    int centerX = 158;
    int threshold = 20;
    
    while (true) {
        pixy.ccc.getBlocks();
        if (pixy.ccc.numBlocks) {
//            for (int i = 0; i < pixy.ccc.numBlocks; i++) {
                if (pixy.ccc.blocks[blockIndex].m_signature == signature) {
                    int objectX = pixy.ccc.blocks[blockIndex].m_x;
                    Serial.print("Object X Position: ");
                    Serial.println(objectX);
                    
                    if (objectX < centerX - threshold) {
                        Serial.println("Turning Left...");
                        motorRotate(-10);
                        motorStop();
                    } else if (objectX > centerX + threshold) {
                        Serial.println("Turning Right...");
                        motorRotate(10);
                        motorStop();
                    } else {
                        Serial.println("Object Centered!");
                        motorStop();
                        return;
                    }
                }
//            }
        }
    }
}

int getNetPoints() { // calculate the net points of balls in the claws
    motorStop(); //freeze
    setPixyCam("DOWN"); //look down
    int netPoints = 0;

    pixy.ccc.getBlocks();

    if (pixy.ccc.numBlocks) { // if there are any balls in the FOV --> numBlocks is the amount of blocks
        for (int i = 0; i < pixy.ccc.numBlocks; i++) { // for each ball in FOV,
            Serial.print("    block ");
            Serial.print(i);
            Serial.print(": ");
            pixy.ccc.blocks[i].print();

            if (pixy.ccc.blocks[i].m_signature == green || pixy.ccc.blocks[i].m_signature == blue) { // if signature (ball) is 1 or 2 (green/blue)
                netPoints++;
            } else if (pixy.ccc.blocks[i].m_signature == red) { // if signature is 3 (red)
                netPoints -= 2;
            }
        }
    } else {
        netPoints = 0;
    }
    return netPoints;
}

// !TODO - get the two pixycam positions and swap between them
void setPixyCam(String position) {
    if (position == "DOWN"){
        //down position
        int pos = 130;
        servo.write(pos);
        delay(15);
    } else if (position == "UP"){
        // up position
        int pos = 160;
        servo.write(pos);
        delay(15);
    }
}

bool ballIntoClaws() {
    while (true) {
        if (getFrontDistance() <= 20) { // length of claws alrdy 12cm
            // stop/run away
            motorRotate(1000); //maybe change 1000 depending on how much it turns in given time
            motorStop(); //stop rotating
            motorForwards();
        }
        pixy.ccc.getBlocks();
        int maxY = 0;  // the highest the ball can be on screen
        if (pixy.ccc.numBlocks && !wasAtBottom) { //if any balls and wasAtBottom is still false
            for (int i = 0; i < pixy.ccc.numBlocks; i++) {
                pixy.ccc.blocks[i].print();
                if (pixy.ccc.blocks[i].m_signature != (4 && 5 && 6 && 7)) { //any signature other than 4 (4 is base; 1,2,3 are balls) ?%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    maxY = max(maxY, pixy.ccc.blocks[i].m_y); // y coord is from the center of the detected object
                    Serial.println(maxY);
                }
            }
        }
        
        if (maxY > 0 && maxY >= FRAME_BOTTOM - 10) {  //this -10 will be dependent on the speed (faster it goes, less responsive the getting y coord)
            wasAtBottom = true;  // ball in claw
            return false;
        } else if (wasAtBottom && maxY == 0) {  
          // Object was at bottom and is now gone
          Serial.println("Object exited from the bottom!");
          wasAtBottom = false;
      
          return true;
        }
    }
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% MOTOR FUNCTIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void motorForwards(int time = 15) {
    // move forward time ms (default 15 ms)
    int motorSpeed = 100; // 50% duty cycle (128 out of 255)
    digitalWrite(dirA, HIGH); // forwards direction
    digitalWrite(dirB, HIGH); // forwards direction
    analogWrite(pwmA, motorSpeed+27);
    analogWrite(pwmB, motorSpeed);

    delay(time); //! TODO - Issue where it can cause hangups in main thread --> use ticks to handle rather than delay

//    analogWrite(pwmA, 0);
//    analogWrite(pwmB, 0);
}

void motorReverse(int time = 15) {
    // move forward time ms (default 15 ms)
    int motorSpeed = 128; // 50% duty cycle (128 out of 255)
    digitalWrite(dirA, LOW); // forwards direction
    digitalWrite(dirB, LOW); // forwards direction
    analogWrite(pwmA, motorSpeed+40);
    analogWrite(pwmB, motorSpeed);

    delay(time); //! TODO - Issue where it can cause hangups in main thread --> use ticks to handle rather than delay

//    analogWrite(pwmA, 0);
//    analogWrite(pwmB, 0);
}

// !TODO - Test & debug motor turning
void motorRotate(int time) {
    // move forward time ms (default 15 ms)
    int motorSpeed = 90; // 50% duty cycle (128 out of 255)

    if (time > 0) {
        digitalWrite(dirA, HIGH); 
        digitalWrite(dirB, LOW); // right
    } else {
        digitalWrite(dirA, LOW); 
        digitalWrite(dirB, HIGH); // left
    }
        
    // overall should turn?
    analogWrite(pwmA, motorSpeed);
    analogWrite(pwmB, motorSpeed);

    delay(abs(time)); //! TODO - Issue where it can cause hangups in main thread --> use ticks to handle rather than delay


//    analogWrite(pwmA, 0);
//    analogWrite(pwmB, 0);   // causes jittery wheels
}

void motorStop() {
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);
}
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% END MOTOR FUNCTIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
    setPixyCam("UP");
}

void loop() {
//   if (getFrontDistance() <= 20) { // length of claws alrdy 12cm
//     stop/run away
//     motorRotate(500); //maybe change 1000 depending on how much it turns in given time
//     motorStop(); //stop rotating
//     motorForwards();
//   } else {
//     seekObject(green);
//   }
    motorForwards(4000);
    motorStop();
    delay(1000);
    
    // unsigned long currentTick = millis(); // get current time in milliseocnds
    // delay(15);  
    // if (currentTick - lastTick >= tickInterval){
    //     // tick elapsed, run some code...

    //     // check for obstacles
        // if (getFrontDistance() <= 20) { // length of claws alrdy 12cm
        //     // stop/run away
        //     motorRotate(500); //maybe change 1000 depending on how much it turns in given time
        //     motorStop(); //stop rotating
        //     motorForwards();
    //     } else {  
    //         seekObject(green);
    //         //motorForwards();
    //         //%%%%%%%%%%%%%function to find balls
    //         // basically centeringObject() but pick a single ball to target
    //         // change centerObject() to be in the for with a while instide the for. so when the for turns off, the while turns on
    //         // if (ballIntoClaws() == true) {
    //         //     int netPoints = getNetPoints();
    //         //     if (netPoints < 0) {
    //         //         //discard (to some other base)
    //         //         seekObject(rightBase); // find some random base lol - ie. colour signature anything other than base?
    //         //     } else if (netPoints == 0) {
    //         //         //dont care and discard    OR    continue grabbing balls
    //         //         seekObject(rightBase);
    //         //     } else if (netPoints > 0) {
    //         //         // go deposit    OR    continue grabbing balls
    //         //         seekObject(ourBase);
    //         //     }
            
    //         // }
    //     }
    // }
}
