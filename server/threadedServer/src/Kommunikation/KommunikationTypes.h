#include <string>

#ifndef KOMMUNIKATIONSTYPES_H_
#define KOMMUNIKATIONSTYPES_H_

typedef struct face_str {
	int face_id;
	std::string name;
	int x;
	int y;
	int width;
	int height;
} face_t;

typedef struct cam_str {
	int id;
	bool use;
	std::string name;
} cam_t;

#endif
