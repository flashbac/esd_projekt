/*
 * IKamera.cpp
 *
 *  Created on: 02.06.2014
 *      Author: rensky
 */

#include "IKamera.h"

IKamera::IKamera(void) {
	// TODO Auto-generated constructor stub

}

IKamera::~IKamera(void) {
	// TODO Auto-generated destructor stub
}

IKamera* IKamera::theInstance = NULL;

IKamera* IKamera::getInstance()
{
	if( theInstance == NULL )
	{
	   theInstance = new IKamera();
	   cam_t cam;
	   cam.id = 0;
	   cam.name = "Kamera 0";
	   cam.use = false;
	   cam_t cam2;
	   cam2.id = 1;
	   cam2.name = "Kamera 1";
	   cam2.use = false;

	   theInstance->cams.push_back(cam);
	   theInstance->cams.push_back(cam2);
	}
	return theInstance;
}

void IKamera::release()
{

}

std::vector<cam_t> IKamera::getCams()
{
	return theInstance->cams;
}

int IKamera::useCam(int id)
{
	if (id >= 0 && id < theInstance->cams.size())
	{
		if (theInstance->cams[id].use == false)
		{
			theInstance->cams[id].use = true;
			return 0;
		}
		else
			return -1;
	}
	else
		return -2;
}

int IKamera::unUseCam(int id){

	if (id >= 0 && id < theInstance->cams.size())
	{
		if (theInstance->cams[id].use == true)
		{
			theInstance->cams[id].use = false;
			return 0;
		}
		else
			return -1;
	}
	else
		return -2;
}

