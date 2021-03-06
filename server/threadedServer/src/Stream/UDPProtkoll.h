/*
 * UDPProtkoll.h
 *
 *  Created on: 08.05.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef UDPPROTKOLL_H_
#define UDPPROTKOLL_H_

#define UDP_DATAGRAMM_LENGTH 28 // iPv4 Header(20) + UDP Header (8)
#define UDP_HEADER_LENGTH 6 	//Protokoll-Version[1Byte];Bildnummer[2Byte];Paketnummer[1Byte];Kamera-ID[1Byte];Paketanzahl[1Byte];Bild-Payload[~1468Bytes]
#define UDP_PROTOKOLL_VERSION 1

#include <cstdio>
#include <stddef.h>
#include <stdlib.h>
#include "UDPClient.h"
#include <vector>

class UDPProtkoll {
public:
	UDPProtkoll(UDPClient *client, int MTUsize = 1500, int andriodTimeout = 3000, int andriodMaxBytesToTimeout = 10240);
	virtual ~UDPProtkoll();

	int sendInChunks(uint8_t kamera_id, unsigned char *buffer, size_t length);
	void setMTUsize(int MTUsize);
	int getMTUsize();

private:
	unsigned int maxPackageSize;
	UDPClient *client;
	uint16_t bild_id;
	int mtu;
	int andriodMaxBytesToTimeout;
	int andriodTimeout;
	//std::vector<unsigned char> chunkBuffer;
	unsigned char *chunkBuffer;

};

#endif /* UDPPROTKOLL_H_ */
