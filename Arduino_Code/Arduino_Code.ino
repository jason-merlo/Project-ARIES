#include <PID_v1.h>
#include <Servo.h> 

Servo driveMotor;

// Control Constants
const int responseTimeout = 500; // ms

// Actuators
const int turnEnable = 4;
const int turnLeftPin = 5;
const int turnRightPin = 6;
const int drivePin = 3;

// Sensors
const int turnPotPin = 5;
const int voltagePin = 0;

// PID
double turnSetPoint, turnInput, turnOutput;
double kP = 5, kI = 3, kD = .1;
PID turnPID(&turnInput, &turnOutput, &turnSetPoint, kP, kI, kD, DIRECT);

// Control variables
boolean state; // (enabled/disabled)
double driveSpeed;
double driveLimit;
long long lastResponseTime;
long long responseTime;

// Periodic update
const int periodicUpdateInterval = 1000;
long long periodicUpdateTime = millis();

void setup()
{
  // Open serial line @57600 baud
  Serial.begin(57600);
  
  // Set output limits
  turnPID.SetOutputLimits(-255,255);
  setDriveLimit(200);
  
  // Enable output pins
  pinMode(turnLeftPin, OUTPUT);
  pinMode(turnRightPin, OUTPUT);
  pinMode(turnEnable, OUTPUT);
  
  driveMotor.attach(drivePin);
  
  // Initialize actuators
  initDriveMotor();
  
  // Initialize sensors
  turnInput = analogRead(turnPotPin);
  turnSetPoint = 500;

  turnPID.SetMode(AUTOMATIC);
}

void loop()
{
  //getSerial();
  delay(1);
  
  // Get response time
  responseTime = millis() - lastResponseTime;
  
  // Check for timeout
  if (responseTime > responseTimeout) {
    state = false;
    driveSpeed = 0;
  }
  
  updateActuators();
}

void sendBatteryVoltage() {
  Serial.print('[');
  Serial.print((10.0f / 1024.0f) * analogRead(voltagePin));
  Serial.print(']');
}
