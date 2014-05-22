/*
 * KommunikationsProtokoll.cpp
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#include "KommunikationsProtokoll.h"

KommunikationsProtokoll::KommunikationsProtokoll() {
	// TODO Auto-generated constructor stub

}

KommunikationsProtokoll::~KommunikationsProtokoll() {
	// TODO Auto-generated destructor stub
}

//void KommunikationsProtokoll::setTcpSenderClass(const Kommunikation k){
//	tcpKommunikation = k;
//}

void KommunikationsProtokoll::sendMessageToSenderThread(std::string json){

}

void KommunikationsProtokoll::cmdExit(){
	Json::Value jo;

	jo["cmd"] = "exit";
	jo["value"] = 1;

	//tcpKommunikation->sendMessage(jo.asCString());
}

void KommunikationsProtokoll::camAvalible(int anzahlKamera){

}
void KommunikationsProtokoll::statusCamera(int currentCam){

}
void KommunikationsProtokoll::statusUDP(std::string ip, int port){

}
void KommunikationsProtokoll::statusServos(int x, int y){

}
void KommunikationsProtokoll::statusFace(std::vector<face_t> faces){

}
void KommunikationsProtokoll::statusTrack(int face_id){

}

void KommunikationsProtokoll::commandoProzess(std::string json){
	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(json, root);
	if ( !parsingSuccessful )
	{
	    // report to the user the failure and their locations in the document.
	    std::cout  << "Failed to parse configuration\n"
	               << reader.getFormatedErrorMessages();
	    return;
	}
	std::string cmd = root.get("cmd","").asString();

/*	switch (std::string(cmd))
	{
		case "exit":
			int value = root.get("value","").Int;
			if (value == 1)
			{
				//disconnect
				cmdExit();

			}
	} */
}

