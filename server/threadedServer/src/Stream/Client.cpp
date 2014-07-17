/*
 * Client.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include "Client.h"

Client::Client(FugexySession *session, std::string ipadress, int port,
		unsigned char kamerID, std::string outgoingDeviceName) {
	this->session = session;
	this->running = false;
	this->isInit = false;
	this->thread_cam = NULL;
	this->thread_face = NULL;
	this->thread_UDPsend = NULL;
	this->cameraID = kamerID;
	this->jpgQuality = 100;
	//default: most cameras support this mode 320x240@25
	this->camera_width = 320;
	this->camera_heigth = 240;
	this->camera_frameRate = 25;

	if (ipadress.empty() || (port <= 0) || outgoingDeviceName.empty())
		this->~Client();

	this->helper = new Helper();
	if (this->helper == NULL)
		this->~Client();

	this->udpClient = new UDPClient(ipadress, port);
	if (this->udpClient == NULL)
		this->~Client();

	this->udpProtokoll = new UDPProtkoll(this->udpClient,
			helper->getMTUsize(outgoingDeviceName));
	if (this->udpProtokoll == NULL)
		this->~Client();

	if (openCVforFaceDetection.addCascade(
			"/usr/share/opencv/lbpcascades/lbpcascade_frontalface.xml")
	//"/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml")
			!= 0)
		this->~Client();
	openCVforFaceDetection.tryToInitAndUseGPU();
}

Client::~Client() {

	stop();
	if (this->udpClient != NULL) {
		delete this->udpClient;
	}

	if (this->udpProtokoll != NULL) {
		delete this->udpProtokoll;
	}

	if (this->helper != NULL)
		delete this->helper;

}

bool Client::isFaceDetectionReady() {
	int value = -1;

	if (sem_getvalue(&sem_faceDetectionBusy, &value) != 0) {
		return false;
	} else {
		if (value > 0)
			return true;
		else
			return false;
	}
}

bool Client::AbweichungImBereich(cv::Rect a, cv::Rect b, double MaxAbweichung) {
	double xAbweichung = (double) abs((a.x + a.width / 2) - (b.x + b.width / 2))
			* 100.0 / (double) camera_width;
	double yAbweichung = (double) abs(
			(a.y + a.height / 2) - (b.y + b.height / 2)) * 100.0
			/ (double) camera_heigth;

	if (xAbweichung <= MaxAbweichung && yAbweichung <= MaxAbweichung)
		return true;
	else
		return false;
}

void Client::setFaceDetectionVector(std::vector<cv::Rect> faces) {
	sem_wait(&sem_faceDetectionVector);

//	std::vector<cv::Rect> aFaces = global_faces;  // Angezegte Faces
//
//	// Vergleich zwischen neuen faces und den vorher angezeigten
//	for (unsigned int i = 0; i < faces.size(); i++) {
//		for (unsigned int j = 0; j < aFaces.size(); j++) {
//			if (AbweichungImBereich(aFaces[j],faces[i], MAX_ABWEICHUNG_RECHTECK) )
//			{
//				//Face behalten
//			}
//			else
//			{ // Abweichung entstanden mit Before vergleichen
//				for (unsigned int k = 0; k < global_faces_before.size(); k++) {
//					if (AbweichungImBereich(aFaces[j],global_faces_before[k], MAX_ABWEICHUNG_RECHTECK))
//					{
//						// Face berhalten
//					}
//					else
//					{
//						// Abweichungen zu Groß also Face übereinstimmung nicht gefunden
//						// Face löschen
//						aFaces.erase(aFaces.begin()+j);
//					}
//				}
//			}
//
//			// Faces hinzufügen fehlt noch
//
//		}
//	}
//	global_faces_before = faces;
//
//	global_faces = aFaces;

	global_faces = faces; // !!! das muss das auskommentiert werden wenn das oben wieder rein kommentiert wird
	sem_post(&sem_faceDetectionVector);
	session->notifyNewFaces();
}

std::vector<cv::Rect> Client::getFaceDetectionVector() {
	std::vector<cv::Rect> faces;
	sem_wait(&sem_faceDetectionVector);
	faces = global_faces;
	sem_post(&sem_faceDetectionVector);

	return faces;
}

std::vector<face_t> Client::getFaceDetectionVectorAsFace_t() {
	std::vector<cv::Rect> faces;
	vector<face_t> facearray;

	if (this->isInit) {
		sem_wait(&sem_faceDetectionVector);
		faces = global_faces;
		sem_post(&sem_faceDetectionVector);

		for (unsigned int i = 0; i < faces.size(); i++) {
			face_t f; // = malloc(sizeof(face_t));
			f.face_id = i;
			std::string name = "Face" + i;
			f.name = name;
			f.x = faces[i].x;
			f.y = faces[i].y;
			f.height = faces[i].height;
			f.width = faces[i].width;
			facearray.push_back(f);
		}
	}
	return facearray;
}

std::vector<unsigned char> * Client::getNextFreeToWriteImage() {
// nächst freien Puffer geben
	static unsigned int Rcounter = 0;
	// slot reservieren bzw. auf freien solt warten
	sem_wait(&sem_freeSpace);

	//ringindex inc
	Rcounter++;

	Rcounter = Rcounter % CLIENT_MAX_BUFFER_PIC_COUNT;

	// rueckhgabe des naechsten ringpointers
	return &(ringpuffer[Rcounter]);
}

std::vector<unsigned char> * Client::getNextToReadToSend() {
// naechsten zu schreibenen puffer geben
	static unsigned int Rcounter = 0;

	// auf naechste element warten
	sem_wait(&sem_numberToWrite);

	//ringindex inc
	Rcounter++;

	Rcounter = Rcounter % CLIENT_MAX_BUFFER_PIC_COUNT;

	//rueckgabe des naechsten ringpointers
	return &(ringpuffer[Rcounter]);
}

void Client::wait(int seconds) {
	boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

void Client::thread_kamera_reader() {
	static int lastFrameRate = 0;
	static int lastFrameMS = 0;
	static int pic_counter = 0;

	try {
		while (running) {
			// ist trozdem call by referenz!
			std::vector<unsigned char>& nextPic = *getNextFreeToWriteImage();

			//gettimeofday(&(this->frameRateMeasureStart), 0);

			if (!openCVforCapture.queryFrame())
				ThreadSafeLogger::instance().print(
						"[debug]\t[Client] bild query fehlgeschlagen");

			openCVforCapture.MatToJPEG(&nextPic, this->jpgQuality);
			
			pic_counter++;
			pic_counter = pic_counter % 5;
			//frage ob faceDetection nicht beschäftigt ist?
			if (isFaceDetectionReady() && (pic_counter == 0)) {
				// kopie vom bild anlegen, copyOfPicForDetection wird durch sem_faceDetectionNewPicAvailable geschuetzt
				copyOfPicForDetection.resize(nextPic.size());
				memcpy(&copyOfPicForDetection[0], &nextPic[0], nextPic.size());
				sem_post(&sem_faceDetectionNewPicAvailable);
			}

			openCVforCapture.drawRects(this->getFaceDetectionVector());
			
			/*std::stringstream frameText;
			//frameText << "FrameRate: " << lastFrameRate << " Bilder/sec == " << lastFrameMS << "ms/Bild";
			frameText << "FrameRate: " << lastFrameRate << " fps";
			openCVforCapture.drawText(frameText.str());*/

			openCVforCapture.MatToJPEG(&nextPic, this->jpgQuality);

			/*gettimeofday(&(this->frameRateMeasureEnd), 0);

			if (this->frameRateMeasureEnd.tv_usec
					< this->frameRateMeasureStart.tv_usec) {
				this->frameRateMeasureEnd.tv_usec =
						this->frameRateMeasureEnd.tv_usec + 1000000;
			}
			lastFrameMS = this->frameRateMeasureEnd.tv_usec
					- this->frameRateMeasureStart.tv_usec;
			lastFrameMS = lastFrameMS / 1000;
			lastFrameRate = 1000 / lastFrameMS;
			*/
			/*std::stringstream str;
			 str << "\nBild Size: " << nextPic.size() << "bytes zeit: " << lastFrameMS << "ms bilder/sec: " << lastFrameRate;
			 this->thread_safe_print(str.str());*/
			sem_post(&sem_numberToWrite);
			boost::this_thread::interruption_point();
		}
	} catch (boost::thread_interrupted&) {
		ThreadSafeLogger::instance().print(
				"[debug]\t[Client] thread_kamera_reader interrupted!");
	}
}

