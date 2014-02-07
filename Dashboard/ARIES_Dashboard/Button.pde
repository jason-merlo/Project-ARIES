class Button {

  int x, y, buttonWidth, buttonHeight;

  color pressedColor = color(100);
  color releasedColor = color(160);
  color hoverColor = color(140);
  color lockedColor = color(80);
  color warningPressedColor = color(150, 45, 45);
  color warningReleasedColor = color(215, 60, 60);
  color warningHoverColor = color(200, 45, 45);
  color warningLockedColor = color(80, 45, 45);
  color pressedTextColor = color(155);
  color releasedTextColor = color(255);
  color hoverTextColor = color(205);
  color lockedTextColor = color(135);

  color textColor;
  color buttonColor;

  boolean enabled = false;
  boolean locked = false;
  boolean pressed = false;
  boolean hover = false;
  boolean hasBeenPressed = false;
  boolean pressOriginInside = false;
  boolean lastPressed = false;
  boolean lastMouse = false;

  String buttonTextDefault;
  String buttonTextEnabled;
  String lockedText;

  boolean warningButton;

  //textDefault will deisplay when the button is in the false state
  
  Button (int tx, int ty, int twidth, int theight, String ttextDefault, boolean twarningButton) {
    x = tx;
    y = ty;
    buttonWidth = twidth;
    buttonHeight = theight;
    warningButton = twarningButton;
    buttonTextDefault = ttextDefault;
    buttonTextEnabled = ttextDefault;
  }
  
  Button (int tx, int ty, int twidth, int theight, String ttextDefault, String ttextEnabled, boolean twarningButton) {
    x = tx;
    y = ty;
    buttonWidth = twidth;
    buttonHeight = theight;
    warningButton = twarningButton;
    buttonTextDefault = ttextDefault;
    buttonTextEnabled = ttextEnabled;
  }

  Button (int tx, int ty, int twidth, int theight, String ttextDefault, boolean twarningButton, String tlockText) {
    x = tx;
    y = ty;
    buttonWidth = twidth;
    buttonHeight = theight;
    warningButton = twarningButton;
    buttonTextDefault = ttextDefault;
    lockedText = tlockText;
  }

  void drawButton() {
    if (warningButton) {
      if (locked) {
        buttonColor = warningLockedColor;
        textColor = lockedTextColor;
      } else if (hover) {
        buttonColor = warningHoverColor;
        textColor = hoverTextColor;
      } else if (pressed) {
        buttonColor = warningPressedColor;
        textColor = pressedTextColor;
      } else {
        buttonColor = warningReleasedColor;
        textColor = releasedTextColor;
      }
    } else { 
      if (locked) {
        buttonColor = lockedColor;
        textColor = lockedTextColor;
      } else if (hover) {
        buttonColor = hoverColor;
        textColor = hoverTextColor;
      } else if (pressed) {
        buttonColor = pressedColor;
        textColor = pressedTextColor;
      } else {
        buttonColor = releasedColor;
        textColor = releasedTextColor;
      }
    }

    noStroke();
    fill(buttonColor);
    rect(x, y, buttonWidth, buttonHeight);
    fill(red(buttonColor) - 40, blue(buttonColor) - 40, green(buttonColor) - 40);
    triangle(x, y + buttonHeight, x + 10, y + buttonHeight, x + 10, y + buttonHeight + 5);
    rect(x + 10, y + buttonHeight, buttonWidth - 20, 5);
    triangle(x + buttonWidth - 10, y + buttonHeight, x + buttonWidth, y + buttonHeight, x + buttonWidth - 10, y + buttonHeight + 5);

    fill(textColor);
    if (locked) {
      text(lockedText, x + (buttonWidth / 2) - (textWidth(lockedText) / 2), y + (buttonHeight / 2) + 7);
    } 
    else if (enabled){
      text(buttonTextEnabled, x + (buttonWidth / 2) - (textWidth(buttonTextEnabled) / 2), y + (buttonHeight / 2) + 7);
    } else {
      text(buttonTextDefault, x + (buttonWidth / 2) - (textWidth(buttonTextDefault) / 2), y + (buttonHeight / 2) + 7);
    }
  }

  void update() {
    //mousePressed in button
    if (mouseX > x && mouseX < (x + buttonWidth) && mouseY > y && mouseY < (y + buttonHeight) && !locked) {
      if (mousePressed) {
        pressed = true;
        hover = false;
      } 
      else {
        hover = true;
        pressed = false;
      }
    } 
    else {
      hover = false;
      pressed = false;
    }
    
    //mouseDown in button
    if (mouseX > x && mouseX < (x + buttonWidth) && mouseY > y && mouseY < (y + buttonHeight) && !locked && lastPressed == false && pressed == true && lastMouse == false && mousePressed) {
      pressOriginInside = true;
    } else if (!(mouseX > x && mouseX < (x + buttonWidth) && mouseY > y && mouseY < (y + buttonHeight)) && !locked) {
      pressOriginInside = false;
    }
    
    //mouseReleased in button
    if (mouseX > x && mouseX < (x + buttonWidth) && mouseY > y && mouseY < (y + buttonHeight) && !locked && lastPressed == true && pressed == false && pressOriginInside) {
      hasBeenPressed = true;
    } else {
      hasBeenPressed = false;
    }
    
    drawButton();
    lastPressed = pressed;
    lastMouse = mousePressed;
  }

  boolean buttonPressed() {
    if (hasBeenPressed) {
      return true;
    } else {
      return false;
    }
  }

  boolean buttonLocked() {
    if (locked) {
      return true;
    } else {
      return false;
    }
  }
  void lock(boolean lockButton) {
    locked = lockButton;
    pressed = false;
  }
  
  void toggle() {
    if (enabled) {
      enabled = false;
    } else {
      enabled = true;
    }
  }
  
  boolean getState() {
    return enabled;
  }
}

