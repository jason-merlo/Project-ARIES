//
// ariesServer.cpp
// ARIES_server
//
// Written by Jason Merlo on 4/1/14.
//
// Description: Simple program to listen on port for commands and control Arduino
// To compile: g++ -o ARIES_Server  ariesServer.cpp -L /usr/lib/libpthread*.so
//
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
#include <sys/socket.h> // For socket functions
#include <netdb.h>      // For socket functions
#include <netinet/in.h> // For TCP
#include <pthread.h>    // For Posix threads

/* Prototyptes */
int set_interface_attribs (int, int, int);
void set_blocking (int, int);
void error_message (char[]);
void error_message (char[], int);
void error_message (char[], int, char[], char[]);
void delay (unsigned int);
void handler (int);
void initSerial();
void enablePing ();
void writeAngle (int);
void writeSpeed (double);
void writekp (double);
void writeki (double);
void writekd (double);
int readSerial ();
int readSerial (int);
// Threads
void serialThread (void);
void tcpThread (void);

using namespace std;

/************************
 *** GLOBAL VARIABLES ***
 ************************/
// Serial Port
int fd;
bool txReady;

// TCP Port
int portno;
int sockfd, newsockfd;

// Serial Input
const int serialBufferLen = 1024;
char serialBuffer[serialBufferLen];

// TCP Input
const int tcpBufferLen = 10;
char tcpBuffer[tcpBufferLen];

// Thread Variables
volatile bool dataReady = false;

/***********************
 ** General Functions **
 ***********************/

void error_message(const char *msg)
{
	perror(msg);
	exit(1);
}

void error_message (char msg[], int errNum) {
	printf( msg, errNum);
    exit(1);
}

void error_message (char msg[], int errNum, char portName[], char strErr[]) {
	printf( msg, errNum, portName, strErr);
    exit(1);
}

void delay (unsigned int ms) {
    usleep(ms * 1000); //microseconds
}

void handler (int s){
	if (s == 2) {
		printf("\nExiting...\nClosing serial port...\nClosing Networks...\n");
        close(fd);
        close(sockfd);
        close(newsockfd);
        printf("All Sockets Closed\n");
	} else {
		printf("\nCaught signal %d\n",s);
	}
	exit(1);
}

/************************
 ** TCP Init Functions **
 ************************/

void initTcp (int portNum) {
	printf("Listening on port [%d]\n",portNum);
	////////////////////////////////
	// Networking Setup Variables //
	/* Initialize variables */
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
    
	/* Create socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
	/* Check if socket was opened */
	if (sockfd < 0)
		error_message("ERROR opening socket\n");
    
	bzero((char *) &serv_addr, sizeof(serv_addr));
    
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portNum);
    
	/* Bind socket */
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error_message("ERROR on binding\n");
    
	/* Wait for message on port */
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
    
	printf("Waiting for request...\n");
	/* Wait for new socket connection */
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error_message("ERROR on accept\n");
    
	printf("Network Socket Accepted\n");
}

/***************************
 ** Serial Init Functions **
 ***************************/

int set_interface_attribs (int fd, int speed, int parity) {
	struct termios tty;
    
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0) {
		error_message ("error %d from tcgetattr\n", errno);
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
		error_message ("error %d from tcsetattr\n", errno);
		return -1;
	}
	return 0;
}

void set_blocking (int fd, int should_block) {
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		error_message ("error %d from tggetattr\n", errno);
		return;
	}
    
	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;			// 0.5 seconds read timeout
	
	if (tcsetattr (fd, TCSANOW, &tty) != 0)
		error_message ("error %d setting term attributes\n", errno);
}

void initSerial() {
    /* Handle Signals */
	struct sigaction sigIntHandler;
    
	sigIntHandler.sa_handler = handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
    
	sigaction(SIGINT, &sigIntHandler, NULL);
    
	/* Open Serial Port */
	char *portname = "/dev/ttyACM0";
    
    printf("Opening %s\n", portname);
    
	fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		error_message ("error %d opening %s: %s\n", errno, portname, strerror (errno));
	}
    
	set_interface_attribs (fd, B57600, 0);	// set speed to 57,600 bps, 8n1 (no parity)
	set_blocking (fd, 0);                   // set no blocking
    
    printf("Serial Socket Opened\n");
}

