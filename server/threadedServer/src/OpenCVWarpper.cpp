/*
 * OpenCVWarpper.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include "OpenCVWarpper.h"

OpenCVWarpper::OpenCVWarpper() {
	// TODO Auto-generated constructor stub
	captureDevice = cvCaptureFromCAM(-1);
}

OpenCVWarpper::~OpenCVWarpper() {
	// TODO Auto-generated destructor stub
}

int OpenCVWarpper::addCascade(std::string path) {
	CascadeClassifier cascade;

	if (!cascade.load(path)) {
		return -1;
	} else {
		loadedCascades.push_back(cascade);
		return 0;
	}
}

int OpenCVWarpper::removeCascade(CascadeClassifier cascade) {
	/*
	 loadedCascades.erase(
	 std::remove(loadedCascades.begin(), loadedCascades.end(), cascade),
	 loadedCascades.end());
	 */
	return -1;
}

std::vector<CascadeClassifier> OpenCVWarpper::getCascades() {
	return loadedCascades;
}

bool OpenCVWarpper::queryFrame() {
	frame = cvQueryFrame(captureDevice);
	return !frame.empty();
}

std::vector<Rect> OpenCVWarpper::detect(CascadeClassifier cascade) {
	if (!frame.empty())
		return this->detect(&frame, cascade);
	else {
		std::vector<Rect> tmpVec;
		tmpVec.clear();
		return tmpVec;
	}
}

std::vector<Rect> OpenCVWarpper::detect(Mat *frame, CascadeClassifier cascade) {
	Mat frame_gray;
	std::vector<Rect> detectedRects;

	cvtColor(*frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	detectedRects.clear();
	//find faces and store them in the vector array
	//face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, Size(30,30));
	cascade.detectMultiScale(frame_gray, detectedRects, 1.1, 2, 0,
			Size(30, 30));

	return detectedRects;
}

std::vector<std::vector<Rect> > OpenCVWarpper::detectAll() {
	if (!frame.empty())
		return this->detectAll(&frame);
	else {
		std::vector<std::vector<Rect> > tmpVec;
		tmpVec.clear();
		return tmpVec;
	}
}

std::vector<std::vector<Rect> > OpenCVWarpper::detectAll(Mat *frame) {
	std::vector<std::vector<Rect> > return_vector;
	std::vector<CascadeClassifier> cascades = getCascades();

	return_vector.clear();

	for (int i = 0; i < cascades.size(); i++) {
		return_vector.push_back(this->detect(frame, cascades[i]));
	}

	return return_vector;
}

Mat* OpenCVWarpper::getMat() {
	return &(this->frame);
}

void OpenCVWarpper::drawRects(std::vector<Rect> rects) {
	this->drawRects(&this->frame, rects);
}

void OpenCVWarpper::drawRects(Mat *frame, std::vector<Rect> rects) {
	if (frame->empty() || (rects.size() <= 0))
		return;

	for (int i = 0; i < rects.size(); i++) {
		Point pt1(rects[i].x + rects[i].width, rects[i].y + rects[i].height);
		Point pt2(rects[i].x, rects[i].y);

		rectangle(*frame, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
	}
}

void OpenCVWarpper::drawAllRects(std::vector<std::vector<Rect> > rects) {
	this->drawAllRects(&frame, rects);
}

void OpenCVWarpper::drawAllRects(Mat *frame,
		std::vector<std::vector<Rect> > rects) {
	if (frame->empty() || (rects.size() <= 0))
		return;

	for (int i = 0; i < rects.size(); i++) {
		this->drawRects(frame, rects[i]);
	}
}

void OpenCVWarpper::display(std::string windowName) {
	this->display(windowName, &frame);
}

void OpenCVWarpper::display(std::string windowName, Mat *frame) {
	if (frame->empty())
		return;
	imshow(windowName, *frame);
}

void OpenCVWarpper::loadFromJPEG(std::vector<unsigned char> *buffer) {
	// decode jpg (or other image from a pointer)
	// imageBuf contains the jpg image
	//cv::Mat imgbuf = cv::Mat(640, 480, CV_8U, &buffer[0]);
	cv::Mat imgbuf = cv::Mat(*buffer);
	//cv::Mat imgMat = cv::imdecode(imgbuf, CV_LOAD_IMAGE_COLOR);
	frame = cv::imdecode(imgbuf, CV_LOAD_IMAGE_COLOR);
	// imgMat is the decoded image
}

void OpenCVWarpper::MatToJPEG(std::vector<unsigned char> *buffer, int quality) {
	this->MatToJPEG(&frame, buffer, quality);
}

void OpenCVWarpper::MatToJPEG(Mat *frame, std::vector<unsigned char> *buffer, int quality) {
	if (frame->empty()) {
		buffer->resize(0);
		buffer->clear();
		return;
	}

	// encode image into jpg
	//cv::vector<uchar> buf;
	vector<int> params = vector<int>(2);
	params[0] = CV_IMWRITE_JPEG_QUALITY;
	params[1] = quality;

	cv::imencode(".jpg", *frame, *buffer, params);
	// encoded image is now in buf (a vector)
	//imageBuf = (unsigned char *) realloc(imageBuf, buf.size());
	//memcpy(imageBuf, &buf[0], buf.size());
	//  size of imageBuf is buf.size();
}

void OpenCVWarpper::QueryDetectAllToJPEG(std::vector<unsigned char> *buffer) {
	buffer->resize(0);
	buffer->clear();

	if (!this->queryFrame())
		return;

	this->drawAllRects(this->detectAll());

	this->MatToJPEG(buffer);
}

void OpenCVWarpper::QueryAndDrawToJPEG(std::vector<Rect> detectedObjects,
		std::vector<unsigned char> *buffer) {
	buffer->resize(0);
	buffer->clear();

	if (!this->queryFrame())
		return;

	this->drawRects(detectedObjects);

	this->MatToJPEG(buffer);
}

void OpenCVWarpper::saveToFile(std::string filename) {
	imwrite(filename, frame);
}