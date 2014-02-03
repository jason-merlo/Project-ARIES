// TODO: convert strings to numerical values

//////////////////////////
//// Parse User Input ////
//////////////////////////
// example data:
// [valueID],[value];...[valueID],[value];\n

// valueId tags each transmitted parameter with an id so not all params have to sent each loop

int dataIndex;
boolean readUsrBool;
byte readUsrByte;
int readUsrInt;
float readUsrFloat;
double readUsrDouble;
char readUsrChar;
byte valueId = -1;

// Buffer string
String dataBuffer;
// each location represents an id and datatype associated //
byte dataType [] = {
  0,4,4,4,4,4};
// 0  enable/disable
// 1  kP
// 2  kI
// 3  kD
// 4  turn set point
// 5  speed
void serialEvent() {
  while(Serial.available()) {
    // get new input
    readUsrChar = char(Serial.read());

    // Unknown state of information
    if (valueId == -1 && dataBuffer == "") {      
      // EOL signified by newline //
      if ( readUsrChar == '\n' ) {
        dataIndex = 0;
      } else {
        valueId = int(readUsrChar);
      }
    } else {
      parseVals();
      storeVals();
      // Set valueId back to unknown state
      dataIndex++;
    }
  }
}

// Parses data for value type
void parseVals() {
  Serial.print(readUsrChar);
  // Split values with commas
  if ( readUsrChar == ',') {
    switch(dataType[valueId]) {
    case 0:
      readUsrBool = atoi(dataBuffer) > 1 ? true: false;
      break;

    case 1:
      readUsrByte = byte(dataBuffer);
      break;

    case 2:
      readUsrInt = int(dataBuffer);
      break;

    case 3:
      readUsrFloat = float(dataBuffer);
      break;

    case 4:
      readUsrDouble = double(dataBuffer);
      break;

    case 5:
      readUsrChar = String(char(dataBuffer)); 
      break;
    }
    // reset valueID and buffer
    dataBuffer = "";
    valueId = -1;
  } else {
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
    Serial.println("case1 triggered");
    Serial.println(readUsrDouble);
    Serial.println(turnSetPoint);
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

  if (valueId >= 2) {
    turnPID.SetTunings(kP,kI,kD);
  }
}

