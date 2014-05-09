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
