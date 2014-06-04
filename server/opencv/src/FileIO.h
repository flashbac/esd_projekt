/*
 * FileIO.h
 *
 *  Created on: 09.05.2014
 *      Author: dennis
 */

#ifndef FILEIO_H_
#define FILEIO_H_

#include <stddef.h>
//#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class FileIO {
public:
	FileIO(string filenname);
	virtual ~FileIO();
	unsigned char* getBuffer();
	size_t getBufferSize();
	bool isFileLoaded();
private:
	ifstream file;
	streampos size;
	unsigned char* buffer;
	bool file_loaded;
	void fillBuffer();
};

#endif /* FILEIO_H_ */
