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
