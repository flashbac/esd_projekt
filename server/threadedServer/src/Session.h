/*
 * Session.h
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#ifndef SESSION_H_
#define SESSION_H_

#endif /* SESSION_H_ */
#include "Kommunikation/TcpProtokoll.h"
#include "Stream/Client.h"


class TcpProtokoll;
//class Client;

class Session {
public:

	//Member

	TcpProtokoll *kp;
	Client *client;
	int kameraID;
	// Servo *servo;

	//Functions
	Session(int Sock);
	virtual ~Session();
	void StartClient();

private:
	void initServo();
	void initCamera();
	void intiTCP();
};