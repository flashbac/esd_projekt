/*
 * Client.h
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#ifndef CLIENT_H_
#define CLIENT_H_

// includes

#include <boost/thread.hpp>
#include <semaphore.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "UDPClient.h"
#include "UDPProtkoll.h"
#include "Helper.h"

//defines
#define CLIENT_MAX_BUFFER_PIC_COUNT 20

//used namespaces
using namespace std;

class Client {
public:
	Client(std::string ipadress, int port, std::string outgoingDeviceName);
	virtual ~Client();
	void thread_safe_print(std::string str);
	void setMTUsize(int MTUsize);
	int getMTUsize();
	int init();
	int start();
	void stop();

private:
	std::vector<unsigned char> ringpuffer[CLIENT_MAX_BUFFER_PIC_COUNT];
	std::vector<unsigned char> global_faces;
	sem_t sem_freeSpace;
	sem_t sem_numberToWrite;
	sem_t sem_faceDetectionBusy;
	sem_t sem_faceDetectionVector;
	sem_t sem_print;

	UDPClient *udpClient;
	UDPProtkoll *udpProtokoll;
	Helper *helper;

	boost::thread *thread_cam;
	boost::thread *thread_face;
	boost::thread *thread_UDPsend;

	bool running;

	bool isFaceDetectionReady();
	void setFaceDetectionVector(std::vector<unsigned char> faces);
	std::vector<unsigned char> getFaceDetectionVector();
	std::vector<unsigned char> * getNextFreeToWriteImage();
	std::vector<unsigned char> * getNextToReadToSend();
	void wait(int seconds);
	void paintRectFromFaces();
	void thread_kamera_reader();
	void thread_face_detection();
	void thread_send_pic();
};

#endif /* CLIENT_H_ */
