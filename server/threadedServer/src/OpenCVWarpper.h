/*
 * OpenCVWarpper.h
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#ifndef OPENCVWARPPER_H_
#define OPENCVWARPPER_H_

#include <cv.h>
#include <highgui.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

class OpenCVWarpper {
public:
	OpenCVWarpper();
	virtual ~OpenCVWarpper();
	int addCascade(std::string path);
	int removeCascade(CascadeClassifier cascade);
	std::vector<CascadeClassifier> getCascades();
	bool queryFrame();
	std::vector<Rect> detect(CascadeClassifier cascade);
	std::vector<Rect> detect(Mat *frame, CascadeClassifier cascade);
	std::vector<std::vector<Rect> > detectAll();
	std::vector<std::vector<Rect> > detectAll(Mat *frame);
	Mat *getMat();
	void drawRects(std::vector<Rect> rects);
	void drawRects(Mat *frame, std::vector<Rect> rects);
	void drawAllRects(std::vector<std::vector<Rect> > rects);
	void drawAllRects(Mat *frame, std::vector<std::vector<Rect> > rects);
	void display(std::string windowName);
	void display(std::string windowName, Mat *frame);
	void loadFromJPEG(std::vector<unsigned char> *buffer);
	void MatToJPEG(std::vector<unsigned char> *buffer, int quality = 100);
	void MatToJPEG(Mat *frame, std::vector<unsigned char> *buffer, int quality = 100);
	void QueryDetectAllToJPEG(std::vector<unsigned char> *buffer);
	void QueryAndDrawToJPEG(std::vector<Rect> detectedObjects,
			std::vector<unsigned char> *buffer);
	void saveToFile(std::string filename);

private:
	CvCapture* captureDevice;
	Mat frame;
	std::vector<CascadeClassifier> loadedCascades;

};

#endif /* OPENCVWARPPER_H_ */