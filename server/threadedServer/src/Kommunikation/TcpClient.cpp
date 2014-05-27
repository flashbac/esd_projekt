/*
 * TcpClient.cpp
 *
 *  Created on: 27.05.2014
 *      Author: rensky
 */

#include "TcpClient.h"

TcpClient::TcpClient(int sock) {
	// TODO Auto-generated constructor stub

}

TcpClient::~TcpClient() {
	// TODO Auto-generated destructor stub
}

int TcpClient::init(int sock)
{
	int state = 0;
	this->thread_TcpSend = new boost::thread(
	boost::bind(&TcpClient::thread_Sender, this,
		sock));

	if (this->thread_TcpSend == NULL)
		state = -1;

	this->thread_TcpRecive = new boost::thread(
			boost::bind(&TcpClient::thread_Recive, this,
					sock));
	if (this->thread_TcpRecive == NULL)
		state = -1;

	return state;
}

void TcpClient::stop() {
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
	}
}


void TcpClient::sendMessage(std::string str) {

	sem_wait(&sem_message_vector);
	printf("Push to Vector: %s\n", str.c_str());
	messages.push_back(str);
	sem_post(&sem_message_vector);
}

void TcpClient::thread_Recive(int socket_desc) {
	try {
		//Get the socket descriptor
		int sock = socket_desc;
		int read_size;
		//std::vector<unsigned char> v;
		char client_message[2000];
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
						//sendMessage(json);
						kp->commandoProzess(json);

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

		this->thread_TcpSend->interrupt();
		this->thread_TcpSend->join();

		close(sock);

	} catch (boost::thread_interrupted&) {
		thread_safe_print("\nTcpSocket reader interrupted!");

	}
}

void TcpClient::thread_Sender(int socket_desc) {

	int sock = socket_desc;
	std::string s;
	try {
		while (1) {

			if (messages.size() > 0) {
					std::string s;
					sem_wait(&sem_message_vector);
					for (unsigned int i = 0; i<messages.size();i++)
					{
						s.append(messages[i]);
						s.append(";");
					}
					messages.clear();
					sem_post(&sem_message_vector);

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

void TcpClient::setSafePrintSemaphore(sem_t *sem) {
	this->sem_print = sem;
}

void TcpClient::thread_safe_print(std::string str) {
	if (sem_print != NULL)
	{
		sem_wait(sem_print);
		std::cout << str;
		sem_post(sem_print);
	}
	else
		printf("%s",str.c_str());
}
