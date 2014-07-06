/*
 * main.cpp
 *
 *  Created on: 18.05.2014
 *      Author: dennis
 */

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <unistd.h>

#include "OpenCVWarpper.h"

std::vector<cv::Rect> faces;

timeval MeasureStart, MeasureEnd;

int main(int argc, char** argv) {

	OpenCVWarpper ocv;
	long timeinterval = 0;
	long sec = 0;
	long average = 0;
	long sec_average = 0;

	std::ifstream testFile("./aufnahme.jpg", std::ios::binary);
	std::vector<unsigned char> fileContents(
			(std::istreambuf_iterator<char>(testFile)),
			std::istreambuf_iterator<char>());
	std::cout << "filesize: " << fileContents.size() << "\n";
	ocv.loadFromJPEG(&fileContents);

	if (ocv.addCascade(
			//"./lbpcascade_frontalface.xml")
	"./haarcascade_frontalface_alt.xml")
			!= 0) {
		std::cout << "fehler beim laden der xml\n";
	}

	faces = ocv.detect(ocv.getCascades()[0]);

	for (int i = 0; i < 10; i++) {

		gettimeofday(&(MeasureStart), 0);
		faces = ocv.detect(ocv.getCascades()[0]);
		gettimeofday(&(MeasureEnd), 0);
		sec = MeasureEnd.tv_sec - MeasureStart.tv_sec;
		if (MeasureEnd.tv_usec < MeasureStart.tv_usec) {
			MeasureEnd.tv_usec = MeasureEnd.tv_usec + 1000000;
			sec--;
		}
		timeinterval = MeasureEnd.tv_usec - MeasureStart.tv_usec;
		timeinterval = timeinterval / 1000;
		average += timeinterval;



		printf("Durchlauf %d: %ld sec und %ld microsec \n",i, sec, timeinterval );
		sec_average += sec;

	}

	average /= 10;
	sec_average /= 10;

	std::cout << "Durchschnittliche Zeit: " << sec_average << "s und " << average <<"ms\n";

	return 0;
}
