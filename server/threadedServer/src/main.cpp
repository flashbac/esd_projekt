/*
 * main.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include <stddef.h>
#include <iostream>
#include "Client.h"
#include "Kommunikation/Kommunikation.h"

int main(int argc, char** argv) {
	Client a("192.168.178.42",50000, "eth0");
	a.init();
	a.start();
	Kommunikation TcpSocket;// = new Kommunikation();
	TcpSocket.start();


	while(1);

	return 0;
}
