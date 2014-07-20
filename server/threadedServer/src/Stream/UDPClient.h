/*
 * UDPClient.h
 *
 *  Created on: 08.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef UDPCLIENT_H_
#define UDPCLIENT_H_

#define MAX_BUF 100

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

class UDPClient {
public:
	UDPClient(std::string ipadress, int port=50000);
	virtual ~UDPClient();

	int sendData(void* buffer, size_t length);

private:
	  int sockd;
	  struct sockaddr_in my_addr, srv_addr;
};

#endif /* UDPCLIENT_H_ */
