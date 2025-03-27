
const int dirA = 4;   // Direction control on digital pin 4
const int pwmA = 5;   // PWM signal on digital pin 5 <-- has pwm

const int dirB = 2;   // Direction control on digital pin 2
const int pwmB = 3;   // PWM signal on digital pin 3 <-- has pwm

void setup() {
    pinMode(dirA, OUTPUT);
    pinMode(pwmA, OUTPUT);
    pinMode(dirB, OUTPUT);
    pinMode(pwmB, OUTPUT);
    Serial.begin(9600); // Initialize serial communication
}

void loop() {
    Serial.println("Motors moving forward");
    digitalWrite(dirA, HIGH); // Set Motor A direction forward
    digitalWrite(dirB, HIGH); // Set Motor B direction forward
    analogWrite(pwmA, 128);   // 50% duty cycle for Motor A
    analogWrite(pwmB, 128);   // 50% duty cycle for Motor B
    delay(1000);
}
