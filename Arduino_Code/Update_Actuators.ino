void updateActuators() {
    // Read sensors
  turnInput = analogRead(turnPotPin);

  // Compute output
  turnPID.Compute();

  // Control actuators
  setTurnOutput(turnOutput, state);
  driveMotorSpeed(driveSpeed, state);

  // Get response time
  responseTime = millis() - lastResponseTime;

  /*if (millis() - periodicUpdateTime >= periodicUpdateInterval) {
    // Send Battery Voltage
    sendBatteryVoltage();

    periodicUpdateTime = millis();
  }*/
}
