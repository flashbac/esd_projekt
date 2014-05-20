/*
 * main.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include <semaphore.h>
//#include <stddef.h>
#include <iostream>

#include "Client.h"
#include "Kommunikation/Kommunikation.h"

int main(int argc, char** argv) {

	sem_t sem_print;

	if (sem_init(&sem_print, 0, 1) < 0) {
		return -1;
		std::cout << "Error: init sem_print";
	}

	//Client a("141.64.164.112",50000, 0, "eth0");
	Client a("192.168.1.126",50000, 0, "eth0");
	a.setSafePrintSemaphore(&sem_print);
	a.init();
	a.setJpgQuality(10);
	a.setMTUsize(10240);
	a.start();

	Kommunikation TcpSocket;
	TcpSocket.setSafePrintSemaphore(&sem_print);
	TcpSocket.start();


	while(1);

	return 0;
}
