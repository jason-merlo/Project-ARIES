/* GUI OPERATIONS */
void initGui() {
  /* FONTS */
  avNextBold = createFont("AvenirNext-Bold", 18, true);
  avNext = createFont("AvenirNext-Regular", 18, true);
  
  /* GUI ELEMENTS */
  ariesLogo = loadImage("ARIESlogolight.png");
  stateToggle = new Button(imgBorder, 345, 320, 70, "Disabled", "Enabled", true);
  batteryIndicator = new BatteryIndicator(width - 50, 20, 50);
  signalIndicator = new SignalIndicator(width - 175, 20, 50, 4);
}

void updateGui() {
  // Draw Logo
  background(#eeeeee);
  image(ariesLogo, 10, 10, 60, 60);
  textFont(avNext);
  text("PROJECT", 70, 50);
  textFont(avNextBold);
  text("ARIES", 70 + textWidth("PROJECT"), 50);
  stateToggle.update();
  if (stateToggle.buttonPressed()) stateToggle.toggle();
  batteryIndicator.update(battVoltage);
  signalIndicator.update(0);
  
  drawCameras();
}

/* DRAW CAMERAS */
void drawCameras() {
  /*noStroke();
  // Draw MJPEG streams
  if (camA.isAvailable()) {
    camA.read();
  }
  fill(0);
  rect( imgBorder, imgBorder + 80, 320 * imageScale, 240 * imageScale);

  if (camB.isAvailable()) {
    camB.read();
  }
  rect( 320 * imageScale + imgBorder * 2, imgBorder + 80, 320 * imageScale, 240 * imageScale);

  // Draw disparity map
  if (true) {
    fill(50);
    rect((320 * imageScale) + (320 * imageScale) + imgBorder * 3, imgBorder + 80, 320, 240);
  }*/
  
  // Draw MJPEG streams
  if (camA.isAvailable()) {
    camA.read();
  }
  image(camA, imgBorder, imgBorder + 80, camA.width * imageScale, camA.height * imageScale);

  if (camB.isAvailable()) {
    camB.read();
  }
  image(camB, camA.width * imageScale + imgBorder * 2, imgBorder + 80, camB.width * imageScale, camB.height * imageScale);

  // Draw disparity map
  if (true) {
    fill(50);
    rect((camA.width * imageScale) + (camB.width * imageScale) + imgBorder * 3, imgBorder + 80, camA.width, camA.height);
  }
}
