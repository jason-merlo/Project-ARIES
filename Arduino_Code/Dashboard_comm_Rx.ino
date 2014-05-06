// TODO: convert strings to numerical values

//////////////////////////
//// Parse User Input ////
//////////////////////////
// example data:
// [valueID],[value]:...[valueID],[value]:

// valueId tags each transmitted parameter with an id so not all params have to sent each loop

// Parsing variables
boolean readUsrBool;
byte readUsrByte;
int readUsrInt;
float readUsrFloat;
double readUsrDouble;
char readUsrChar;
String readUsrStr;

// Id of data being sent
byte valueId = -1;

// Buffer string
String dataBuffer = "";
// each location represents an id and datatype associated //
byte dataType [] = {
  0,4,4,4,4,4};
// 0  enable/disable
// 1  turn set point
// 2  drive speed
// 3  kP
// 4  kI
// 5  kD
//void serialEvent() {
void getSerial() {
  //responseTime = 0;
  //lastResponseTime = millis();
  
  if(Serial.available() > 0) {
    // get new input
    readUsrChar = char(Serial.read());

    parseVals();
    delay(1);
  }
}

// Parses data for value type
void parseVals() {
  
  // ValueID's come before ','
  if ( readUsrChar == ',') {
    char floatbuf[32];
    memset(floatbuf, 0, 32);
    dataBuffer.toCharArray(floatbuf, sizeof(floatbuf));
    /*Serial.print("String Array: ");
    Serial.println(floatbuf);*/
    
    valueId = atoi(floatbuf);
    
    /*Serial.print("ValueID = ");
    Serial.println(valueId);*/
    
    // Reset Buffer
    dataBuffer = "";
    
  // Values come before ':' 
  } else if ( readUsrChar == ':') {
    char floatbuf[32];
    memset(floatbuf, 0, 32);
    dataBuffer.toCharArray(floatbuf, sizeof(floatbuf));
    /*Serial.print("String Array: ");
    Serial.println(floatbuf);*/
    
    switch(dataType[valueId]) {
    case 0:
      readUsrBool = atoi(floatbuf) >= 1 ? true : false;
      /*Serial.print("readUsrBool = ");
      Serial.println(readUsrBool);*/
      break;

    case 1:
      readUsrByte = byte(atoi(floatbuf));
      /*Serial.print("readUsrBool = ");
      Serial.println(readUsrBool);*/
      break;

    case 2:
      readUsrInt = dataBuffer.toInt();
      /*Serial.print("readUsrInt = ");
      Serial.println(readUsrInt);*/
      break;

    case 3:
      readUsrFloat = atof(floatbuf);
      /*Serial.print("readUsrFloat = ");
      Serial.println(readUsrFloat);*/
      break;

    case 4:
      readUsrDouble = double(atof(floatbuf));
      /*Serial.print("readUsrDouble = ");
      Serial.println(readUsrDouble);*/
      break;

    case 5:
      readUsrStr = dataBuffer;
      /*Serial.print("readUsrStr = ");
      Serial.println(readUsrStr);*/
      break;
    }
    // reset buffer
    dataBuffer = "";
    
    storeVals();
  } else if (readUsrChar != '\n' && readUsrChar != '\r' && byte(readUsrChar) > 5) {
    // Throw out newlines from serial
    // If no parsing information found, continue to search
    dataBuffer += readUsrChar;
  }
}

// Store parsed values
void storeVals() {
  switch(valueId) {
    case 0:
      if (readUsrBool) {
        state = true;
      } else {
        state = false;
      }
      break;
    
    case 1:
      turnSetPoint = readUsrDouble;
      break;
    
    case 2:
      driveSpeed = readUsrDouble;
      break;
    
    case 3:
      kP = readUsrDouble;
      break;
    
    case 4:
      kI = readUsrDouble;
      break;
    
    case 5:
      kD = readUsrDouble;
      break;
    }
    
    Serial.print('*');
    
    if (valueId >= 3) {
      turnPID.SetTunings(kP,kI,kD);
      //Serial.println("updated tunings");
    }
    /*Serial.print(state);
    Serial.print(",");
    Serial.print(turnSetPoint);
    Serial.print(",");
    Serial.print(driveSpeed);
    Serial.print(",");
    Serial.print(kP);
    Serial.print(",");
    Serial.print(kI);
    Serial.print(",");
    Serial.println(kD);*/
}

