/*
 * IKamera.cpp
 *
 *  Created on: 02.06.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#include "IKamera.h"

IKamera::IKamera(void) {
	// TODO Auto-generated constructor stub

}

IKamera::~IKamera(void) {
	// TODO Auto-generated destructor stub
}

IKamera* IKamera::theInstance = NULL;

IKamera* IKamera::getInstance() {
	if (theInstance == NULL) {
		theInstance = new IKamera();
		cam_t cam;
		cam.systemID = 0;
		cam.name = "Rene 0";
		cam.use = false;
		cam.camWidth = 640;
		cam.camHeigth = 480;
		cam.camFrameRate = 20;
		cam_t cam2;
		cam2.systemID = 1;
		cam2.name = "Rene 1";
		cam2.use = false;
		cam2.camWidth = 640;
		cam2.camHeigth = 480;
		cam2.camFrameRate = 20;
		cam_t cam3;
		cam3.systemID = 3;
		cam3.name = "Udoo id3";
		cam3.use = false;
		cam3.camWidth = 640;
		cam3.camHeigth = 360;
		cam3.camFrameRate = 25;
		cam_t cam4;
		cam4.systemID = 0;
		cam4.name = "Dennis 0";
		cam4.use = false;
		cam4.camWidth = 640;
		cam4.camHeigth = 360;
		cam4.camFrameRate = 25;
		cam_t cam5;
		cam5.systemID = 3;
		cam5.name = "Udoo 640x480";
		cam5.use = false;
		cam5.camWidth = 640;
		cam5.camHeigth = 480;
		cam5.camFrameRate = 25;

		cam_t cam6;
		cam6.systemID = 0;
		cam6.name = "jetson 640x360@25";
		cam6.use = false;
		cam6.camWidth = 640;
		cam6.camHeigth = 360;
		cam6.camFrameRate = 25;

		theInstance->cams.push_back(cam);
		theInstance->cams.push_back(cam2);
		theInstance->cams.push_back(cam3);
		theInstance->cams.push_back(cam4);
		theInstance->cams.push_back(cam5);
		theInstance->cams.push_back(cam6);
	}
	return theInstance;
}

void IKamera::release() {

}

int IKamera::getCamInformations(int systemID) {
	for (int i = 0; i < cams.size(); i++) {
		if (cams[i].systemID == systemID)
			return i;
	}
	return -1;
}

std::vector<cam_t> IKamera::getCams() {
	return theInstance->cams;
}

int IKamera::useCam(int id) {
	if (id >= 0 && id < theInstance->cams.size()) {
		if (theInstance->cams[id].use == false) {
			theInstance->cams[id].use = true;
			return 0;
		} else
			return 0;
	} else
		return 0;
}

int IKamera::unUseCam(int id) {

	if (id >= 0 && id < theInstance->cams.size()) {
		if (theInstance->cams[id].use == true) {
			theInstance->cams[id].use = false;
			return 0;
		} else
			return 0;
	} else
		return 0;
}

