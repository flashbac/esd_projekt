/*
 * Kommunikation.h
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#ifndef KOMMUNIKATION_H_
#define KOMMUNIKATION_H_

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


class Kommunikation {
public:
	Kommunikation();
	virtual ~Kommunikation();
	void thread_safe_print(std::string str);
	int start();
	void stop();



private:
	bool running;

	sem_t sem_print;

	boost::thread *thread_TcpSend;
	boost::thread *thread_TcpRecive;
	boost::thread *thread_TcpBinder;
	void thread_Sender(int *socket_desc);
	void thread_Recive(void *socket_desc);
	int thread_Binder();

};

#endif /* KOMMUNIKATION_H_ */
