/*
 * main.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#include "Helper.h"
#include "UDPClient.h"
#include "UDPProtkoll.h"
#include <opencv2/core/core.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/types_c.h>
#include <stddef.h>
#include <vector>

class Helper;

/*
 * main.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#define debug true

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <cstdio>

#include <pthread.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <unistd.h>    //write

#include <cv.h>
#include <highgui.h>

// message queues
#include <sys/ipc.h>;
#include <sys/msg.h>;
#include <sys/types.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame, bool save);

/** Global variables */
String face_cascade_name =
		"/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name =
		"/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

//String face_cascade_name = "/usr/local/Cellar/opencv/2.4.8.2/share/OpenCV/lbpcascades/lbpcascade_frontalface.xml";
//String eyes_cascade_name = "/usr/local/Cellar/opencv/2.4.8.2/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

pthread_t Treader;
pthread_t Twriter;

void *reader(void * arg) {
	CvCapture* capture;
	Mat frame;

	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)) {
		printf("--(!)Error loading\n");
		return NULL;
	};
	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("--(!)Error loading\n");
		return NULL;
	};

	//-- 2. Read the video stream
	capture = cvCaptureFromCAM(-1);
	if (capture) {
		while (true) {
			frame = cvQueryFrame(capture);
			//imwrite( "test.jpg", frame );

			//-- 3. Apply the classifier to the frame
			if (!frame.empty()) {
				detectAndDisplay(frame, false);
			} else {
				printf(" --(!) No captured frame -- Break!");
				break;
			}

			int c = waitKey(10);
			if ((char) c == 'c') {
				break;
			}
		}
	}

}

void *writer(void * arg) {
	/*
	 Mat image;
	 image = imread("test.jpg", 1);

	 //-- 1. Load the cascades
	 if (!face_cascade.load(face_cascade_name)) {
	 printf("--(!)Error loading\n");
	 return NULL;
	 };
	 if (!eyes_cascade.load(eyes_cascade_name)) {
	 printf("--(!)Error loading\n");
	 return NULL;
	 };

	 if (image.data) {
	 while (true) {
	 Mat image2;
	 image.copyTo(image2);
	 image = imread("test.jpg", 1);
	 detectAndDisplay (image2, true);

	 int c = waitKey(10);
	 if ((char) c == 'c') {
	 break;
	 }
	 }
	 }
	 */
	while (1)
		;
}

void start_opencv_threads(void) {
	if (pthread_create(&Treader, NULL, reader, (void*) NULL) == -1) {
		perror("Error: can't create producer Thread\n");
		return; // define
	}
	if (pthread_create(&Twriter, NULL, writer, (void*) NULL) == -1) {
		perror("Error: can't create consumer Thread\n");
		return; //define
	}
}
#define TEST_SIZE 3000
unsigned char testbuffer[TEST_SIZE];

int main(int argc, char** argv) {

	Helper h = Helper();

	UDPClient client = UDPClient("192.168.178.75", 50000);
	UDPProtkoll protokoll = UDPProtkoll(&client, h.getMTUsize());

	memset(testbuffer, 't', TEST_SIZE);
	if (protokoll.sendInChunks(0, testbuffer, TEST_SIZE) != TEST_SIZE)
				printf("\nsenden nicht okay\n");

	if (protokoll.sendInChunks(0, testbuffer, TEST_SIZE) != TEST_SIZE)
					printf("\nsenden nicht okay\n");

	return 0;
}

std::vector<Rect> faces;
/** @function detectAndDisplay */
void detectAndDisplay(Mat frame, bool save) {
	static int counter = 0;
	//std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//convert captured image to gray scale and equalize

	if ((++counter % 5) == 0) {
		faces.clear();
		//find faces and store them in the vector array
		//face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, Size(30,30));
		face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0,
				Size(30, 30));
	}

	//draw a rectangle for all found faces in the vector array on the original image
	for (int i = 0; i < faces.size(); i++) {
		Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
		Point pt2(faces[i].x, faces[i].y);

		rectangle(frame, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2,
				0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++) {
			Point center(faces[i].x + eyes[j].x + eyes[j].width * 0.5,
					faces[i].y + eyes[j].y + eyes[j].height * 0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 1, 8, 0);
		}
		printf("erkannt: %d\n", (int) faces.size());
	}

	//-- Detect faces
	//face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	/*

	 printf("erkannt: %d\n", (int)faces.size());
	 for( size_t i = 0; i < faces.size(); i++ )
	 {
	 Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
	 ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );


	 Mat faceROI = frame_gray( faces[i] );
	 std::vector<Rect> eyes;

	 //-- In each face, detect eyes
	 eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	 for( size_t j = 0; j < eyes.size(); j++ )
	 {
	 Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
	 int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
	 circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
	 }

	 }
	 */
	//-- Show what you got
	if (save)
		imwrite("testoutput.jpg", frame);
	else
		imshow(window_name, frame);
}

