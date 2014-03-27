//
// ariesServer.cpp
// ARIES_server
//
// Ported from AdamBots 2014 BeagleBone code by Jason Merlo on 2/25/14.
//
// Description: Simple program to listen on port for commands and control Arduino
// To compile: g++ -o ARIES_Server  ariesServer.cpp -L /usr/lib/libpthread*.so
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> // For TCP
#include <netinet/in.h> // For TCP
#include <pthread.h>    // For Posix threads
#include <fcntl.h>      // For O_RDWR
#include <unistd.h>     // For open(), creat()
#include <time.h>       // For delay
#include <signal.h>     // For system signals
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions
#include <errno.h>      // For error nums
#include <termios.h>    // For Serial comm

using namespace std;

/* Prototypes */
/*
 void error_message (char[]);
 void error_message (char[], int);
 void error_message (char[], int, char[], char[]);
 int set_interface_attribs (int, int, int);
 void set_blocking (int, int);
 void delay (unsigned int);
 void handler (int);
 void tcpServer(void);
 */
void error_message(const char *msg)
{
	perror(msg);
	exit(1);
}

void error_message (char msg[], int errNum) {
    printf( msg, errNum);
}

void error_message (char msg[], int errNum, char portName[], char strErr[]) {
    printf( msg, errNum, portName, strErr);
}

int portno;

int fd, sockfd, newsockfd; // Ports: fd = serial, sockfd = network, newsockfd = network

const char responseLen = 32;
char *buffer[responseLen];
int dataReady = 0;

int set_interface_attribs (int fd, int speed, int parity) {
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        error_message ("error %d from tcgetattr", errno);
        return -1;
    }
    
    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);
    
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;                 // ignore break signal
    tty.c_lflag = 0;                        // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                        // no remapping, no delays
    tty.c_cc[VMIN]  = 0;                    // read doesn't block
    tty.c_cc[VTIME] = 5;                    // 0.5 seconds read timeout
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
    
    tty.c_cflag |= (CLOCAL | CREAD);        // ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
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
    tty.c_cc[VTIME] = 5;                    // 0.5 seconds read timeout
    
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
        error_message ("error %d setting term attributes", errno);
}

void delay (unsigned int ms) {
    /* save start clock tick */
    const clock_t start = clock();
    
    clock_t current;
    while ((double)(current-start)/CLOCKS_PER_SEC < ms / 1000) {
        current = clock();
    }
}

void handler(int s){
    if (s == 2) {
        printf("\nExiting...\nClosing serial port...\nClosing Networks...\n");
        close(fd);
        close(sockfd);
        close(newsockfd);
    } else {
        printf("\nCaught signal %d\n",s);
    }
    exit(1);
}

void* serialComm(void*) {
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
    
    set_interface_attribs (fd, B9600, 0);   // set speed to 9,600 bps, 8n1 (no parity)
    set_blocking (fd, 0);                   // set no blocking
    
    while ( true ) {
		if (dataReady == 1) {
			write (fd, &buffer, responseLen);
			dataReady = 0;
			printf("buffer= %s", buffer);
		}
		printf("dataReady: %i\n", dataReady);
		delay(100);
	}
}

void* tcpServer(void*) {
	printf("Listening on port [%d]\n",portno);
	////////////////////////////////
	// Networking Setup Variables //
	/* Initialize variables */
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
    
	int n;
    
	/* Create socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
	/* Check if socket was opened */
	if (sockfd < 0)
		error_message("ERROR opening socket");
    
	bzero((char *) &serv_addr, sizeof(serv_addr));  // places 0's in RAM where indicated
    
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
    
	/* Bind socket */
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error_message("ERROR on binding");
    
	/* Wait for message on port */
    
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
    
	///////////////////////
	// Main program loop //
	printf("Waiting for request...\n");
	/* Wait for new socket connection */
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error_message("ERROR on accept");
    
	printf("Accepted\n");
    
    
	while ( true ) {
		if (dataReady == 0) {
			bzero(&buffer, responseLen);
            
			if (read(newsockfd, &buffer,responseLen) < 0)
				error_message("ERROR reading from socket");

			/* Send success message varification back go client */
			if (send(newsockfd,buffer,responseLen,0) == -1)
				error_message("ERROR on sending");
			
			delay(10);
			dataReady = 1;
		}
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}

int main(int argc,char *argv[]) {
    
    
	pthread_t tcpThread;
    pthread_t serialThread;
    
	if (argc >= 2)
		portno = atoi(argv[1]);
	else
		error_message("No port specified");
    
	pthread_create(&tcpThread,NULL,tcpServer,NULL);
    pthread_create(&serialThread,NULL,serialComm,NULL);
    
    while (1) {
        delay(100);
    }
    
	return 0;
}

