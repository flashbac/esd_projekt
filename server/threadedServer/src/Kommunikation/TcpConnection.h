/*
 * TcpClient.h
 *
 *  Created on: 27.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include <boost/thread/thread.hpp>
#include <semaphore.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include "./TcpProtokoll.h"
#include "../VugexySession.h"
#include "../ThreadSafeLogger.h"

class TcpProtokoll;
class VugexySession;

class TcpConnection {
public:
	TcpConnection(int sock, TcpProtokoll *tcpP);
	virtual ~TcpConnection();
	void sendMessage(std::string str);
	void stop();

private:
	bool running;

	boost::thread *thread_TcpSend;
	boost::thread *thread_TcpRecive;
	std::vector<std::string> messages;
	TcpProtokoll *tcpP;
	int sock;

	sem_t sem_message_vector;

	void thread_Sender(int socket_desc);
	void thread_Recive(int socket_desc);
	int init();

};

#endif /* TCPCLIENT_H_ */
