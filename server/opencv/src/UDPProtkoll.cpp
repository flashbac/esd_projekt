/*
 * UDPProtkoll.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#include "UDPProtkoll.h"

UDPProtkoll::UDPProtkoll(UDPClient *client, int MTUsize) {
	// TODO Auto-generated constructor stub
	if (client == NULL)
		this->~UDPProtkoll();
	this->client = client;

	this->maxPackageSize = MTUsize - UDP_DATAGRAMM_LENGTH - UDP_HEADER_LENGTH;
	this->bild_id = 0;
}

UDPProtkoll::~UDPProtkoll() {
	// TODO Auto-generated destructor stub
}

int UDPProtkoll::sendInChunks(uint8_t kamera_id, unsigned char *buffer,
		size_t length) {

	bool errorFree = true;
	uint8_t chunkCounter = 0;
	unsigned int byteCounter = 0;
	unsigned char *chunkBuffer = (unsigned char*) malloc(maxPackageSize);
	uint8_t packageCount = (uint8_t) ((length / maxPackageSize) + 1);

	if (kamera_id < 0)
		return -1;
	if (length < 1)
		return -1;
	if (buffer == NULL)
		return -1;

	printf("\n[send] bild_id: %d; anzahl der packete %d", bild_id,
			packageCount);

	do {
		// wipe chunkBuffer
		memset(chunkBuffer, 0, maxPackageSize);
		unsigned int lengthOfSendingContent = 0;

		chunkBuffer[0] = UDP_PROTOKOLL_VERSION;
		chunkBuffer[1] = (uint8_t)((bild_id & 0xFF00) >> 8);
		chunkBuffer[2] = (uint8_t)(bild_id & 0x00FF);
		chunkBuffer[3] = chunkCounter;
		chunkBuffer[4] = kamera_id;
		chunkBuffer[5] = packageCount;

		if ((length - byteCounter) < maxPackageSize)
			lengthOfSendingContent = (length - byteCounter);
		else
			lengthOfSendingContent = maxPackageSize;

		memcpy(chunkBuffer + UDP_HEADER_LENGTH, buffer + byteCounter,
				lengthOfSendingContent);

		if (client->sendData(chunkBuffer, lengthOfSendingContent+UDP_HEADER_LENGTH)
				!= lengthOfSendingContent+UDP_HEADER_LENGTH) {
			errorFree = false;
			printf("\nError while sending via UDP\n");
		}

		byteCounter += lengthOfSendingContent;
		chunkCounter++;
		//printf("\nsend chunk packageID: %d", chunkCounter);

	} while ((byteCounter < length) && (errorFree));

	//bild_id = (bild_id + 1) % sizeof(uint16_t);
	bild_id = (bild_id + 1) % 65536;
	// free allocate space
	free(chunkBuffer);

	if (errorFree) {
		//printf("\nsuccessfule send data with ID: %d", bild_id);
	}
	return byteCounter;
}
