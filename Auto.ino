#include <Ultrasonic.h>
Ultrasonic ultrasonicF(23, 22, 7000UL);  
Ultrasonic ultrasonicB(21, 19, 7000UL);   

#define enB 33
#define enA 32
#define in4 16
#define in3 17
#define in2 5
#define in1 18

int motorSpeedA = 0;
int motorSpeedB = 0;

const int freq = 30000;
const int pwmChannel = 1;
const int pwmChannel2 = 2;
const int res = 8;

const int irFpin = 25;
const int irBpin = 26;
const int irLpin = 27;
const int irRpin = 13; //outputs PWM signal at boot
int blackF;
int blackR;
int blackL;
int blackB;


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Stop();
  Serial.begin(9600);
  
  ledcSetup(pwmChannel, freq, res);
  ledcAttachPin(enA, pwmChannel);
  ledcAttachPin(enB, pwmChannel2);

  pinMode(irFpin, INPUT);
  pinMode(irRpin, INPUT);
  pinMode(irLpin, INPUT);
  pinMode(irBpin, INPUT);

  //delay(5000);
}


void Edge(){
  
  if (digitalRead(irFpin) == 0){
   Serial.println("F");
   Backward();
   Serial.println("Backward");
   delay(1500);
   RotateR();
   Serial.println("RotateR");
   delay(1500);
  }
  if (digitalRead(irRpin) == 0){
    Serial.println("R");
    RotateL();
    Serial.println("RotateL");
    delay(1500);
    Forward();
    Serial.println("Forward");
    delay(1500);
  }
  if (digitalRead(irLpin) == 0){
    Serial.println("L");
    RotateR();
    Serial.println("RotateR");
    delay(1500);
    Forward();
    Serial.println("Forward");
    delay(1500);
  }
  if (digitalRead(irBpin) == 0){
    Serial.println("B");
    Forward();
    Serial.println("Forward");
    delay(1500); 
  }
  else {
    blackF = 1;
    blackR = 1;
    blackL = 1;
    blackB = 1;
  }
}

void Stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  motorSpeedA = 0;
  motorSpeedB = 0;
  Serial.println("Stopped"); 
}

void Forward() {
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   motorSpeedA = 255;
   motorSpeedB = 255;
   ledcWrite(pwmChannel, motorSpeedA); // Send PWM signal to motor A
   ledcWrite(pwmChannel2, motorSpeedB); // Send PWM signal to motor B
   //Serial.println("Forward"); 
}

void Backward() {
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   motorSpeedA = 255;
   motorSpeedB = 255;
   ledcWrite(pwmChannel, motorSpeedA); // Send PWM signal to motor A
   ledcWrite(pwmChannel2, motorSpeedB); // Send PWM signal to motor B
   //Serial.println("Backward"); 
}


void RotateL(){
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   motorSpeedA = 255;
   motorSpeedB = 255;
   ledcWrite(pwmChannel, motorSpeedA); // Send PWM signal to motor A
   ledcWrite(pwmChannel2, motorSpeedB); // Send PWM signal to motor B
   //Serial.println("RotateL"); 
}

void RotateR(){
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   motorSpeedA = 255;
   motorSpeedB = 255;
   ledcWrite(pwmChannel, motorSpeedA); // Send PWM signal to motor A
   ledcWrite(pwmChannel2, motorSpeedB); // Send PWM signal to motor B
   //Serial.println("RotateR"); 
}

void loop() { 
Edge();
  
    while (digitalRead(irFpin) == 1 && digitalRead(irRpin) == 1 && digitalRead(irLpin) == 1 && digitalRead(irBpin) == 1){
      int cmF = ultrasonicF.read();
      int cmB = ultrasonicB.read();
    
      Serial.print(" Sensor F: ");
      Serial.print(cmF); 
      Serial.print("cm");
      Serial.print(" Sensor B: ");
      Serial.print(cmB); 
      Serial.println("cm");
    
      if (cmB < 10){
        Backward();
      }    
//      else if(cmB < 25){
//        RotateR();
//        delay(1000);
//      }
      else if (cmF < 25){
        Forward();  
      }
      else if (cmF > 25 && cmB > 25 ){
        RotateL();
      }
   }
 }
