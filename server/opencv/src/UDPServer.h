/*
 * UDPServer.h
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#ifndef UDPSERVER_H_
#define UDPSERVER_H_

#define UDPServermaxClientThreads 20

#include <pthread.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<stdio.h>

class UDPServer {
public:
	UDPServer(int port=5000);
	virtual ~UDPServer();

	void start_thread_listen_and_accept();
private:
	int socket_desc;
	int client_sock;

	struct sockaddr_in server;
	struct sockaddr_in client;
};

#endif /* UDPSERVER_H_ */

/* BSP
 class EineKlasse
 {
 public:                              // öffentlich
 EineKlasse();                      // der Default-Konstruktor
 EineKlasse(int a=0);               // weiterer Konstruktor mit Parameter und Defaultwert
 EineKlasse(const EineKlasse& a);   // Copy-Konstruktor
 ~EineKlasse();                     // der Destruktor

 int eineFunktion(int x=42);        // eine Funktion mit einem (Default-) Parameter

 private:                             // privat
 int m_eineVariable;
 };
 */
