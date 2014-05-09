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
int responseTime;

// Periodic update
const int periodicUpdateInterval = 1000;
int periodicUpdateTime = millis();

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
}





// Turning functions
void setTurnOutput(double output, boolean enabled) {
  digitalWrite(turnEnable, enabled);
  
  // Contro H-bridge to turn steering
  if (output >= 0) {
    analogWrite(turnRightPin, 0);
    analogWrite(turnLeftPin, output); 
  } else {
    analogWrite(turnLeftPin, 0);
    analogWrite(turnRightPin, -output);
  }
}

// Drive motor functions
void initDriveMotor() {
  delay(1000);
  driveMotor.writeMicroseconds(1500);
  delay(1000);
  // Send signal to start sending data
  Serial.print("*");
}

// take in number between -500 and 500
void driveMotorSpeed(double output, boolean enabled) {
  if (output > driveLimit)
    output = driveLimit;
  else if (output < -driveLimit)
    output = -driveLimit;
    
  if (enabled) 
    driveMotor.writeMicroseconds(1500 - output);
  else
    driveMotor.writeMicroseconds(1500);
} 

void setDriveLimit(double input) {
  if (input > 500)
    input = 500;
  else if (input < 0)
    input = 0;
  driveLimit = input;
}

void sendBatteryVoltage() {
  Serial.print('v');
  Serial.print((10.0f / 1024.0f) * analogRead(voltagePin));
  Serial.print(':');
}
