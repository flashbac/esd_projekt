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
#include "KommunikationsProtokoll.h"


class Kommunikation {
public:
	Kommunikation();
	virtual ~Kommunikation();
	void setSafePrintSemaphore(sem_t *sem);
	void thread_safe_print(std::string str);
	int start();
	void stop();
	void sendMessage(std::string str);
	void setKommunikationsProtokoll(KommunikationsProtokoll *k);
	KommunikationsProtokoll *cp;



private:
	bool running;
	int numberOffClients;
	sem_t *sem_print;
	sem_t sem_message_vector;
	boost::atomic<bool> done;
	//boost::lockfree::queue queue;

	boost::thread *thread_TcpSend;
	boost::thread *thread_TcpRecive;
	boost::thread *thread_TcpBinder;
	std::vector<std::string> messages;


	void thread_Sender(int socket_desc);
	void thread_Recive(int socket_desc);
	int thread_Binder();
	std::string getMessage();

};

#endif /* KOMMUNIKATION_H_ */
