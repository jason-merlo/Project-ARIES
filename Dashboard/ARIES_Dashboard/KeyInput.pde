// KeyInput Variables
boolean keyHeld = false;

boolean keyRight = false;
boolean keyLeft = false;
boolean keyUp = false;
boolean keyDown = false;

void keyPressed() {

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
    keyUp = true;
  } else if (key == 's') {
    keyDown = true;
  }
  // Turn
  if (key == 'a') {
    keyRight = true;
  } else if (key == 'd') {
    keyLeft = true;
  }
}
void keyReleased() {
  keyHeld = false;
  
  // onHold keys
  // Drive
  if (key == 'w')
    keyUp = false;
  if (key == 's')
    keyDown = false;
  // Turn
  if (key == 'a')
    keyRight = false;
  if (key == 'd')
    keyLeft = false;
}

// Key Updater
void updateKeys() {
  // Drive
  if (keyUp == true) {
    speed = 0;
  } else if (keyDown == true) {
    speed = 0;
  } if (keyDown == false && keyUp == false) {
    speed = 0;
  }
  
  // Turn
  if (keyLeft == true) {
    turn = 400;
  } else if (keyRight == true) {
    turn = 600;
  } else {
    turn = 500;
  }
}
