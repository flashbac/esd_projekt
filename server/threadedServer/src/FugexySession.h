/*
 * FugexySession.h
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#ifndef FUGEXYSESSION_H_
#define FUGEXYSESSION_H_


#include "Kommunikation/TcpProtokoll.h"
#include "Stream/Client.h"


class TcpProtokoll;
class TcpConnection;
class Client;

class FugexySession {
public:

	FugexySession(int Sock);
	virtual ~FugexySession();

	//Member

	TcpProtokoll *tcpP;
	Client *client;
	TcpConnection *tcpC;
	int kameraID;
	// Servo *servo;

	//Functions

	void StartClient(std::string ip, int port);
	void SetCamera(int camID);

private:
	void initServo();
	void initCamera();
	void intiTCP();
	void disconnect();
};

#endif /* FUGEXYSESSION_H_ */
