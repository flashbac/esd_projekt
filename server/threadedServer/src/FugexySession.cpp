/*
 * FugexySession.cpp
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#include "FugexySession.h"


FugexySession::FugexySession(int Sock)
{
	tcpP = new TcpProtokoll(this);

	tcpC = new TcpConnection(Sock, tcpP);
	tcpP->setTcpConnectionClass(tcpC);


	client = NULL;
	kameraID = 0;

}

FugexySession::~FugexySession()
{
}

void FugexySession::StartClient(std::string ip, int port)
{
	// Kamera.get(KameraID);
	Client a(ip,port, 0, "eth0");
}
