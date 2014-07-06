/*
 * main.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <sys/time.h>

#include "OpenCVWarpper.h"

std::vector<cv::Rect> faces;

timeval MeasureStart, MeasureEnd;

int main(int argc, char** argv) {

	OpenCVWarpper ocv;
	int timeinterval = 0;
	int average = 0;

	std::ifstream testFile("./aufnahme.jpg", std::ios::binary);
	std::vector<unsigned char> fileContents(
			(std::istreambuf_iterator<char>(testFile)),
			std::istreambuf_iterator<char>());
	std::cout << "filesize: " << fileContents.size() << "\n";
	ocv.loadFromJPEG(&fileContents);

	if (ocv.addCascade(
			//"/usr/share/opencv/lbpcascades/lbpcascade_frontalface.xml")
	"/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml")
			!= 0) {
		std::cout << "fehler beim laden der xml\n";
	}

	faces = ocv.detect(ocv.getCascades()[0]);

	for (int i = 0; i < 10; i++) {

		gettimeofday(&(MeasureStart), 0);
		faces = ocv.detect(ocv.getCascades()[0]);
		gettimeofday(&(MeasureEnd), 0);

		if (MeasureEnd.tv_usec < MeasureStart.tv_usec) {
			MeasureEnd.tv_usec = MeasureEnd.tv_usec + 1000000;
		}
		timeinterval = MeasureEnd.tv_usec - MeasureStart.tv_usec;
		timeinterval = timeinterval / 1000;
		average += timeinterval;
	}

	average /= 10;
	std::cout << "Durchschnittliche Zeit: " << average << "ms\n";

	return 0;
}
