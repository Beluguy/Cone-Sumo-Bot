#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
 
#define enB 33
#define enA 32
#define in4 18
#define in3 5
#define in2 17
#define in1 16

int motorSpeedA = 0;
int motorSpeedB = 0;

const int freq = 30000;
const int pwmChannel = 1;
const int pwmChannel2 = 2;
const int res = 8;


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  Serial.begin(9600);
  Dabble.begin("CYBERTRUCK");       //set bluetooth name of your device
  
  ledcSetup(pwmChannel, freq, res);
  ledcAttachPin(enA, pwmChannel);
  ledcAttachPin(enB, pwmChannel2);
}

void loop() {
  Dabble.processInput();
  float xAxis = GamePad.getXaxisData();
  Serial.print("xAxis: ");
  Serial.print(xAxis);
  float yAxis = GamePad.getYaxisData();
  Serial.print("  yAxis: ");
  Serial.print(yAxis);

  if (yAxis < -0.5) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    motorSpeedA = map(yAxis, 0, -6, 150, 255);
    motorSpeedB = map(yAxis, 0, -6, 150, 255);
  }
  
  else if (yAxis > 0.5) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    motorSpeedA = map(yAxis, 0, 7, 150, 255);
    motorSpeedB = map(yAxis, 0, 7, 150, 255);
  }
  
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

   //X-axis used for left and right control
  if (xAxis < -1.5) {
    int xMapped = map(xAxis, 0, -7, 150, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedB = motorSpeedB - xMapped;
    motorSpeedA = motorSpeedA + xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedB < 150) {
      motorSpeedB = 150;}
    if (motorSpeedB > 255) {
      motorSpeedB = 255;}
    if (motorSpeedA < 150) {
      motorSpeedA = 150;}
    if (motorSpeedA > 255) {
      motorSpeedA = 255;}
  }
  
  if (xAxis > 1.5) {
    int xMapped = map(xAxis, 0, 6, 150, 255);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedB = motorSpeedB + xMapped;
    motorSpeedA = motorSpeedA - xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedB < 150) {
      motorSpeedB = 150;}
    if (motorSpeedB > 255) {
      motorSpeedB = 255;}
    if (motorSpeedA < 150) {
      motorSpeedA = 150;}
    if (motorSpeedA > 255) {
      motorSpeedA = 255;}
  }
  Serial.print("  motorSpeedA = "); 
  Serial.print(motorSpeedA);
  Serial.print("  motorSpeedB = "); 
  Serial.println(motorSpeedB);
  ledcWrite(pwmChannel, motorSpeedA); // Send PWM signal to motor A
  ledcWrite(pwmChannel2, motorSpeedB); // Send PWM signal to motor B
}
