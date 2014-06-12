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
#include "IKamera.h"


class TcpProtokoll;
class TcpConnection;
class Client;
class IKamera;

class FugexySession {
public:

	FugexySession(int Sock, int MTU, std::string outgoingDevice);
	virtual ~FugexySession();

	//Member

	TcpProtokoll* tcpP;
	TcpConnection* tcpC;
	IKamera *iKamera;
	int kameraID;
	int faceID;
	// Servo *servo;

	//Functions
	void disconnectUdp();
	void StartClient(std::string ip, int port);
	void SetCamera(int camID);
	void SetFaceToDetect(int faceID);
	void disconnectedClient();
	bool isClientConnected();

private:
	void initServo();
	void initCamera();
	void intiTCP();
	Client* theClient;
	int MTU;
	std::string outgoingDevice;
	bool clientConnected = true;
};

#endif /* FUGEXYSESSION_H_ */
