/*
 * FugexySession.cpp
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#include "FugexySession.h"

FugexySession::FugexySession(int Sock, int MTU, std::string outgoingDevice) {
	iKamera = IKamera::getInstance();
	tcpP = new TcpProtokoll(this);
	tcpC = new TcpConnection(Sock, tcpP);
	tcpP->setTcpConnectionClass(tcpC);

	tcpP->camAvalible();
	//client = NULL;
	theClient = NULL;
	kameraID = -1;
	clientConnected = true;

	this->MTU = MTU;
	this->outgoingDevice = outgoingDevice;

	if (sem_init(&sem_notifyNewFaces, 0, 1) < 0) {
		this->thread_notifyNewFaces = NULL;
	} else {
		this->thread_notifyNewFaces = new boost::thread(
				&FugexySession::thread_notifyNFaces, this);
	}

	//this->controlMode = CONTROL_MODE_MANUELL;
	this->controlMode = CONTROL_MODE_AUTOMATIK;
	sem_init(&sem_setControlMode, 0, 1);
	SerialWrapper::instance().sendPos(AvailabeServoGroups, 90, 90);
}

FugexySession::~FugexySession() {
	if (this->theClient != NULL) {
		theClient->stop();
		delete this->theClient;
	}
	if (this->tcpC != NULL) {
		delete this->tcpC;
	}
	if (this->tcpP != NULL) {
		delete this->tcpP;
	}
	if (this->thread_notifyNewFaces != NULL) {
		this->thread_notifyNewFaces->interrupt();
		this->thread_notifyNewFaces->join();
		delete (this->thread_notifyNewFaces);
	}
}

void FugexySession::disconnectedClient() {
	clientConnected = false;
}
bool FugexySession::isClientConnected() {
	return clientConnected;
}

void FugexySession::disconnectUdp() {
	if (this->theClient != NULL) {
		delete this->theClient;
		std::cout << "\n theClient wurde hard gekilled";
	}
}

void FugexySession::SetCamera(int camID) {

	// Kamera wird freigegegebn 		  || Kamera ist noch nicht gesetzt
	if (iKamera->unUseCam(kameraID) == 0) {
		//delete client;
		if (theClient != NULL) {
			this->theClient->stop();
			delete this->theClient;
		}
		if (iKamera->useCam(camID) == 0) {
			kameraID = camID;
		}
	}
}

void FugexySession::SetFaceToDetect(int faceID) {
	this->faceID = faceID;
}

void FugexySession::StartClient(std::string ip, int port) {
	//std::string device = "eth0";
	//int MTU = 1500;
	cam_t tmpCam = iKamera->getCams().at(kameraID);

	std::stringstream ss;
	ss << "Use Camera: (ID:" << kameraID <<") (SystemID:" << tmpCam.systemID << ")"<< "\n";
	ThreadSafeLogger::instance().print(ss.str());

	theClient = new Client(this, ip, port, tmpCam.systemID,
			this->outgoingDevice);
	sem_t sem_print;
	if (sem_init(&sem_print, 0, 1) < 0) {
		std::cout << "Error: init sem_print";
	}
	theClient->setSafePrintSemaphore(&sem_print);

	//setup Logitech c270 -> 640x360 @ 25 fps
	theClient->setVideoSettings(tmpCam.camWidth, tmpCam.camHeigth,
			tmpCam.camFrameRate);
	//a.setVideoSettings(640, 480, 25);
	theClient->init();
	theClient->setJpgQuality(20);
	theClient->setMTUsize(this->MTU);
	theClient->start();
}

void FugexySession::setControlModeToAutomatik() {
	sem_wait(&sem_setControlMode);
	this->controlMode = CONTROL_MODE_AUTOMATIK;
	sem_post(&sem_setControlMode);
}

void FugexySession::setControlModeToManuell() {
	sem_wait(&sem_setControlMode);
	this->controlMode = CONTROL_MODE_MANUELL;
	sem_post(&sem_setControlMode);
}

int FugexySession::getControlMode() {
	int tmp = -1;
	sem_wait(&sem_setControlMode);
	tmp = this->controlMode;
	sem_post(&sem_setControlMode);

	return tmp;
}

int FugexySession::getBiggestRect(std::vector<face_t> faces) {
	if (faces.size() < 0)
		return -1;
	int tmpID = 0;
	int value = faces[0].width * faces[0].height;

	if (faces.size() < 2)
		return 0; // => ID = 0
	for (int i = 1; i < (int) faces.size(); i++) {
		int tmp = faces[i].width * faces[i].height;
		if (value < tmp) {
			tmpID = i;
			value = tmp;
		}
	}

	return tmpID;
}

void FugexySession::thread_notifyNFaces() {
	while ((this->tcpP == NULL) || (theClient == NULL)) {
		boost::this_thread::interruption_point();
	}

	/*if ( (this->tcpP == NULL) || (theClient == NULL) )
	 return;
	 */
	while (1) {
		while (sem_trywait(&sem_notifyNewFaces) != 0) {
			boost::this_thread::interruption_point();
		}
		std::vector<face_t> faces =
				this->theClient->getFaceDetectionVectorAsFace_t();
		if (faces.size() > 0) {

			this->tcpP->statusFace(faces);

			if (this->getControlMode() == CONTROL_MODE_AUTOMATIK) {
				PositionToAngle::instance().calculateAndSendNewPosition(
						faces[getBiggestRect(faces)],
						iKamera->getCams().at(kameraID).camWidth,
						iKamera->getCams().at(kameraID).camHeigth);
			}
		}
	}
	//sem_post(&sem_notifyNewFaces);
}

void FugexySession::notifyNewFaces() {
	//this->thread_notifyNewFaces->start_thread();
	int value = -1;
	sem_getvalue(&sem_notifyNewFaces, &value);
	if (value == 0)
		sem_post(&sem_notifyNewFaces);
}
