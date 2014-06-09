/////////////
// IMPORTS //
/////////////
import ipcapture.*;       // MJPEG lib
import processing.serial.*;  // Serial lib

// OpenCV Imports
import gab.opencv.*;
import org.opencv.core.Mat;
import org.opencv.calib3d.StereoBM;
import org.opencv.core.CvType;
import org.opencv.calib3d.StereoSGBM;

/////////////
// OBJECTS //
/////////////
// SERIAL DEVICES
Serial arduino;

///////////////
// CONSTANTS //
///////////////
// Ports
final String ip = "192.168.1.131";
final int camPortA = 8091;
final int camPortB = 8090;
final String camUrl = "/?action=stream.mjpg";
final String serialPort = "/dev/tty.usbserial";  // All or part of the serial port name

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
int responseTime = -1;
float avgRespTime = 0;
float lastAvgRespTime = 0;
int connectTime = 0;
int lastSendTime = -1;

// Voltage
float avgBattVoltage;
float lastAvgBattVoltage = 7.4;
float battVoltage = 7.4;

/* INITIALIZATION FUNCTION */
void setup() 
{
  /* WINDOW SETUP */
  size(1000, 430, "processing.core.PGraphicsRetina2D");
  frameRate(30);

  /* CAMERA COMMUNICATION */
  // Connect to cameras
  camA = new IPCapture(this, "http://" + ip + ":" + camPortA + camUrl, "", "");
  camB = new IPCapture(this, "http://" + ip + ":" + camPortB + camUrl, "", "");
  // Start camera streams
  camA.start();
  camB.start();

  /* INITIALIZE GUI */
  initGui();

  /* INITIALIZE OPENCV */
  initStereo();

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

  try {
    getData();
  } 
  catch (NullPointerException e) {
    e.printStackTrace();
  }

  responseTime = millis() - connectTime;

  avgRespTime = lastAvgRespTime * 0.9999 + responseTime * 0.0001;
  lastAvgRespTime = avgRespTime;

  avgBattVoltage = lastAvgBattVoltage * 0.9 + battVoltage * .1;
  lastAvgBattVoltage = avgBattVoltage;

  try {
    sendData();
  } 
  catch (NullPointerException e) {
    e.printStackTrace();
  }
}