/***************************
 ** Serial Communications **
 ***************************/

// Serial Writes

void enablePing() {
    write (fd, "00,000001:", 10);
    txReady = false;
}

void writeAngle (int ang) {
    char output[10];
    sprintf(output, "01,%6i:", ang);
    write (fd, output, 10);
    txReady = false;
}

void writeSpeed (double speed) {
    char output[10];
    sprintf(output, "02,%6d:", speed);
    write (fd, output, 10);
    txReady = false;
}

void writeKp (double Kp) {
    char output[10];
    sprintf(output, "01,%6%d:", Kp);
    write (fd, output, 10);
    txReady = false;
}

void writeKi (double Ki) {
    char output[10];
    sprintf(output, "01,%6%d:", Ki);
    write (fd, output, 10);
    txReady = false;
}
void writeKd (double Kd) {
    char output[10];
    sprintf(output, "01,%6%d:", Kd);
    write (fd, output, 10);
    txReady = false;
}

// Serial Reads

int readSerial () {
    int responseLen;
    char buf[1024];
    bzero(buf, 1024);
    responseLen = read (fd, buf, sizeof(buf));	// read up to 1024 characters if ready to read
    
    if (!txReady) {
        for (int i = 0; i < responseLen; i++) {
            if (buf[i] == '*') {
                txReady = true;
            }
        }
    }
    
    bzero(serialBuffer, responseLen);
    
    for(int i = 0; i < responseLen; i++) {
        serialBuffer[i] = buf[i];
    }
    
    return responseLen;
}

int readSerial (int len) {
    int responseLen;
    char buf[len];
    bzero(buf, sizeof(buf));
    responseLen = read (fd, buf, sizeof(buf));
    
    if (!txReady) {
        for (int i = 0; i < responseLen; i++) {
            if (buf[i] == '*') {
                txReady = true;
            }
        }
    }
    
    bzero(serialBuffer, responseLen);
    
    for(int i = 0; i < responseLen; i++) {
        serialBuffer[i] = buf[i];
    }
    
    return responseLen;
}

bool serialReady () {
    if (txReady) {
        return true;
    }   else    {
        return false;
    }
}

/*********************
 ** Program Threads **
 *********************/

// Serial Thread
void* serialThread(void*) {
    
    initSerial();
    
    int loopcount = 0;
    
    while ( true ) {
		printf("SERIAL LOOP\n");
		if (dataReady == true) {
			printf("buffer= %s\n", &tcpBuffer);
			write (fd, &tcpBuffer, tcpBufferLen);
			printf("Serial written\n");
			bzero(&tcpBuffer, tcpBufferLen);
			dataReady = false;
		}
        delay(1);
	}
}

// TCP Thread (Adapted from AdamBots 2014 BeagleBone TCP Server)
void* tcpThread(void*) {
	   
	initTcp(portno);
	
	int tcpResponseLen;
	int valueId;
	int valueIdLen = 2;
	char valueIdBuffer[2];
    
	while ( true ) {
	if (dataReady == false) {
	/* Ping client for new data (sends last valueID)*/
		if (send(newsockfd,valueIdBuffer,2,0) == -1)
			error_message("ERROR on sending\n");
            
			if(tcpResponseLen = read(newsockfd, tcpBuffer, tcpBufferLen) < 0)
				error_message("ERROR reading from socket\n");
			
			/* Parse for valueID */
			bzero(valueIdBuffer, valueIdLen);

			for (int i = 0; i < 2; i++) {
				valueIdBuffer[i] = tcpBuffer[i];
			}

			valueId = atoi(valueIdBuffer);

			dataReady = true;
			delay(1);
		}
		delay(1);
	}
	close(newsockfd);
	close(sockfd);
	printf("Network Socket Closed\n");
	return 0;
}

/******************
 ** Main Program **
 ******************/

int main(int argc, const char* argv[]) {
	
	pthread_t _tcpThread;
	pthread_t _serialThread;
    
	if (argc >= 2)
		portno = atoi(argv[1]);
	else
		error_message("No port specified\n");
    
	pthread_create(&_tcpThread,NULL,tcpThread,NULL);
	pthread_create(&_serialThread,NULL,serialThread,NULL);
    
	while( true ) {
		printf("MAIN LOOP\n");
		delay(1000);
	}
}
