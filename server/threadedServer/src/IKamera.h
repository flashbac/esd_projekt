/*
 * IKamera.h
 *
 *  Created on: 02.06.2014
 *      Author: rensky
 */

#include <cstdio>
#include <vector>
#include "Kommunikation/KommunikationTypes.h"

#ifndef IKAMERA_H_
#define IKAMERA_H_

struct cams_str;

class IKamera {
public:

	static IKamera * getInstance();
	static void release();
	std::vector<cam_t> getCams();
	int useCam(int id);
	int unUseCam(int id);


private:
    static IKamera* _instance;
    IKamera(void);
    virtual ~IKamera(void);

    static IKamera* theInstance;
    std::vector<cam_t> cams;
};

#endif /* IKAMERA_H_ */
