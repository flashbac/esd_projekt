/*
 * FugexySession.cpp
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#include "FugexySession.h"


FugexySession::FugexySession(int Sock)
{
	iKamera = IKamera::getInstance();
	tcpP = new TcpProtokoll(this);
	tcpC = new TcpConnection(Sock, tcpP);
	tcpP->setTcpConnectionClass(tcpC);

	tcpP->camAvalible();
	client = NULL;
	kameraID = 0;

}

FugexySession::~FugexySession()
{

}

void FugexySession::disconnect(){

	if (this->client != NULL) {
			delete this->client;
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
	//if (getCameraFromSingelton(camID))
	{
		kameraID = camID;
	}
	//else
	{
		//tcpP->
	}
}

void FugexySession::StartClient(std::string ip, int port)
{
	std::string device = "eth0";
	int MTU = 1500;
	double cameraWidth = 640.0;
	double cameraHeigth = 480.0;
	double cameraFrameRate = 25.0;


	Client a(ip, port, kameraID, device);
	sem_t sem_print;
	if (sem_init(&sem_print, 0, 1) < 0) {
		std::cout << "Error: init sem_print";
	}
	a.setSafePrintSemaphore(&sem_print);

	//setup Logitech c270 -> 640x360 @ 25 fps
	a.setVideoSettings(cameraWidth, cameraHeigth, cameraFrameRate);
	//a.setVideoSettings(640, 480, 25);
	a.init();
	a.setJpgQuality(20);
	a.setMTUsize(1500);
	a.start();

}
