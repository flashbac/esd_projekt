/*
 * Helper.h
 *
 *  Created on: 08.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <sys/types.h>
#include <termios.h>
#include <string.h>

class Helper {
public:
	Helper();
	virtual ~Helper();

	int getMTUsize(std::string devicename = "eth0");
	int kbhit(void);
};

#endif /* HELPER_H_ */
