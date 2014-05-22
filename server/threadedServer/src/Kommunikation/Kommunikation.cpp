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

	if (sem_init(&sem_message_vector, 0, 1) < 0) {
		std::cout << "Error: init sem_message_vector";
	}
	//cp = KommunikationsProtokoll(this);
}

Kommunikation::~Kommunikation() {
	// TODO Auto-generated destructor stub
	this->stop();
}



int Kommunikation::start() {
	int return_value = 0;
	if (!running) {
		running = true;
		this->thread_TcpBinder = new boost::thread(
				&Kommunikation::thread_Binder, this);
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

void Kommunikation::sendMessage(std::string str) {

	sem_wait(&sem_message_vector);
	printf("Push to Vector: %s\n", str.c_str());
	messages.push_back(str);
	sem_post(&sem_message_vector);
}

std::string Kommunikation::getMessage() {

	if (messages.size() < 0) {
		std::string s;
		sem_wait(&sem_message_vector);
		for (int i = 0; i < messages.size(); ++i) {

			s = s.append(messages.at(i).c_str());
		}
		sem_post(&sem_message_vector);
		return s;
	}
	else
		return NULL;
	// s an socket schicken
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
			if (numberOffClients <= 0) {
				this->thread_TcpSend = new boost::thread(
						boost::bind(&Kommunikation::thread_Sender, this,
								client_sock));


				if (this->thread_TcpSend == NULL)
					state = -1;

				this->thread_TcpRecive = new boost::thread(
						boost::bind(&Kommunikation::thread_Recive, this,
								client_sock));
				if (this->thread_TcpRecive == NULL)
					state = -1;

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

void Kommunikation::thread_Recive(int socket_desc) {
	try {
		//Get the socket descriptor
		int sock = socket_desc;
		int read_size;
		//std::vector<unsigned char> v;
		char *message, client_message[2000];
		std::string json = "";
		int ende; // letzte fund eines Simikolons
		int i;	  // laufvariable
		//Receive a message from client
		while ((read_size = recv(sock, client_message, 2000, 0)) > 0) {
			ende = 0;
			for (i = 0; i <= read_size; i++) {
				if (client_message[i] == ';') { //Befehl vollständig

					json.append(client_message, ende,i - ende);

					// Befehl ausführen
					{
						printf("Empfangen: %s\n", json.c_str());
						sendMessage(json);
					}
					json.clear();
					i++; // simikolon entfernen
					i++; // \n entfernen
					ende = i;
				}
			}
			if (!(i == (ende + 1))) { // nur in dem fall wo auch wirklich noch daten kamen
				json.append(client_message, i - ende - 1);
			}
			//clear the message buffer
			memset(client_message, 0, 2000);

			//if (zustand == -1)
			//	break;
			boost::this_thread::interruption_point();
		}
		thread_safe_print("Client disconnected.");
		numberOffClients--;
		this->thread_TcpSend->interrupt();
		this->thread_TcpSend->join();

		close(sock);

	} catch (boost::thread_interrupted&) {
		thread_safe_print("\nTcpSocket reader interrupted!");
		numberOffClients--;
	}
}

void Kommunikation::thread_Sender(int socket_desc) {

	int sock = socket_desc;
	std::string s;
	try {
		while (1) {

			if (messages.size() > 0) {
					std::string s;
					sem_wait(&sem_message_vector);
					for (int i = 0; i<messages.size();i++)
					//while ( messages.size() > 0)
					{
						s.append(messages[i]);
						s.append(";");
						//messages.erase(messages.begin());
					}
					messages.clear();
					sem_post(&sem_message_vector);
					//std::cout << s;
					printf("Sende    : %s\n", s.c_str());
					send(sock,s.c_str(),s.length(),0);
			}
			boost::this_thread::interruption_point();
		}
		//close(sock);
	} catch (boost::thread_interrupted&) {
		thread_safe_print("\nTcpSocket writer interrupted!");
	}
}

void Kommunikation::setSafePrintSemaphore(sem_t *sem) {
	this->sem_print = sem;
}

void Kommunikation::thread_safe_print(std::string str) {
	sem_wait(sem_print);
	std::cout << str;
	sem_post(sem_print);
}
