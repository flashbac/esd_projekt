/*
 * Session.h
 *
 *  Created on: 26.05.2014
 *      Author: rensky
 */

#ifndef SESSION_H_
#define SESSION_H_

#endif /* SESSION_H_ */
#include "Kommunikation/KommunikationsProtokoll.h"
#include "Stream/Client.h"

class KommunikationsProtokoll;
class Client;

class Session {
public:

	//Member

	KommunikationsProtokoll *kp;
	Client *client;
	int kameraID;
	// Servo *servo;

	//Functions
	Session(int Sock, int KameraID);
	virtual ~Session();
	void StartClient();

private:
	void initServo();
	void initCamera();
	void intiTCP();
};