void Client::thread_face_detection() {
	try {
		while (running) {
			boost::this_thread::interruption_point();
			sem_wait(&sem_faceDetectionNewPicAvailable); // TODO Hier wird der Thread geblockt und kann deshalb nicht ordnetlich beendet werden.
			/*while (sem_trywait(&sem_faceDetectionNewPicAvailable) != 0) {
			 boost::this_thread::interruption_point();
			 }*/
			boost::this_thread::interruption_point();
			sem_wait(&sem_faceDetectionBusy);
			boost::this_thread::interruption_point();

			this->openCVforFaceDetection.loadFromJPEG(&copyOfPicForDetection);
			this->setFaceDetectionVector(
					openCVforFaceDetection.detect(
							openCVforFaceDetection.getCascades()[0]));

			sem_post(&sem_faceDetectionBusy);
			boost::this_thread::interruption_point();
		}
		//ThreadSafeLogger::instance().print("[debug]\t[Client] Face detection wurde beeendet!");
	} catch (boost::thread_interrupted&) {
		ThreadSafeLogger::instance().print(
				"[debug]\t[Client] Face detection interruppted!");
	}
}

void Client::thread_send_pic() {
	try {
		while (running) {
			// keine kopie, ist noch call by reference
			std::vector<unsigned char>& nextPic = *getNextToReadToSend();

			// send pic
			this->udpProtokoll->sendInChunks(this->cameraID, &nextPic[0],
					nextPic.size());

			sem_post(&sem_freeSpace);
			boost::this_thread::interruption_point();
		}
	} catch (boost::thread_interrupted&) {
		ThreadSafeLogger::instance().print(
				"[debug]\t[Client] thread_send_pic interrupted!");
	} catch (std::exception &e) {
		std::stringstream ss;
		ss << "[debug]\t[Client] " << e.what() << "\n";
		ThreadSafeLogger::instance().print(ss.str());
	}
}

