/*
 * Kommunikation.cpp
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#include "Kommunikation.h"

Kommunikation::Kommunikation() {
	// TODO Auto-generated constructor stub
	numberOffClients = 0;
}

Kommunikation::~Kommunikation() {
	// TODO Auto-generated destructor stub
	this->stop();
}

int Kommunikation::start() {
	int return_value = 0;
	if (!running) {
		running = true;
		this->thread_TcpBinder = new boost::thread(&Kommunikation::thread_Binder,
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

void Kommunikation::stop() {
	if (running) {
		running = false;
		if (this->thread_TcpSend != NULL) {
			this->thread_TcpSend->interrupt();
			this->thread_TcpSend->join();
			delete (this->thread_TcpSend);
		}
		if (this->thread_TcpRecive != NULL) {
			this->thread_TcpRecive->interrupt();
			this->thread_TcpRecive->join();
			delete (this->thread_TcpRecive);
		}
		if (this->thread_TcpBinder != NULL) {
			this->thread_TcpBinder->interrupt();
			this->thread_TcpBinder->join();
			delete (this->thread_TcpBinder);
		}
	}
}

int Kommunikation::thread_Binder() {

	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		thread_safe_print("Could not create socket.");

	}
	thread_safe_print("Socket created.");

	//Prepare the sockaddr_in structure
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
			if (thread_TcpSend != NULL && thread_TcpRecive != NULL)
			{
				this->thread_TcpSend = new boost::thread(boost::bind(&Kommunikation::thread_Sender, this, client_sock));

				if (this->thread_TcpSend == NULL)
					state = -1;

				this->thread_TcpRecive = new boost::thread(boost::bind(
						&Kommunikation::thread_Recive, this, client_sock));
				if (this->thread_TcpRecive == NULL)
					state = -1;

				if (state != 0) {
					this->stop();
				}
				else
				{
					numberOffClients = 1;
				}
			}
			else
			{
				std::string msg("Server accept only one Client.");
				send(client_sock, msg.c_str(), msg.length(), 0);
				close(client_sock);
			}
		} else {
			thread_safe_print("Can not connect bind socket is not in lissen mode.");
		}

		if (client_sock < 0) {
			thread_safe_print("accept failed");
			return 1;
		}
		return 0;

	}
	return 1;
}

void Kommunikation::thread_Recive(int socket_desc) {
	try {
	    //Get the socket descriptor
	    int sock = socket_desc;
	    int read_size;
	    //std::vector<unsigned char> v;
	    char *message , client_message[2000];
		//Receive a message from client
		while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
		{
			//end of string marker
			client_message[read_size] = '\0';
			printf("Empfangen: %s\n", client_message);

			//zustand = commando(client_message);

			message = client_message;
			printf("Sende    : %s\n", message);
			//Send the message back to client
			write(sock , message , strlen(message));


			//clear the message buffer
			memset(client_message, 0, 2000);

			//if (zustand == -1)
			//	break;
		}

	} catch (boost::thread_interrupted&) {
		thread_safe_print("\nthread_kamera_reader interrupted!");
	}
}

void Kommunikation::thread_Sender(int socket_desc) {
	try {
		while (1) {

		}
	} catch (boost::thread_interrupted&) {
		thread_safe_print("\nthread_kamera_reader interrupted!");
	}
}

void Kommunikation::thread_safe_print(std::string str) {
	sem_wait(&sem_print);
	std::cout << str;
	sem_post(&sem_print);
}
