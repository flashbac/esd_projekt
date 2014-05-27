/*
 * FugexySession.cpp
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#include "FugexySession.h"


FugexySession::FugexySession(int Sock)
{
	kp = new TcpProtokoll(tcpC);

	tcpC = new TcpConnection(Sock, kp, this);
	kp->setTcpConnectionClass(tcpC);


	client = NULL;
	kameraID = 0;

}

FugexySession::FugexySession()
{
	kp = NULL;
	client = NULL;
	kameraID = 0;
}

FugexySession::~FugexySession()
{
}

void FugexySession::StartClient()
{
	Client a("141.64.166.22",50000, 0, "eth0");
}
