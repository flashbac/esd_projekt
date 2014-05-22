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


void KommunikationsProtokoll::sendMessageToSenderThread(std::string json){

}

void KommunikationsProtokoll::cmdExit(){
	json::Object obj;
	obj["cmd"] = "exit";
	obj["value"] = 1;
	std::string str = json::Serialize(obj);
	TCP_Kommunikation.sendMessage(str);
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

}

