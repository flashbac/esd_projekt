/*
 * Kommunikation.cpp
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#include "TcpListenner.h"

TcpListenner::TcpListenner(int globalMTU, std::string globalOutgoingDevice) {

	numberOffClients = 0;
	thread_TcpBinder = NULL;
	running = 0;
	this->globalMTU = globalMTU;
	this->globalOutgoingDevice = globalOutgoingDevice;
}

TcpListenner::~TcpListenner() {
	// TODO Auto-generated destructor stub
	this->stop();
}

int TcpListenner::start() {
	int return_value = 0;
	if (!running) {
		running = true;
		this->thread_TcpBinder = new boost::thread(&TcpListenner::thread_Binder,
				this);
		if (this->thread_TcpBinder == NULL)
			return_value = -1;

		if (return_value != 0) {
			this->stop();
		}
	} else
		return_value = -2;
	return return_value;
}

int TcpListenner::cleaning() {
	for (unsigned int i = 0; i < sessions.size();) {
		if (!sessions[i]->isClientConnected() && sessions[i] != NULL) {
			FugexySession *s = sessions[i];
			sessions.erase(sessions.begin() + i);
			numberOffClients--;
			delete s;
		} else
			i++;
	}
	return 0;
}

void TcpListenner::stop() {
	if (running) {
		running = false;

		if (this->thread_TcpBinder != NULL) {
			this->thread_TcpBinder->interrupt();
			this->thread_TcpBinder->join();
			delete (this->thread_TcpBinder);
		}
	}
}

int TcpListenner::thread_Binder() {

	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		ThreadSafeLogger::instance().print(
				"[debug]\t[TcpListner] Could not create socket.");

	}
	ThreadSafeLogger::instance().print("[debug]\t[TcpListner] Socket created.");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);

	//Bind
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		ThreadSafeLogger::instance().print(
				"[debug]\t[TcpListner] Could not bind to socket. Error");
		return 1;
	}
	//ThreadSafeLogger::instance().print("Sucessfully bind to socket");

	//Listen
	listen(socket_desc, 3);

	//Accept and incoming connection
	ThreadSafeLogger::instance().print(
			"[debug]\t[TcpListner] Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	while ((client_sock = accept(socket_desc, (struct sockaddr *) &client,
			(socklen_t*) &c))) {
		//ThreadSafeLogger::instance().print("Connection accepted ");

		if (client_sock > 0) {
			if (running) {
				if (numberOffClients <= 1) {

					FugexySession *s = new FugexySession(client_sock,
							this->globalMTU, this->globalOutgoingDevice);
					sessions.push_back(s);
					numberOffClients++;
				} else {
					std::stringstream ss;
					ss << "Server accept only " << numberOffClients
							<< "Clients";

					send(client_sock, ss.str().c_str(), ss.str().length(), 0);
					close(client_sock);
				}
			} else {
				ThreadSafeLogger::instance().print(
						"[debug]\t[TcpListner] Can not connect bind socket is not in lissen mode.");
			}

		} else {
			ThreadSafeLogger::instance().print(
					"[debug]\t[TcpListner] accept failed");
			return 1;
		}
	}
	return 0;
}
