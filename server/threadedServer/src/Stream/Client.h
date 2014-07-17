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
#include <sys/time.h>
#include "../Kommunikation/KommunikationTypes.h"
#include "../Kommunikation/TcpProtokoll.h"

//defines
#define CLIENT_MAX_BUFFER_PIC_COUNT 20
#define MAX_ABWEICHUNG_RECHTECK 5

//used namespaces
using namespace std;

class TcpProtokoll;
class FugexySession;

class Client {
public:
	Client(FugexySession *session,std::string ipadress, int port, unsigned char camerID, std::string outgoingDeviceName);
	virtual ~Client();
	void setMTUsize(int MTUsize);
	int getMTUsize();
	int init();
	int start();
	void stop();
	void setJpgQuality(int prozent);
	void setVideoSettings(double width,  double heigth, double frameRate);
	std::vector<face_t> getFaceDetectionVectorAsFace_t();

private:
	unsigned char cameraID;
	bool isInit;

	std::vector<unsigned char> ringpuffer[CLIENT_MAX_BUFFER_PIC_COUNT];
	std::vector<unsigned char> copyOfPicForDetection;
	std::vector<cv::Rect> global_faces;
	std::vector<cv::Rect> global_faces_before;
	int jpgQuality;
	double camera_width;
	double camera_heigth;
	double camera_frameRate;

	sem_t sem_freeSpace;
	sem_t sem_numberToWrite;
	sem_t sem_faceDetectionBusy;
	sem_t sem_faceDetectionVector;
	sem_t sem_faceDetectionNewPicAvailable;

	UDPClient *udpClient;
	UDPProtkoll *udpProtokoll;
	Helper *helper;
	OpenCVWarpper openCVforCapture;
	OpenCVWarpper openCVforFaceDetection;
	FugexySession *session;

	boost::thread *thread_cam;
	boost::thread *thread_face;
	boost::thread *thread_UDPsend;

	bool running;
	timeval frameRateMeasureStart, frameRateMeasureEnd;

	bool AbweichungImBereich(cv::Rect a, cv::Rect b, double MaxAbweichung);
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
