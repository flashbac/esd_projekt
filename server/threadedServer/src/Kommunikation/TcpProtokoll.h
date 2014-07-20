/*
 * KommunikationsProtokoll.h
 *
 *  Created on: 18.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef KOMMUNIKATIONSPROTOKOLL_H_
#define KOMMUNIKATIONSPROTOKOLL_H_

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include "TcpConnection.h"
#include "KommunikationTypes.h"
#include "../IKamera.h"
#include "../ThreadSafeLogger.h"
#include "../SerialWrapper.h"
#include "../settings.h"

#define TCP_Protokoll_Version 1

class TcpConnection;
class VugexySession;
class IKamera;

class TcpProtokoll {

public:

	// Variablen
	TcpConnection *tcpConnection;
	VugexySession *session;

	// Functions

	TcpProtokoll(VugexySession *session);
	virtual ~TcpProtokoll();

	void commandoProzess(std::string json);
	void setTcpConnectionClass(TcpConnection* tcpConnection);
	void init ();

	void cmdExit();
	void camAvalible(std::vector<cam_t> cams );
	void statusCamera(int currentCam);
	void statusUDP(std::string ip, int port);
	void statusServos(int x, int y);
	void statusFace(std::vector<face_t> faces);
	void statusTrack(int face_id);
	void statusMTU(int mtu);

private:

	void sendMessageToSenderThread(std::string json);
	void CloseConnection();

};


#endif /* KOMMUNIKATIONSPROTOKOLL_H_ */
