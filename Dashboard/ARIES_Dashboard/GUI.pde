// CAMERAS
IPCapture camA;
IPCapture camB;

// GUI ELEMENTS
PImage ariesLogo;
Button stateToggle;
BatteryIndicator batteryIndicator;
SignalIndicator signalIndicator;
TextBox PID_kP;
TextBox PID_kI;
TextBox PID_kD;
Button setPid;

// FONTS
PFont avNextBold;
PFont avNext;

/* GUI OPERATIONS */
void initGui() {
  /* FONTS */
  avNextBold = createFont("AvenirNext-Bold", 18, true);
  avNext = createFont("AvenirNext-Regular", 18, true);

  /* GUI ELEMENTS */
  ariesLogo = loadImage("ARIESlogolight.png");

  batteryIndicator = new BatteryIndicator(width - 50, 20, 50);
  signalIndicator = new SignalIndicator(width - 175, 20, 50, 4);

  stateToggle = new Button(imgBorder, 345, 320, 70, "Disabled", "Enabled", true);

  PID_kP = new TextBox(730, 345, true, "kP: ");
  PID_kI = new TextBox(730, 370, true, "kI: ");
  PID_kD = new TextBox(730, 395, true, "kD: ");

  setPid = new Button (340, 345, 320, 70, "Set PID", false);
}

void updateGui() {
  // Draw Logo
  background(#eeeeee);
  image(ariesLogo, 10, 10, 60, 60);
  textFont(avNext);
  fill(50);
  text("PROJECT", 70, 50);
  textFont(avNextBold);
  text("ARIES", 70 + textWidth("PROJECT"), 50);

  stateToggle.update();
  if (stateToggle.buttonPressed()) stateToggle.toggle();

  batteryIndicator.update(truncate(avgBattVoltage, 2));
  signalIndicator.update(truncate(avgRespTime, 2));

  kP = PID_kP.update(Float.toString(kP));
  kI = PID_kI.update(Float.toString(kI));
  kD = PID_kD.update(Float.toString(kD));

  setPid.update();

  drawCameras();
}

/* DRAW CAMERAS */
void drawCameras() {

  // Draw MJPEG streams
  if (camA.isAvailable()) camA.read();

  fill(0);
  rect(imgBorder, imgBorder + 80, 320 * imageScale, 240 * imageScale);
  image(camA, imgBorder, imgBorder + 80, camA.width * imageScale, camA.height * imageScale); 

  if (camB.isAvailable()) camB.read();

  fill(0);
  rect( 320 * imageScale + imgBorder * 2, imgBorder + 80, 320 * imageScale, 240 * imageScale);
  image(camB, camA.width * imageScale + imgBorder * 2, imgBorder + 80, camB.width * imageScale, camB.height * imageScale);

  
    fill(50);
    rect((320 * imageScale) + (320 * imageScale) + imgBorder * 3, imgBorder + 80, 320, 240);
    
  if (camA.width > 0 && camB.width > 0) {
    updateStereo();
    // Draw disparity map
    image(depth1, camA.width * 2 * imageScale + imgBorder * 3, imgBorder + 80, camB.width * imageScale, camB.height * imageScale);
  }
}

