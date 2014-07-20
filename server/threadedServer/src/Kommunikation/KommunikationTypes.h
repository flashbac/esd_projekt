/*
 * KommunikationTypes.h
 *
 *  Created on: 18.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#include <string>

#ifndef KOMMUNIKATIONSTYPES_H_
#define KOMMUNIKATIONSTYPES_H_

struct face_str;
typedef struct face_str {
	int face_id;
	std::string name;
	int x;
	int y;
	int width;
	int height;
}face_t;

#endif
