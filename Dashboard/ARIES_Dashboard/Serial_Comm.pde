
/* SERIAL COMMUNICATIONS */
boolean voltageRead = false;
char usrChar;
String voltageStr = "";

void getData() {
  if ( arduino.available() > 0 ) {
    usrChar = arduino.readChar();
    
    if (usrChar == '*') {
      readyToSend = true;
      connectTime = millis();
      println("MESSAGE RECIEVED --------------------------------------");
    } else {
      //readyToSend = false;
    }
    
    if (usrChar == ']' && voltageRead) {
      voltageRead = false;
      try {
         battVoltage = Float.parseFloat(voltageStr);
      } catch (NumberFormatException e) {
        //e.printStackTrace();
        println("MALFORMED PACKET");
      }
      voltageStr = "";
    }
    
    if (voltageRead && usrChar != '\n' && usrChar != '*') {
      voltageStr += usrChar;
    }
    
    if (usrChar == '[')
      voltageRead = true;
  }
}

void sendData() {
  // 0  enable/disable
  // 1  turn set point
  // 2  drive speed
  // 3  kP
  // 4  kI
  // 5  kD

  if (stateToggle.getState()) {
    writeEnabled(true);
    writeTurn(turn);
    writeSpeed(speed);
  } else {
    writeEnabled(false);
  }
  
  readyToSend = false;
  lastSendTime = millis();
  connectTime = millis();
}

void writeEnabled (boolean isEnabled) {
  if (isEnabled)
    arduino.write("00,000001:");
  else
    arduino.write("00,000000:");
}

void writeTurn (float turnAng) {
  arduino.write("01," + nf(turnAng, 3, 2) + ":");
}

void writeSpeed (float driveSpeed) {
  if (driveSpeed < 0) {
    arduino.write("02," + nf(driveSpeed, 3, 1) + ":");
  } else {
    arduino.write("02," + nf(driveSpeed, 3, 2) + ":");
  }
}
