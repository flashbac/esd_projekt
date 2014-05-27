/*
 * Kommunikation.h
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#ifndef KOMMUNIKATION_H_
#define KOMMUNIKATION_H_

#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic/atomic.hpp>
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
#include "../Session.h"

class TcpProtokoll;

class TcpConnection {
public:
	TcpConnection();
	virtual ~TcpConnection();
	void setSafePrintSemaphore(sem_t *sem);
	void thread_safe_print(std::string str);
	int start();
	void stop();
	void sendMessage(std::string str);

	std::vector<Session> sessions;

private:
	bool running;
	int numberOffClients;
	sem_t *sem_print;
	sem_t sem_message_vector;

	boost::thread *thread_TcpBinder;

	int thread_Binder();

};

#endif /* KOMMUNIKATION_H_ */
