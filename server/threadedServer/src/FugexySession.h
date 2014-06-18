/*
 * FugexySession.h
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#ifndef FUGEXYSESSION_H_
#define FUGEXYSESSION_H_

#define CONTROL_MODE_MANUELL 0
#define CONTROL_MODE_AUTOMATIK 1

#include "Kommunikation/TcpProtokoll.h"
#include "Stream/Client.h"
#include "IKamera.h"
#include <boost/thread/thread.hpp>
#include <semaphore.h>

#include "./Kommunikation/KommunikationTypes.h"
#include "PositionToAngle.h"
#include "SerialWrapper.h"

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
	int faceID;
	// Servo *servo;

	//Functions
	void disconnectUdp();
	void StartClient(std::string ip, int port);
	void SetCamera(int camID);
	void SetFaceToDetect(int faceID);
	void disconnectedClient();
	bool isClientConnected();
	void notifyNewFaces();
	void setControlModeToAutomatik();
	void setControlModeToManuell();
	int getControlMode();

private:
	void initServo();
	void initCamera();
	void intiTCP();
	int getBiggestRect(std::vector<face_t> faces);

	Client* theClient;
	int MTU;
	std::string outgoingDevice;
	bool clientConnected;

	void thread_notifyNFaces();
	boost::thread *thread_notifyNewFaces;
	sem_t sem_notifyNewFaces;

	int controlMode;
	sem_t sem_setControlMode;

	int kameraID;
};

#endif /* FUGEXYSESSION_H_ */
