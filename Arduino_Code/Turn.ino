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
