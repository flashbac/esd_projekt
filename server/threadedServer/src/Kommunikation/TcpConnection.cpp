/*
 * TcpClient.cpp
 *
 *  Created on: 27.05.2014
 *      Author: rensky
 */

#include "TcpConnection.h"

TcpConnection::TcpConnection(int sock, TcpProtokoll *tcpP, FugexySession *session) {
	this->sock = sock;
	this->session = session;
	this->kp = tcpP;

	if (sem_init(&sem_message_vector, 0, 1) < 0) {
		thread_safe_print("Error: init sem_message_vector");
	}

	messages.clear();
	init();

}

TcpConnection::~TcpConnection() {
	// TODO Auto-generated destructor stub
}

int TcpConnection::init()
{
	int state = 0;
	this->thread_TcpSend = new boost::thread(
	boost::bind(&TcpConnection::thread_Sender, this,
		sock));

	if (this->thread_TcpSend == NULL)
		state = -1;

	this->thread_TcpRecive = new boost::thread(
			boost::bind(&TcpConnection::thread_Recive, this,
					sock));
	if (this->thread_TcpRecive == NULL)
		state = -1;

	if (state == 0)
	{
		running = true;
	}
	return state;
}

void TcpConnection::stop() {
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


void TcpConnection::sendMessage(std::string str) {

	if (running == true)
	{
		sem_wait(&sem_message_vector);
		printf("Push to Vector: %s\n", str.c_str());
		messages.push_back(str);
		sem_post(&sem_message_vector);
	}
}

void TcpConnection::thread_Recive(int socket_desc) {
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

void TcpConnection::thread_Sender(int socket_desc) {

	int sock = socket_desc;
	std::string s;
	try {
		while (1) {

			if (messages.size() > 0) {
					std::string s;
					sem_wait(&sem_message_vector);
					if (!messages.empty())
					{
						for (unsigned int i = 0; i<messages.size();i++)
						{
							s.append(messages[i]);
							s.append(";");
						}
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
