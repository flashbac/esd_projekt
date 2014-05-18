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
	int removeCascade(std::string path);
	std::vector<std::string> listCascadesPaths();
	std::vector<CascadeClassifier> getCascades();
	std::vector<Rect> detect(CascadeClassifier cascade);
	std::vector<Rect> detectAll();
	void display(std::string displayName, Mat frame);
	void displayLast(std::string displayName);
	void MatToJPEG(Mat frame);

private:
	CvCapture* captureDevice;
	Mat frame;
	std::vector<CascadeClassifier> loadedCascades;

};

#endif /* OPENCVWARPPER_H_ */
