/*
 * UDPProtkoll.h
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#ifndef UDPPROTKOLL_H_
#define UDPPROTKOLL_H_

#define UDP_DATAGRAMM_LENGTH 28 // iPv4 Header(20) + UDP Header (8)
#define UDP_HEADER_LENGTH 5 	//Protokoll-Version[1Byte];Bildnummer[2Byte];Paketnummer[1Byte];Kamera-ID[1Byte];Bild-Payload[~1468Bytes]
#define UDP_PROTOKOLL_VERSION 1

#include "UDPClient.h"
#include <cstdio>
#include <stddef.h>
#include <stdlib.h>

class UDPProtkoll {
public:
	UDPProtkoll(UDPClient *client, int MTUsize = 1500);
	virtual ~UDPProtkoll();

	int sendInChunks(uint8_t kamera_id, unsigned char *buffer, size_t length);

private:
	unsigned int maxPackageSize;
	UDPClient *client;
	uint16_t bild_id;

};

#endif /* UDPPROTKOLL_H_ */
