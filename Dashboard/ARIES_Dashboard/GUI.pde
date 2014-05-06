/* GUI OPERATIONS */
void initGui() {
  /* GUI ELEMENTS */
  stateToggle = new Button(100, 400, 200, 50, "Disabled", "Enabled", true);
  batteryIndicator = new BatteryIndicator(width - 50, 50, 50);
}

void updateGui() {
  stateToggle.update();
  if (stateToggle.buttonPressed()) stateToggle.toggle();
  batteryIndicator.update(battVoltage);
  
  drawCameras();
}

/* DRAW CAMERAS */
void drawCameras() {
  // Draw background box
  noStroke();
  fill(100);
  rect(0, 0, camA.width + camB.width + camB.width + imgBorder * 4, camA.height + imgBorder * 2);
  // Draw MJPEG streams
  if (camA.isAvailable()) {
    camA.read();
  }
  image(camA, imgBorder, imgBorder, camA.width * imageScale, camA.height * imageScale);

  if (camB.isAvailable()) {
    camB.read();
  }
  image(camB, camA.width * imageScale + imgBorder * 2, imgBorder, camB.width * imageScale, camB.height * imageScale);

  // Draw disparity map
  if (true) {
    fill(255);
    rect((camA.width * imageScale) + (camB.width * imageScale) + imgBorder * 3, imgBorder, camA.width, camA.height);
  }
}
