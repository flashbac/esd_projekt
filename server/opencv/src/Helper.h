/*
 * Helper.h
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <string>

class Helper {
public:
	Helper();
	virtual ~Helper();

	int getMTUsize(std::string devicename = "eth0");
};

#endif /* HELPER_H_ */
