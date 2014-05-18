/*
 * Client.h
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#ifndef CLIENT_H_
#define CLIENT_H_

// includes

//#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <semaphore.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


#include "UDPClient.h"
#include "UDPProtkoll.h"
#include "Helper.h"
#include "OpenCVWarpper.h"

//defines
#define CLIENT_MAX_BUFFER_PIC_COUNT 20

//used namespaces
using namespace std;

class Client {
public:
	Client(std::string ipadress, int port, unsigned char kamerID, std::string outgoingDeviceName);
	virtual ~Client();
	void setSafePrintSemaphore(sem_t *sem);
	void thread_safe_print(std::string str);
	void setMTUsize(int MTUsize);
	int getMTUsize();
	int init();
	int start();
	void stop();

private:
	unsigned char kameraID;

	std::vector<unsigned char> ringpuffer[CLIENT_MAX_BUFFER_PIC_COUNT];
	std::vector<unsigned char> copyOfPicForDetection;
	std::vector<cv::Rect> global_faces;

	sem_t sem_freeSpace;
	sem_t sem_numberToWrite;
	sem_t sem_faceDetectionBusy;
	sem_t sem_faceDetectionVector;
	sem_t sem_faceDetectionNewPicAvailable;
	sem_t *sem_print;

	UDPClient *udpClient;
	UDPProtkoll *udpProtokoll;
	Helper *helper;
	OpenCVWarpper openCVforCapture;
	OpenCVWarpper openCVforFaceDetection;

	boost::thread *thread_cam;
	boost::thread *thread_face;
	boost::thread *thread_UDPsend;

	bool running;

	bool isFaceDetectionReady();
	void setFaceDetectionVector(std::vector<cv::Rect> faces);
	std::vector<cv::Rect> getFaceDetectionVector();
	std::vector<unsigned char> * getNextFreeToWriteImage();
	std::vector<unsigned char> * getNextToReadToSend();
	void wait(int seconds);
	void thread_kamera_reader();
	void thread_face_detection();
	void thread_send_pic();
};

#endif /* CLIENT_H_ */
