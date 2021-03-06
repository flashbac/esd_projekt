/*
 * KommunikationsProtokoll.cpp
 *
 *  Created on: 18.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#include "TcpProtokoll.h"

TcpProtokoll::TcpProtokoll(VugexySession *session) {
	this->tcpConnection = NULL;
	this->session = session;
}

TcpProtokoll::~TcpProtokoll() {

}

void TcpProtokoll::setTcpConnectionClass(TcpConnection* tcpConnection) {
	this->tcpConnection = tcpConnection;

}

void TcpProtokoll::sendMessageToSenderThread(std::string json) {
	std::stringstream ss;
	ss << "[debug]\t[TcpProtokoll Send: "<< json << "\n";
	//ThreadSafeLogger::instance().print(ss.str());
	tcpConnection->sendMessage(json);
}

void TcpProtokoll::cmdExit() {
	Json::Value jo;

	jo["cmd"] = "exit";
	jo["version"] = TCP_Protokoll_Version;
	jo["value"] = 1;

	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));
}

void TcpProtokoll::camAvalible(std::vector<cam_t> cams) {

	//IKamera *ik = IKamera::getInstance();
	Json::Value jo;
	Json::Value ja;

	//std::vector<cam_t> cams = ik->getCams();
	for (unsigned int i = 0; i < cams.size(); i++) {
		Json::Value io;
		io["id"] = i;
		io["name"] = cams[i].name;
		io["use"] = cams[i].use;
		ja.append(io);
	}
	jo["version"] = TCP_Protokoll_Version;
	jo["status"] = "cams";
	jo["value"] = ja;

	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));

}
void TcpProtokoll::statusCamera(int currentCam) {
	Json::Value jo;

	jo["version"] = TCP_Protokoll_Version;
	jo["status"] = "camera";
	jo["value"] = currentCam;
	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));

}
void TcpProtokoll::statusUDP(std::string ip, int port) {

	Json::Value jo;
	Json::Value io;
	jo["version"] = TCP_Protokoll_Version;
	jo["status"] = "udp";
	io["des"] = ip.c_str();
	io["port"] = port;
	jo["value"] = io;

	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));
}
void TcpProtokoll::statusServos(int x, int y) {
	Json::Value jo;
	Json::Value io;
	jo["version"] = TCP_Protokoll_Version;
	jo["status"] = "position";
	io["x"] = x;
	io["y"] = y;
	jo["value"] = io;

	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));

}
void TcpProtokoll::statusFace(std::vector<face_t> faces) {
	Json::Value jo;
	Json::Value array;
	Json::Value io;
	jo["status"] = "face";
	for (unsigned int i = 0; i < faces.size(); i++) {
		jo["version"] = TCP_Protokoll_Version;
		io["id"] = faces[i].face_id;
		io["name"] = faces[i].name;
		io["x"] = faces[i].x;
		io["y"] = faces[i].y;
		io["height"] = faces[i].height;
		io["width"] = faces[i].width;
		array.append(io);
	}
	jo["value"] = array;

	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));
}
void TcpProtokoll::statusTrack(int face_id) {
	Json::Value jo;

	jo["version"] = TCP_Protokoll_Version;
	jo["status"] = "track";
	jo["value"] = face_id;

	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));
}

void TcpProtokoll::statusMTU(int mtu) {
	Json::Value jo;

	jo["version"] = TCP_Protokoll_Version;
	jo["status"] = "mtu";
	jo["value"] = mtu;

	Json::FastWriter fastWriter;
	sendMessageToSenderThread(fastWriter.write(jo));
}

void TcpProtokoll::commandoProzess(std::string json) {
	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(json, root);
	if (!parsingSuccessful) {
		// report to the user the failure and their locations in the document.
		std::stringstream ss;
		ss << "Failed to parse configuration\n"
				<< reader.getFormatedErrorMessages();
		ThreadSafeLogger::instance().print(ss.str());
		return;
	}
	std::string cmd = root.get("cmd", "").asString();

	//printf("%s", cmd.c_str());

	if (cmd == "position") {
		Json::Value value;
		value = root.get("value", "");
		std::string derection = value.get("direction", "").asString();
		int steps = value.get("steps", "").asInt();
		if (derection == "left") {
			SerialWrapper::instance().sendDelta(AvailabeServoGroups, steps, 0);
			return;
		}
		if (derection == "right") {
			SerialWrapper::instance().sendDelta(AvailabeServoGroups, -steps, 0);
			return;
		}
		if (derection == "top") {
			SerialWrapper::instance().sendDelta(AvailabeServoGroups, 0, steps);
			return;
		}
		if (derection == "bottom") {
			SerialWrapper::instance().sendDelta(AvailabeServoGroups, 0, -steps);
			return;
		}
	}

	if (cmd == "face") {
		int value = root.get("value", "").asInt();
		session->SetFaceToDetect(value);
		return;
	}

	if (cmd == "mode") {
		std::string value = root.get("value", "").asString();
		if (value == "auto") {
			session->setControlModeToAutomatik();
		}
		if (value == "manu") {
			session->setControlModeToManuell();
		}
		return;
	}

	if (cmd == "camera") {
		int value = root.get("value", "").asInt();
		session->SetCamera(value);
		return;
	}

	if (cmd == "udp") {
		Json::Value value;
		value = root.get("value", "");
		std::string des = value.get("des", "").asString();
		int port = value.get("port", "").asInt();
		session->StartClient(des, port);
		return;
	}

	if (cmd == "exit") {
		int value = root.get("value", "").asInt();
		if (value == 1) {
			//disconnect
			session->disconnectedClient();
			return;
		}
	}
}

