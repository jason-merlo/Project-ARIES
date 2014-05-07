class BatteryIndicator {
  
  //Position variables
  float x, y, indicatorWidth, indicatorHeight;
  float ratio = 1.5;
  
  //Data variables
  float batteryVoltage;
  float batteryPercent;
  
  //Color variables
  color batteryColor = color(160);
  color backgroundColor = color(80);
  color batteryAlertColor = color(215, 60, 60);
  
  //Threshold variables
  float alertThreshold = .2;
  
  //State variables
  boolean enabled = false;
  boolean locked = false;
  boolean pressed = false;
  boolean hover = false;
  boolean hasBeenPressed = false;
  boolean pressOriginInside = false;
  boolean lastPressed = false;
  boolean lastMouse = false;
  
  BatteryIndicator(int tx, int ty, int size) {
    x = tx;
    y = ty;
    indicatorWidth = size/ratio;
    indicatorHeight = size - (ratio*2); // compensate for battery tip
  }
  
  void displayBattery() {
    
    //Background Rectangle
    noStroke();
    fill(backgroundColor);
    rect(x + (indicatorWidth)/2 - indicatorHeight/8, y - ratio*2, indicatorHeight/4, ratio*2);
    rect(x, y, indicatorWidth, indicatorHeight);
    
    if(batteryPercent != 0) {
      
      if(batteryPercent <= alertThreshold) {
        fill(batteryAlertColor);
      } else {
        fill(batteryColor);
      }
      
      //Draw Battery Box
      rect(x, y + (indicatorHeight * (1 - batteryPercent)), indicatorWidth, indicatorHeight * batteryPercent);
      
      //Write battery percentage
      textFont(avNextBold);
      
      checkClick();
      
      if (buttonPressed()) {
        toggle();
      }
      
      if (enabled) {
        text(int(batteryPercent * 100) + "%", x - textWidth(int(batteryPercent * 100) + "%") - indicatorHeight/10 , y + indicatorHeight);
      } else {
        text(batteryVoltage + "V", x - textWidth(batteryVoltage + "V") - indicatorHeight/10 , y + indicatorHeight);
      }
    }
  }
  
  void checkClick() {
   //mousePressed in button
    if (mouseX > x && mouseX < (x + indicatorWidth) && mouseY > y && mouseY < (y + indicatorHeight) && !locked) {
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
    if (mouseX > x && mouseX < (x + indicatorWidth) && mouseY > y && mouseY < (y + indicatorHeight) && !locked && lastPressed == false && pressed == true && lastMouse == false && mousePressed) {
      pressOriginInside = true;
    } else if (!(mouseX > x && mouseX < (x + indicatorWidth) && mouseY > y && mouseY < (y + indicatorHeight)) && !locked && lastPressed == false && pressed == true && lastMouse == false && mousePressed) {
      pressOriginInside = false;
    }
    
    //mouseReleased in button
    if (mouseX > x && mouseX < (x + indicatorWidth) && mouseY > y && mouseY < (y + indicatorHeight) && !locked && lastPressed == true && pressed == false && pressOriginInside) {
      hasBeenPressed = true;
    } else {
      hasBeenPressed = false;
    }
    
    lastPressed = pressed;
    lastMouse = mousePressed;
  }

  boolean buttonPressed() {
    if (hasBeenPressed) {
      //hasBeenPressed = false;
      return true;
    } else {
      //hasBeenPressed = false;
      return false;
    }
  }
  
  
  void toggle() {
    if (enabled) {
      enabled  = false;
    } else {
      enabled = true;
    }
  }
  
  void update(float tBattVoltage) {
    batteryVoltage = tBattVoltage;
    batteryPercent = (tBattVoltage != 0) ? (float) (batteryVoltage - 7.4f) : 0.0f;
    displayBattery();
  }
}