void Client::setMTUsize(int MTUsize) {
	this->udpProtokoll->setMTUsize(MTUsize);
}

int Client::getMTUsize() {
	return this->udpProtokoll->getMTUsize();
}

int Client::init() {
	// Wie oft darf geschreiben werden
	if (sem_init(&sem_freeSpace, 0, CLIENT_MAX_BUFFER_PIC_COUNT) < 0) {
		std::cout << "Error: init sem_freeSpace";
		return -1; // durch define ersetzten
	}
	if (sem_init(&sem_numberToWrite, 0, 0) < 0) {
		std::cout << "Error: sem_numberToWrite";
	}
	if (sem_init(&sem_faceDetectionBusy, 0, 1) < 0) {
		std::cout << "Error: init sem_faceDetectionBusy";
		return -1; // durch define ersetzten
	}
	if (sem_init(&sem_faceDetectionVector, 0, 1) < 0) {
		return -1;
		std::cout << "Error: init sem_faceDetectionVector";
	}
	if (sem_init(&sem_faceDetectionNewPicAvailable, 0, 0) < 0) {
		return -1;
		std::cout << "Error: init sem_faceDetectionVector";
	}

	openCVforCapture.init(this->cameraID, this->camera_width,
			this->camera_heigth, this->camera_frameRate);

	this->isInit = true;
	return 0;
}

int Client::start() {
	int return_value = 0;

	//boost::thread::attributes attrs;
	//attrs.set_stack_size(1048576*40); //40MByte

	if (!running) {
		running = true;
		//boost::function< void > f = boost::bind(&Client::thread_kamera_reader, this)
		//*
		this->thread_cam = new boost::thread(&Client::thread_kamera_reader,
				this);
		/*
		 this->thread_cam = new boost::thread( attrs, boost::bind(&Client::thread_kamera_reader, this) );
		 */
		if (this->thread_cam == NULL)
			return_value = -1;

		this->thread_face = new boost::thread(&Client::thread_face_detection,
				this);
		if (this->thread_face == NULL)
			return_value = -1;

		this->thread_UDPsend = new boost::thread(&Client::thread_send_pic,
				this);
		if (this->thread_UDPsend == NULL)
			return_value = -1;

		if (return_value != 0) {
			this->stop();
		}
	} else
		return_value = -2;
	return return_value;
}

void Client::stop() {
	if (running) {
		running = false;
		this->thread_face->interrupt();
		sem_post(&sem_faceDetectionNewPicAvailable);
		this->thread_UDPsend->interrupt();
		this->thread_cam->interrupt();
		// erstmal die Facedetection abbrechen lassen
		if (this->thread_face != NULL) {
			this->thread_face->join();
//			if (this->thread_face->timed_join(boost::posix_time::seconds(5))){
//				thread_safe_print("\nthread_face sucessfull joind");
//			} else {
//				thread_safe_print("\nthread_face joind Timed out!");
//			}
			delete (this->thread_face);
		}
		if (this->thread_UDPsend != NULL) {
			this->thread_UDPsend->join();
//			if (this->thread_UDPsend->timed_join(boost::posix_time::seconds(5))){
//				thread_safe_print("\nthread_UDPsend sucessfull joind");
//			} else {
//				thread_safe_print("\nthread_UDPsend joind Timed out!");
//			}
			delete (this->thread_UDPsend);
		}
		if (this->thread_cam != NULL) {

			this->thread_cam->join();
//			if (this->thread_cam->timed_join(boost::posix_time::seconds(5))){
//				thread_safe_print("\nthread_cam sucessfull joind");
//			} else {
//				thread_safe_print("\nthread_cam joind Timed out!");
//			}
			delete (this->thread_cam);
		}

		ThreadSafeLogger::instance().print(
				"[debug]\t[Client] all threads interruppted!");
	}
}

void Client::setJpgQuality(int prozent) {
	this->jpgQuality = prozent;
}

void Client::setVideoSettings(double width, double heigth, double frameRate) {
	this->camera_width = width;
	this->camera_heigth = heigth;
	this->camera_frameRate = frameRate;
}
