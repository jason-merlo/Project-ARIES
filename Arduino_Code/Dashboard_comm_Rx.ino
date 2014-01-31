//////////////////////////
//// Parse User Input ////
//////////////////////////

// valueId tags each transmitted parameter with an id so not all params have to sent each loop

int dataIndex;
boolean readUsrBool;
byte readUsrByte;
int readUsrInt;
float readUsrFloat;
double readUsrDouble;
String readUsrChar;
byte valueId = -1;
// each location represents an id and datatype associated //
byte dataType [] = {0,4,4,4,4,4};
// 0  enable/disable
// 1  kP
// 2  kI
// 3  kD
// 4  turn set point
// 5  speed
void SearialEvent() {
  
  Serial.println("RECIEVED:");
  if(Serial.available()) {
    
    // get new input
    readUsrByte = byte(Serial.read());

    
    if (valueId == -1) {

      // Take state of car //
      if (dataIndex = 0) {
        state = readUsrByte;
        dataIndex++;
        Serial.println("RECIEVED:");
        Serial.println(readUsrByte);
      }

      // EOL signified by newline //
      if ( char(readUsrByte) == '\n' ) {
        dataIndex = 0;
      } 

      if (dataIndex > 0) {
        valueId = readUsrByte;
      }

    } else {
      parseVals();
      storeVals();
      // Set valueId back to unknown state
      valueId = -1;
      dataIndex++;
    }
  }
}

// Parses data for value type
void parseVals() {
  switch(dataType[valueId]) {
      case 0:
        readUsrBool = boolean(Serial.read());
        break;

      case 1:
        readUsrByte = byte(Serial.read());
        break;

      case 2:
        readUsrInt = int(Serial.read());
        break;

      case 3:
        readUsrFloat = float(Serial.read());
        break;

      case 4:
        readUsrDouble = double(Serial.read());
        break;

      case 5:
        readUsrChar = String(char(Serial.read())); 
        break;
      }
}

// Store parsed values
void storeVals() {
  switch(valueId) {

  case 0:
    if (readUsrBool) {
      state = true;
    } 
    else {
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

  if (valueId >= 2) {
    turnPID.SetTunings(kP,kI,kD);
  }
}
