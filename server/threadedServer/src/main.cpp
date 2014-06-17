/*
 * main.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include <semaphore.h>
//#include <iostream>

#include "Stream/Client.h"
#include "Kommunikation/TcpListenner.h"
#include "SerialWrapper.h"
uint8_t x = 90, y = 90;
int main(int argc, char** argv) {

	Helper h;
	sem_t sem_print;
	//std::string ip = "141.64.166.22";
	std::string ip = "192.168.1.230";
	//std::string ip = "192.168.178.42";
	std::string device = "eth0";
	int port = 50000;
	int camID = 0;
	int MTU = 1500;
	double cameraWidth = 640.0;
	double cameraHeigth = 480.0;

	double cameraFrameRate = 30.0;

	if (sem_init(&sem_print, 0, 1) < 0) {
		return -1;
		std::cout << "Error: init sem_print";
	}

	//argv auswertung
	// ip
	if (argc > 1)
		ip = argv[1];
	if (argc > 2)
		port = atoi(argv[2]);
	if (argc > 3)
		device = argv[3];
	if (argc > 4)
		camID = atoi(argv[4]);
	if (argc > 5)
		MTU = atoi(argv[5]);
	if (argc > 6)
		cameraWidth = atof(argv[6]);
	if (argc > 7)
		cameraHeigth = atof(argv[7]);
	if (argc > 8)
		cameraFrameRate = atof(argv[8]);
	// <------------ argv

	// print settings
	std::cout << "\nUsed Settings:\n\n  Destination IP:\t\t" << ip << ":"
			<< port << "\n  camID:\t\t\t" << camID
			<< "\n  device for MTU info:\t\t" << device
			<< "\n  cam settings:\t\t\t" << cameraWidth << "x" << cameraHeigth
			<< "@" << cameraFrameRate << "\n" << std::endl;

	// <----

	TcpListenner tcpL(MTU, device);
	tcpL.start();

	do {
		usleep(200);
		tcpL.cleaning();
	} while (!h.kbhit());
	tcpL.stop();
	/*
	 Client a(ip, port, camID, device);
	 //Client a("192.168.178.42", 50000, 0, "eth0");
	 a.setSafePrintSemaphore(&sem_print);
	 //setup Logitech c270 -> 640x360 @ 25 fps
	 a.setVideoSettings(cameraWidth, cameraHeigth, cameraFrameRate);
	 //a.setVideoSettings(640, 320, 25);
	 a.init();
	 a.setJpgQuality(20);
	 a.setMTUsize(1500);
	 a.start();*/

	//TcpListenner tcpL;
	//tcpL.start();
	return 0;
}
