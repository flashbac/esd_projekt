/*
 * Kommunikation.cpp
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#include "TcpConnection.h"

TcpConnection::TcpConnection() {

	numberOffClients = 0;

	if (sem_init(&sem_message_vector, 0, 1) < 0) {
		std::cout << "Error: init sem_message_vector";
	}

	thread_TcpBinder = NULL;

	running = 0;
	sem_print = NULL;
}

TcpConnection::~TcpConnection() {
	// TODO Auto-generated destructor stub
	this->stop();
}

int TcpConnection::start() {
	int return_value = 0;
	if (!running) {
		running = true;
		this->thread_TcpBinder = new boost::thread(
				&TcpConnection::thread_Binder, this);
		if (this->thread_TcpBinder == NULL)
			return_value = -1;

		if (return_value != 0) {
			this->stop();
		}
	} else
		return_value = -2;
	return return_value;
}

void TcpConnection::stop() {
	if (running) {
		running = false;

		if (this->thread_TcpBinder != NULL) {
			this->thread_TcpBinder->interrupt();
			this->thread_TcpBinder->join();
			delete (this->thread_TcpBinder);
		}
	}
}

int TcpConnection::thread_Binder() {

	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		thread_safe_print("Could not create socket.");

	}
	thread_safe_print("Socket created.");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);

	//Bind
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		thread_safe_print("Could not bind to socket. Error");
		return 1;
	}
	thread_safe_print("Sucessfully bind to socket");

	//Listen
	listen(socket_desc, 3);

	//Accept and incoming connection
	thread_safe_print("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	while ((client_sock = accept(socket_desc, (struct sockaddr *) &client,
			(socklen_t*) &c))) {
		thread_safe_print("Connection accepted ");

		int state = 0;
		if (running) {
			if (numberOffClients <= 0) {

				// Session erstellen

				Session s(client_sock);

				sessions.push_back(s);


				if (state != 0) {
					this->stop();
				} else {
					numberOffClients = 1;
				}
			} else {
				std::string msg("Server accept only one Client.");
				send(client_sock, msg.c_str(), msg.length(), 0);
				close(client_sock);
			}
		} else {
			thread_safe_print(
					"Can not connect bind socket is not in lissen mode.");
		}

		if (client_sock < 0) {
			thread_safe_print("accept failed");
			return 1;
		}
	}
	return 0;
}

void TcpConnection::setSafePrintSemaphore(sem_t *sem) {
	this->sem_print = sem;
}

void TcpConnection::thread_safe_print(std::string str) {
	if (sem_print != NULL)
	{
		sem_wait(sem_print);
		std::cout << str;
		sem_post(sem_print);
	}
	else
		printf("%s",str.c_str());
}
