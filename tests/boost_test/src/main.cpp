#include <boost/thread.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <iostream>
#include <semaphore.h>
#include <sstream>
#include <string>
#include <vector>
//#include <array>

//using namespace boost::interprocess;
using namespace std;

// Defines
#define MAX_BUFFER_PIC_COUNT 20
#define MAX_PIC_SIZE_IN_kB 95
#define MAX_PIC_SIZE_IN_BYTE MAX_PIC_SIZE_IN_kB*1024
//-------<

//variablen
//char* ringpuffer[MAX_BUFFER_PIC_COUNT][MAX_PIC_SIZE_IN_BYTE];
//std::array<std::vector<unsigned char>, MAX_BUFFER_PIC_COUNT> ringpuffer;
std::vector<unsigned char> ringpuffer[MAX_BUFFER_PIC_COUNT];
std::vector<unsigned char> global_faces;
sem_t sem_freeSpace;
sem_t sem_numberToWrite;

sem_t sem_faceDetectionBusy;
sem_t sem_faceDetectionVector;
sem_t sem_print;
//--------<

void thread_safe_print(string str) {
	sem_wait(&sem_print);
	std::cout << str;
	sem_post(&sem_print);
}

bool isFaceDetectionReady() {
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

void setFaceDetectionVector(std::vector<unsigned char> faces) {
	sem_wait(&sem_faceDetectionVector);
	global_faces = faces;
	sem_post(&sem_faceDetectionVector);
}

std::vector<unsigned char> getFaceDetectionVector() {
	std::vector<unsigned char> faces;
	sem_wait(&sem_faceDetectionVector);
	faces = global_faces;
	sem_post(&sem_faceDetectionVector);

	return faces;
}

std::vector<unsigned char> * getNextFreeToWriteImage() {
// nächst freien Puffer geben
	static unsigned int Rcounter = 0;
	// slot reservieren bzw. auf freien solt warten
	sem_wait(&sem_freeSpace);

	//ringindex inc
	Rcounter++;

	Rcounter = Rcounter % MAX_BUFFER_PIC_COUNT;

	// rueckhgabe des naechsten ringpointers
	return &(ringpuffer[Rcounter]);
}

std::vector<unsigned char> * getNextToReadToSend() {
// naechsten zu schreibenen puffer geben
	static unsigned int Rcounter = 0;

	// auf naechste element warten
	sem_wait(&sem_numberToWrite);

	//ringindex inc
	Rcounter++;

	Rcounter = Rcounter % MAX_BUFFER_PIC_COUNT;

	//rueckgabe des naechsten ringpointers
	return &(ringpuffer[Rcounter]);
}

void wait(int seconds) {
	boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

unsigned char tmp[10] = { 'a', 'a','a','a','a','a','a','a','a','a' };

void paintRectFromFaces()
{
}

void thread_kamera_reader() {
	try {
		while (1) {
			//std::vector<unsigned char> *nextPic = NULL;
			//nextPic = getNextFreeToWriteImage();
			std::vector<unsigned char>& nextPic = *getNextFreeToWriteImage();

			paintRectFromFaces();

			nextPic.clear();
			nextPic.resize(10);
			memcpy(&nextPic[0], tmp, 10);

			sem_post(&sem_numberToWrite);
		}
	} catch (boost::thread_interrupted&) {
		thread_safe_print("\nthread_kamera_reader interrupted!");
	}
}

void thread_face_detection() {
	sem_wait(&sem_faceDetectionBusy);

	thread_safe_print("\nGesicht erkannt");
	// gesichtserkennung hier rein
	for (long l; l < 9999999999999; l++)
		;
	sem_post(&sem_faceDetectionBusy);
}

void thread_send_pic() {
	try {
		while (1) {
			//std::vector<unsigned char> *nextPic = NULL;
			// keine kopie, ist noch call by reference
			std::vector<unsigned char>& nextPic = *getNextToReadToSend();
			//nextPic = *getNextToReadToSend();

			//frage ob faceDetection nicht beschäftigt ist?
			if (isFaceDetectionReady()) {
				//memcopy of pic for face detection
			}
			// send pic

			std::stringstream str;

			str << "\nInhalt: ";
			for (int i = 0; i < nextPic.size(); i++) {
				str << nextPic[i];
			}
			thread_safe_print(str.str());

			// free sem
			sem_post(&sem_numberToWrite);
		}
	} catch (boost::thread_interrupted&) {
		thread_safe_print("\nthread_send_pic interrupted!");
	} catch (std::exception &e) {
		thread_safe_print(e.what());
	}
}

int main() {
	cout << "\ntest";
	// Wie oft darf geschreiben werden
	if (sem_init(&sem_freeSpace, 0, MAX_BUFFER_PIC_COUNT) < 0) {
		std::cout << "Error: init sem_freeSpace";
		return 1; // durch define ersetzten
	}
	cout << "\n2";
	if (sem_init(&sem_numberToWrite, 0, 0) < 0) {
		std::cout << "Error: sem_numberToWrite";
	}
	cout << "\n3";
	if (sem_init(&sem_faceDetectionBusy, 0, 1) < 0) {
		std::cout << "Error: init sem_faceDetectionBusy";
		return 1; // durch define ersetzten
	}
	cout << "\n4";
	if (sem_init(&sem_faceDetectionVector, 0, 1) < 0) {
		std::cout << "Error: init sem_faceDetectionVector";
	}
	cout << "\n5";
	if (sem_init(&sem_print, 0, 1) < 0) {
		std::cout << "Error: init sem_print";
	}
	thread_safe_print("\nthread_safe: test");

	boost::thread t1(thread_kamera_reader);
	cout << "\n7";
	boost::thread t2(thread_send_pic);
	cout << "\n8";

	//wait(3);
	//t.interrupt();
	//t.join();

	while (1)
		;
	cout << "\n9";
	return 0;
}
