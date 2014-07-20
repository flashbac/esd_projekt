/*
 * main.cpp
 *
 *  Created on: 18.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#include <semaphore.h>

#include "Kommunikation/TcpListenner.h"
#include "SerialWrapper.h"
uint8_t x = 90, y = 90;

volatile int quit_signal=0;
#include <signal.h>
extern "C" void quit_signal_handler(int signum) {
 if (quit_signal!=0) exit(0); // just exit already
 quit_signal=1;
}


int main(int argc, char** argv) {
	signal(SIGINT,quit_signal_handler); // listen for ctrl-C

	std::string device = "eth0";
	int port = 50000;
	int MTU = 1500;

	double cameraFrameRate = 30.0;

	if (argc > 1)
		device = argv[3];
	if (argc > 2)
		MTU = atoi(argv[5]);

	TcpListenner tcpL(MTU, device);
	tcpL.start();

	do {
		usleep(200);
		tcpL.cleaning();
	} while (!quit_signal);
	tcpL.stop();
	return 0;
}
