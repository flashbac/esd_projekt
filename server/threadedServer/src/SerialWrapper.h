/*
 * SerialWrapper.h
 *
 *  Created on: 01.06.2014
 *      Author: dennis
 */

#ifndef SERIALWRAPPER_H_
#define SERIALWRAPPER_H_

#define SERIAL_ENDE_00_02 0xff
#define SERIAL_ENDE_01 0x00
#define SERIAL_SEND_BUFFER_LENGTH 6

#include "settings.h"
#include <SerialStream.h>

using namespace LibSerial;

class SerialWrapper {
public:
	SerialWrapper(std::string serialPortName =
			SerialPortForServoGroupCommunication);
	virtual ~SerialWrapper();
	void sendPos(uint8_t groupID, uint8_t x=90, uint8_t y=90);
private:
	std::string serialPortName;
	SerialStream hw_serialPort;
	char sendBuffer[SERIAL_SEND_BUFFER_LENGTH];
};

#endif /* SERIALWRAPPER_H_ */
