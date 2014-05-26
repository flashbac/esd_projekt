/*
 * KommunikationsProtokoll.h
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#ifndef KOMMUNIKATIONSPROTOKOLL_H_
#define KOMMUNIKATIONSPROTOKOLL_H_

#include "../SuperEasyJson/json.h"
#include <jsoncpp/value.h>
#include <jsoncpp/reader.h>
#include "TcpConnection.h"
#include "KommunikationTypes.h"

class TcpConnection;

class KommunikationsProtokoll {

public:

	KommunikationsProtokoll();
	virtual ~KommunikationsProtokoll();

	void commandoProzess(std::string json);
	void setTcpSenderClass(TcpConnection *k);
	void init ();

	void cmdExit();
	void camAvalible(int anzahlKamera);
	void statusCamera(int currentCam);
	void statusUDP(std::string ip, int port);
	void statusServos(int x, int y);
	void statusFace(std::vector<face_t> faces);
	void statusTrack(int face_id);



private:

	void sendMessageToSenderThread(std::string json);
	TcpConnection *tcpKommunikation;
};


#endif /* KOMMUNIKATIONSPROTOKOLL_H_ */
