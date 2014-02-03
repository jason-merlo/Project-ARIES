#include <PID_v1.h>

// Actuators
const int turnEnable = 4;
const int turnLeftPin = 5;
const int turnRightPin = 6;
const int drivePin = 11;

// Sensors
const int turnPotPin = 5;

// PID
double turnSetPoint, turnInput, turnOutput;
double kP = 5, kI = 3, kD = .1;
PID turnPID(&turnInput, &turnOutput, &turnSetPoint, kP, kI, kD, DIRECT);

// Control variables
boolean state; // (enabled/disabled)
double driveSpeed;

void setup()
{
  Serial.begin(9600);
  
  turnPID.SetOutputLimits(-255,255);
  
  pinMode(turnLeftPin, OUTPUT);
  pinMode(turnRightPin, OUTPUT);
  pinMode(drivePin, OUTPUT);
  
  pinMode(turnEnable, OUTPUT);
  
  turnInput = analogRead(turnPotPin);
  turnSetPoint = 500;

  turnPID.SetMode(AUTOMATIC);
}

void loop()
{
  digitalWrite(turnEnable, state);
  
  turnInput = analogRead(turnPotPin);
  turnPID.Compute();
  
  // Contro H-bridge to turn steering
  if (turnOutput >= 0) {
    analogWrite(turnRightPin, 0);
    analogWrite(turnLeftPin, turnOutput); 
  } else {
    analogWrite(turnLeftPin, 0);
    analogWrite(turnRightPin, -turnOutput);
  }
  
  //Serial.print(turnInput);
  //Serial.print(",\t");
  //Serial.println(turnOutput);
}


