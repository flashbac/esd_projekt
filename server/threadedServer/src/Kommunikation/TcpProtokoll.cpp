/*
 * KommunikationsProtokoll.cpp
 *
 *  Created on: 18.05.2014
 *      Author: rensky
 */

#include "TcpProtokoll.h"


TcpProtokoll::TcpProtokoll(FugexySession *session)
{
	this->tcpConnection = NULL;
	this->session = session;
}

TcpProtokoll::~TcpProtokoll() {

}

void TcpProtokoll::setTcpConnectionClass(TcpConnection* tcpConnection){
	this->tcpConnection = tcpConnection;

}

void TcpProtokoll::sendMessageToSenderThread(std::string json){
	tcpConnection->sendMessage(json);
}

void TcpProtokoll::cmdExit(){
	Json::Value jo;

	jo["cmd"] = "exit";
	jo["value"] = 1;

	sendMessageToSenderThread(jo.asCString());
}

void TcpProtokoll::camAvalible(){

	IKamera *ik = IKamera::getInstance();
	Json::Value jo;
	Json::Value ja;

	std::vector<cam_t> cams = ik->getCams();
	for (unsigned int i = 0; i<cams.size();i++)
	{
		Json::Value io;
		io["id"] = cams[i].id;
		io["name"] = cams[i].name;
		io["use"] = cams[i].use;
		ja.append(io);
    }
	jo["status"] = "cams";
	jo["value"] = ja;

	Json::FastWriter fastWriter;
	std::string s = fastWriter.write(jo);

	printf("%s",s.c_str());
	sendMessageToSenderThread(s);

}
void TcpProtokoll::statusCamera(int currentCam){
	Json::Value jo;

	jo["status"] = "camera";
	jo["value"] = currentCam;
	sendMessageToSenderThread(jo.asCString());
}
void TcpProtokoll::statusUDP(std::string ip, int port){

	Json::Value jo;
	Json::Value io;
	jo["status"] = "udp";
	io["des"] = ip.c_str();
	io["port"] = port;
	jo["value"] = io;

	sendMessageToSenderThread(jo.asCString());
}
void TcpProtokoll::statusServos(int x, int y){
	Json::Value jo;
	Json::Value io;
	jo["status"] = "position";
	io["x"] = x;
	io["y"] = y;
	jo["value"] = io;

	sendMessageToSenderThread(jo.asCString());

}
void TcpProtokoll::statusFace(std::vector<face_t> faces){
	Json::Value jo;
	Json::Value array;
	Json::Value io;
	jo["status"] = "face";
	for (unsigned int i = 0; i<faces.size();i++)
	{
		io["id"] = faces[i].face_id;
		io["name"] = faces[i].name;
		io["x"] = faces[i].x;
		io["y"] = faces[i].y;
		io["height"] = faces[i].height;
		io["width"] = faces[i].width;
		array.append(io);
	}
	jo["value"] = array;
	sendMessageToSenderThread(jo.asCString());
}
void TcpProtokoll::statusTrack(int face_id){
	Json::Value jo;

	jo["status"] = "track";
	jo["value"] = face_id;
	sendMessageToSenderThread(jo.asCString());
}

void TcpProtokoll::commandoProzess(std::string json){
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

	printf("%s", cmd.c_str());

	if (cmd == "position" ){
		Json::Value value;
		value = root.get("value","");
		std::string derection = value.get("direction","").asString();
		int steps = value.get("steps","").asInt();
		if (derection == "left")
		{

			return;
		}
		if (derection == "right")
		{

			return;
		}
		if (derection == "top")
		{

			return;
		}
		if (derection == "bottom")
		{

			return;
		}
	}

	if (cmd == "mode" ){
		std::string value = root.get("value","").asString();
		sendMessageToSenderThread("{\"angekommen\":\"ja\"}");
		return;
	}

	if (cmd == "camera" ){
		int value = root.get("value","").asInt();
		session->kameraID = value;
		return;
	}

	if (cmd == "udp" ){
		Json::Value value;
		value = root.get("value","");
		std::string des = value.get("des","").asString();
		int port = value.get("port","").asInt();
		session->StartClient(des,port);
		return;
	}

	if (cmd == "exit" ){
		int value = root.get("value","").asInt();
		if (value == 1)
		{
			//disconnect
			cmdExit();
			return;
		}
	}
}



