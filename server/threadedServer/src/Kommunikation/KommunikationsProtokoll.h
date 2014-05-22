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
#include "Kommunikation.h"


typedef struct face_str {
	int face_id;
	std::string name;
	int x;
	int y;
	int width;
	int height;
} face_t;

class KommunikationsProtokoll {
public:

	KommunikationsProtokoll();
	virtual ~KommunikationsProtokoll();



	void commandoProzess(std::string json);
	//void setTcpSenderClass(const Kommunikation k);
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

};

#endif /* KOMMUNIKATIONSPROTOKOLL_H_ */
