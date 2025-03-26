const int dirA = 11;   // Direction control on digital pin 11
const int pwmA = 10;   // PWM signal on digital pin 10

void setup() {
    pinMode(dirA, OUTPUT);
    pinMode(pwmA, OUTPUT);
    Serial.begin(9600); // Initialize serial communication
}

void loop() {
    Serial.println("Motor running forward");
    digitalWrite(dirA, HIGH); // Set direction
    analogWrite(pwmA, 128);   // 50% duty cycle (128 out of 255)
    delay(1000);

    Serial.println("Motor running backward");
    digitalWrite(dirA, LOW);  // Change direction
    delay(1000);

    Serial.println("Motor stopped");
    analogWrite(pwmA, 0);     // Turn motor off
}
