/*
 * UDPClient.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#include "UDPClient.h"

UDPClient::UDPClient(std::string ipadress, int port) {
	// TODO Auto-generated constructor stub

	if (ipadress.empty())
		this->~UDPClient();
	if ((port < 1) && (port > 65535))
		this->~UDPClient();

	/* create a socket */
	sockd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockd == -1) {
		perror("Socket creation error");
		this->~UDPClient();
	}
	// for broadcasting
	/*
	 int val = 1;
	 setsockopt(sockd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
	 */

	/* client address */
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = 0;

	if (bind(sockd, (struct sockaddr*) &my_addr, sizeof(my_addr)) < 0) {
		perror("bind failed. Error");
		this->~UDPClient();
	}

	/* server address */
	srv_addr.sin_family = AF_INET;
	inet_aton(ipadress.c_str(), &srv_addr.sin_addr);
	srv_addr.sin_port = htons(port);
}

UDPClient::~UDPClient() {
	// TODO Auto-generated destructor stub
	close(sockd);
}

int UDPClient::sendData(void *buffer, size_t length) {

	return sendto(sockd, (const char*) buffer, length, 0, (sockaddr*) &srv_addr,
			sizeof(srv_addr));
}
