/*
 * FileIO.cpp
 *
 *  Created on: 09.05.2014
 *      Author: dennis
 */

#include "FileIO.h"

#include <stdlib.h>
#include <cstdio>

FileIO::FileIO(string filename) {
	// TODO Auto-generated constructor stub
	file_loaded = false;
	file.open(filename.c_str(), ios::binary | ios::in);
	if (!file.is_open())
		this->~FileIO();

	// filesize
	int begin = file.tellg();
	file.seekg(0, ios::end);
	int end = file.tellg();
	size = end - begin;
	//printf("filesize: %d\n", (int) size);
	// psoition wieder auf anfang
	file.seekg(0, ios::beg);
	buffer = (unsigned char*) malloc(sizeof(unsigned char) * size);
	file.read((char*) buffer, size);
	file.close();
	file_loaded = true;
}

FileIO::~FileIO() {
	// TODO Auto-generated destructor stub
	if (file.is_open()) {
		file.close();
	}
	size = 0;
	if (buffer != NULL) {
		free(buffer);
	}
}

unsigned char* FileIO::getBuffer() {
	return buffer;
}

size_t FileIO::getBufferSize() {
	return size;
}

bool FileIO::isFileLoaded() {
	return file_loaded;
}
