// KeyInput Variables
boolean keyHeld = false;
boolean keyDown = false;

boolean rightKey = false;
boolean leftKey = false;
boolean upKey = false;
boolean downKey = false;

void keyPressed() {

  keyDown = true;
  
  // onRelease keys
  if (!keyHeld) {
    keyHeld = true;
    if (key == ' ') {
      //eStopButton.lock(true);
      //////////////////////////////
      //TODO: WRITE CODE TO E-STOP//
      //////////////////////////////
    }
    if (key == '\n') {
      //stateButton.toggle();
      //////////////////////////////
      //TODO: WRITE CODE TO E-STOP//
      //////////////////////////////
    }
  }

  // onHold keys
  // Drive
  if (key == 'w') {
    upKey = true;
  } else if (key == 's') {
    downKey = true;
  }
  // Turn
  if (key == 'a') {
    rightKey = true;
  } else if (key == 'd') {
    leftKey = true;
  }
}
void keyReleased() {
  keyHeld = false;
  keyDown = false;
  
  // onHold keys
  // Drive
  if (key == 'w')
    upKey = false;
  if (key == 's')
    downKey = false;
  // Turn
  if (key == 'a')
    rightKey = false;
  if (key == 'd')
    leftKey = false;
}

// Key Updater
void updateKeys() {
  // Drive
  if (upKey == true) {
    speed = 150;
  } else if (downKey == true) {
    speed = -75;
  } if (downKey == false && upKey == false) {
    speed = 0;
  }
  
  // Turn
  if (leftKey == true) {
    turn = 700;
  } else if (rightKey == true) {
    turn = 375;
  } else {
    turn = 525;
  }
}
