/*
 * UDPServer.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#include "UDPServer.h"


UDPServer::UDPServer(int port) {
	// TODO Auto-generated constructor stub


	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		perror("Could not create socket");
		this->~UDPServer();
	}
	puts("Socket created");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//Bind
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		this->~UDPServer();
	}
	puts("bind done");

	//Listen
	listen(socket_desc, 3);

}

UDPServer::~UDPServer() {
	// TODO Auto-generated destructor stub
	socket_desc = -1;
}

void wait_for_next_command()
{
	//pthread_create(thread,NULL,func_name, NULL);
}

