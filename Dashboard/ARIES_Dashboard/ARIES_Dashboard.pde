/////////////
// IMPORTS //
/////////////
import ipcapture.*;       // MJPEG lib
import processing.serial.*;  // Serial lib

/////////////
// OBJECTS //
/////////////
// SERIAL DEVICES
Serial arduino;

// CAMERAS
IPCapture camA;
IPCapture camB;

// GUI ELEMENTS
Button stateToggle;

///////////////
// CONSTANTS //
///////////////
// Ports
final String ip = "192.168.1.131";
final int camPortA = 8090;
final int camPortB = 8091;
final String camUrl = "/?action=stream.mjpg";
final String serialPort = "/dev/cu.usbserial";  // All or part of the serial port name

// GUI
final byte imageScale = 1;
final byte imgBorder = 10; // 10 pixels

// Connection
final int sendTimeout = 200; // milliseconds

///////////////
// VARIABLES //
///////////////
// Control
float speed;  // (-100, 100)
float turn;   // (-100, 100)

// PID
float kP = 5;
float kI = 3;
float kD = .1;

// Serial
boolean readyToSend;
int connectTime;
int lastSendTime = -1;


/* INITIALIZATION FUNCTION */
void setup() 
{
  /* WINDOW SETUP */
  size(1000, 650);
  background(49);
  frameRate(30);

  /* CAMERA COMMUNICATION */
  // Connect to cameras
  camA = new IPCapture(this, "http://" + ip + ":" + camPortA + camUrl, "", "");
  camB = new IPCapture(this, "http://" + ip + ":" + camPortB + camUrl, "", "");
  // Start camera streams
  camA.start();
  camB.start();

  /* GUI ELEMENTS */
  stateToggle = new Button(100, 400, 200, 50, "Disabled", "Enabled", true);
  
  /* ARDUINO SERIAL COMMUNICATION */
  for (int i = 0; i < Serial.list().length; i++) {
    if (Serial.list()[i].indexOf("tty.usbserial") != -1) {
      arduino = new Serial(this, Serial.list()[i], 57600);
      println("Opened serial port: " + Serial.list()[i]);
    }
  }
  delay(10);
}


/* MAIN FUNCTION */
void draw() 
{
  updateKeys();
  updateGui();
  drawCameras();
  
  getData();
  
  int time = millis() - connectTime;
  
  if (readyToSend() || true) {
    println(time);
    sendData();
  }
}


