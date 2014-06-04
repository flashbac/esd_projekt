/*
 * FugexySession.cpp
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#include "FugexySession.h"


FugexySession::FugexySession(int Sock, int MTU, std::string outgoingDevice)
{
	iKamera = IKamera::getInstance();
	tcpP = new TcpProtokoll(this);
	tcpC = new TcpConnection(Sock, tcpP);
	tcpP->setTcpConnectionClass(tcpC);

	tcpP->camAvalible();
	//client = NULL;
	theClient = NULL;
	kameraID = 0;

	this->MTU = MTU;
	this->outgoingDevice = outgoingDevice;

}

FugexySession::~FugexySession()
{
	this->theClient->stop();
	delete this->theClient;
}

void FugexySession::disconnect(){

	if (this->theClient != NULL) {
			delete this->theClient;
		}
		if (this->tcpC != NULL) {
				delete this->tcpC;
			}
		if (this->tcpP != NULL) {
				delete this->tcpP;
		}
	// mich selbst entfernen
	delete this;
}

void FugexySession::SetCamera(int camID)
{
	// Kamera wird freigegegebn 		  || Kamera ist noch nicht gesetzt
	if (iKamera->unUseCam(kameraID) == 0 )
	{
		//delete client;
		this->theClient->stop();
		delete this->theClient;
		if (iKamera->useCam(camID) == 0)
		{
			kameraID = camID;
		}
	}
}

void FugexySession::StartClient(std::string ip, int port)
{
	//std::string device = "eth0";
	//int MTU = 1500;
	cam_t tmpCam = iKamera->getCams().at(kameraID);

	theClient = new Client(ip, port, tmpCam.systemID, this->outgoingDevice);

	sem_t sem_print;
	if (sem_init(&sem_print, 0, 1) < 0) {
		std::cout << "Error: init sem_print";
	}
	theClient->setSafePrintSemaphore(&sem_print);

	//setup Logitech c270 -> 640x360 @ 25 fps
	theClient->setVideoSettings(tmpCam.camWidth, tmpCam.camHeigth, tmpCam.camFrameRate);
	//a.setVideoSettings(640, 480, 25);
	theClient->init();
	theClient->setJpgQuality(20);
	theClient->setMTUsize(this->MTU);
	theClient->start();

}
