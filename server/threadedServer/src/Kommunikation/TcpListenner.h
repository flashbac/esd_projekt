/*
 * Kommunikation.h
 *
 *  Created on: 18.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
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
#include "./TcpProtokoll.h"
#include "../VugexySession.h"
#include "../ThreadSafeLogger.h"

class TcpProtokoll;
class VugexySession;

class TcpListenner {
public:
	TcpListenner(int globalMTU, std::string globalOutgoingDevice);
	virtual ~TcpListenner();
	int start();
	void stop();
	int cleaning();
	void sendMessage(std::string str);

	std::vector<VugexySession*> sessions;

private:
	bool running;
	int numberOffClients;

	int globalMTU;
	std::string globalOutgoingDevice;
	boost::thread *thread_TcpBinder;
	void createSession(int sock, boost::thread *thread);
	int thread_Binder();

};

#endif /* KOMMUNIKATION_H_ */
