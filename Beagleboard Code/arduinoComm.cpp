#include <errno.h>      // For error nums
#include <termios.h>	// For Serial comm
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_RDWR
#include <unistd.h>     // For open(), creat()
#include <time.h>       // For delay
#include <signal.h>     // For system signals
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions

/* Prototyptes */
int set_interface_attribs (int, int, int);
void set_blocking (int, int);
void error_message (char[], int);
void error_message (char[], int, char[], char[]);
void delay (unsigned int);
void handler (int);
void enablePing ();
void writeAngle (int);
void writeSpeed (double);
void writekp (double);
void writeki (double);
void writekd (double);
void readSerial ();
void readSerial (int);

using namespace std;


/************************
 *** GLOBAL VARIABLES ***
 ************************/
// Serial Port
int fd;
bool txReady;

// Loop timer
time_t timer;
double seconds;


int set_interface_attribs (int fd, int speed, int parity) {
	struct termios tty;
    
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0) {
		error_message ("error %d from tcgetattr", errno);
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;	// 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;			// ignore break signal
	tty.c_lflag = 0;			// no signaling chars, no echo,
							// no canonical processing
	tty.c_oflag = 0;			// no remapping, no delays
	tty.c_cc[VMIN]  = 0;			// read doesn't block
	tty.c_cc[VTIME] = 5;			// 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY);	// shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);	// ignore modem controls,
						// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);	// shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
		error_message ("error %d from tcsetattr", errno);
		return -1;
	}
	return 0;
}

void set_blocking (int fd, int should_block) {
	struct termios tty;	
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		error_message ("error %d from tggetattr", errno);
		return;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;			// 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
		error_message ("error %d setting term attributes", errno);
}

void error_message (char msg[], int errNum) {
	printf( msg, errNum);
}

void error_message (char msg[], int errNum, char portName[], char strErr[]) {
	printf( msg, errNum, portName, strErr);
}

void delay (unsigned int ms) {
    usleep(ms * 1000); //microseconds
}

void handler (int s){
	if (s == 2) {
		printf("\nExiting...\nClosing serial port...\n");
		close(fd);
	} else {
		printf("\nCaught signal %d\n",s);
	}
	exit(1); 
}

/**************************
 ** Serial Communications
 **************************/

void beginSerial() {
    /* Handle Signals */
	struct sigaction sigIntHandler;
    
	sigIntHandler.sa_handler = handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
    
	sigaction(SIGINT, &sigIntHandler, NULL);
    
	/* Open Serial Port */
	char *portname = "/dev/ttyACM0";
    
	fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		error_message ("error %d opening %s: %s", errno, portname, strerror (errno));
		return -1;
	}
    
	set_interface_attribs (fd, B57600, 0);	// set speed to 57,600 bps, 8n1 (no parity)
	set_blocking (fd, 0);                   // set no blocking
}

// Serial Writes

void enablePing() {
    time(&timer);
    
    seconds = difftime(timer,time());
    write (fd, "00,000001", 10);
    printf("Loop time: %d", seconds);
    txReady = false;
}

void writeAngle (int ang) {
    char output[10];
    sprintf(output, "01,%6i", ang);
    write (fd, output, 10);
    txReady = false;
}

void writeSpeed (double speed) {
    char output[10];
    sprintf(output, "02,%6d", speed);
    write (fd, output, 10);
    txReady = false;
}

void writeKp (double Kp) {
    char output[10]
    sprintf(output, "01,%6%d", Kp);
    write (fd, output, 10);
    txReady = false;
}

void writeKi (double Ki) {
    char output[10]
    sprintf(output, "01,%6%d", Ki);
    write (fd, output, 10);
    txReady = false;
}
void writeKd (double Kd) {
    char output[10]
    sprintf(output, "01,%6%d", Kd);
    write (fd, output, 10);
    txReady = false;
}

// Serial Reads

char[] readSerial () {
    char buf [1024];
    bzero(buf, 1024);
    responseLen = read (fd, buf, sizeof(buf));	// read up to 1024 characters if ready to read
    
    if (!txREady) {
        for (int i = 0; i < responseLen; i++) {
            if (buf[i] == '*') {
                txReady = true;
            }
        }
    }
}

char[] readSerial (int len) {
    char buf [len];
    bzero(buf, sizeof(buf));
    responseLen = read (fd, buf, sizeof(buf));
    
    if (!txREady) {
        for (int i = 0; i < responseLen; i++) {
            if (buf[i] == '*') {
                txReady = true;
            }
        }
    }
}

bool serialReady () {
    if (txReady) {
        return true;
    }   else    {
        return false;
    }
}

int main(int argc, const char* argv[]) {
	
	int ang = 400;
	char angBuff[10];
	char responseLen = -1;

	while (1) {
        if (serialReady()) {
            enablePing();
            writeAngle();
        }
        
		delay(100);
	}
	return 0;
}
