/*
 * UDPProtkoll.cpp
 *
 *  Created on: 08.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#include "UDPProtkoll.h"
#include <iostream>

UDPProtkoll::UDPProtkoll(UDPClient *client, int MTUsize, int andriodTimeout,
		int andriodMaxBytesToTimeout) {
	if (client == NULL)
		this->~UDPProtkoll();
	this->client = client;
	this->maxPackageSize = MTUsize - UDP_DATAGRAMM_LENGTH - UDP_HEADER_LENGTH;
	this->bild_id = 0;
	this->mtu = MTUsize;
	this->andriodTimeout = andriodTimeout;
	this->andriodMaxBytesToTimeout = andriodMaxBytesToTimeout;
	this->chunkBuffer = (unsigned char*) malloc(MTUsize);
	std::cout << " maxPS: " << this->maxPackageSize << "\n" << std::flush;
}

UDPProtkoll::~UDPProtkoll() {
	delete this->chunkBuffer;
}

int UDPProtkoll::sendInChunks(uint8_t kamera_id, unsigned char *buffer,
		size_t length) {

	bool errorFree = true;
	uint8_t chunkCounter = 0;
	unsigned int byteCounter = 0;
	uint8_t packageCount;
	packageCount = (uint8_t) ((length / this->maxPackageSize) + 1);
	int pauseZaehler = 0;

	if (kamera_id < 0)
		return -1;
	if (length < 1)
		return -1;
	if (buffer == NULL)
		return -1;
	do {
		// wipe chunkBuffer
		memset(chunkBuffer, 0, maxPackageSize);
		unsigned int lengthOfSendingContent = 0;

		chunkBuffer[0] = UDP_PROTOKOLL_VERSION;
		chunkBuffer[1] = (uint8_t) ((bild_id & 0xFF00) >> 8);
		chunkBuffer[2] = (uint8_t) (bild_id & 0x00FF);
		chunkBuffer[3] = chunkCounter;
		chunkBuffer[4] = kamera_id;
		chunkBuffer[5] = packageCount;

		if ((length - byteCounter) < maxPackageSize)
			lengthOfSendingContent = (length - byteCounter);
		else {
			if (this->maxPackageSize == 0)
				lengthOfSendingContent = 1466;
			else
				lengthOfSendingContent = maxPackageSize;
		}

		memcpy(chunkBuffer + UDP_HEADER_LENGTH, buffer + byteCounter,
				lengthOfSendingContent);

		if (client->sendData(chunkBuffer,
		//if (client->sendData(&chunkBuffer[0],
				lengthOfSendingContent + UDP_HEADER_LENGTH) != lengthOfSendingContent+UDP_HEADER_LENGTH) {
			errorFree = false;
		}

		byteCounter += lengthOfSendingContent;
		chunkCounter++;
		pauseZaehler = pauseZaehler + mtu;
		if (pauseZaehler >= andriodMaxBytesToTimeout) {
			usleep(andriodTimeout);
			pauseZaehler = 0;
		}
	} while ((byteCounter < length) && (errorFree));
	bild_id = (bild_id + 1) % 65536;
	if (!errorFree) {
		return 0;
	}
	return byteCounter;
}

void UDPProtkoll::setMTUsize(int MTUsize) {
	this->mtu = MTUsize;
	//this->chunkBuffer.resize(MTUsize);
	this->chunkBuffer = (unsigned char*) realloc(this->chunkBuffer, MTUsize);
}

int UDPProtkoll::getMTUsize() {
	return this->mtu;
}
