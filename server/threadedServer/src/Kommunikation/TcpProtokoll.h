/*
 * KommunikationsProtokoll.h
 *
 *  Created on: 18.05.2014
 *      Author: rensky
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

class TcpConnection;
class FugexySession;
class IKamera;

class TcpProtokoll {

public:

	// Variablen
	TcpConnection *tcpConnection;
	FugexySession *session;

	// Functions

	TcpProtokoll(FugexySession *session);
	virtual ~TcpProtokoll();

	void commandoProzess(std::string json);
	void setTcpConnectionClass(TcpConnection* tcpConnection);
	void init ();

	void cmdExit();
	void camAvalible();
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
