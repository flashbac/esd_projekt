/*
 * main.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include <stddef.h>
#include <iostream>
#include "Client.h"


int main(int argc, char** argv) {
	Client a("192.168.178.42",50000, "eth0");
	a.init();
	a.start();

	while(1);

	return 0;
}
