/*
 * TcpClient.h
 *
 *  Created on: 27.05.2014
 *      Author: rensky
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

class TcpProtokoll;

class TcpClient {
public:
	TcpClient(int sock);
	virtual ~TcpClient();
	void sendMessage(std::string str);
	void setSafePrintSemaphore(sem_t *sem);
	void thread_safe_print(std::string str);
	void stop();

private:
	bool running;

	boost::thread *thread_TcpSend;
	boost::thread *thread_TcpRecive;
	std::vector<std::string> messages;
	TcpProtokoll *kp;

	sem_t *sem_print;
	sem_t sem_message_vector;

	void thread_Sender(int socket_desc);
	void thread_Recive(int socket_desc);
	int init(int sock);


};

#endif /* TCPCLIENT_H_ */
