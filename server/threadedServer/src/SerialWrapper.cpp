/*
 * SerialWrapper.cpp
 *
 *  Created on: 01.06.2014
 *      Author: dennis
 */

#include "SerialWrapper.h"

#include <iostream>



SerialWrapper& SerialWrapper::instance() {
	static SerialWrapper _instance;
	return _instance;
}

SerialWrapper::SerialWrapper() {
	sem_init(&sendSem, 0, 1);
	hw_serial = new SerialWrapperBoost(SerialPortForServoGroupCommunication,
			115200);
}

SerialWrapper::~SerialWrapper() {
	// TODO Auto-generated destructor stub
	if (hw_serial != NULL)
		delete hw_serial;
}

bool SerialWrapper::isOpen() {
	return hw_serial->isOpen();
}

void SerialWrapper::sendDelta(uint8_t groupID, int8_t dx, int8_t dy){

	if (dx >= MAX_STEP_VALUE)
	{
		x += MAX_STEP_VALUE;
	}
	else if (dx <= -MAX_STEP_VALUE)
	{
		x += -MAX_STEP_VALUE;
	}
	else
	{
		x += dx;
	}

	if (dy >= MAX_STEP_VALUE)
	{
		y += MAX_STEP_VALUE;
	}
	else if (dy <= -MAX_STEP_VALUE)
	{
		y += -MAX_STEP_VALUE;
	}
	else
	{
		y += dy;
	}
	if (x > MAX_X_VALUE) x = MAX_X_VALUE;
	if (x < MIN_X_VALUE) x = MIN_X_VALUE;
	if (x > MAX_Y_VALUE) x = MIN_Y_VALUE;
	if (x < MIN_Y_VALUE) x = MIN_Y_VALUE;
	SerialWrapper::sendPos(groupID, x, y);


}

void SerialWrapper::sendPos(uint8_t groupID, uint8_t desiredXpos,
		uint8_t desiredYpos) {
	x = desiredXpos;
	y = desiredYpos;
	sem_wait(&sendSem);
	if (hw_serial->isOpen()) {
		this->sendBuffer[0] = groupID;
		this->sendBuffer[1] = desiredXpos;
		this->sendBuffer[2] = desiredYpos;
		this->sendBuffer[3] = SERIAL_ENDE_00_02;
		this->sendBuffer[4] = SERIAL_ENDE_01;
		this->sendBuffer[5] = SERIAL_ENDE_00_02;
		//printf("Send to Serial x:%d y:%d\n", desiredXpos, desiredYpos);
		hw_serial->writeBuffer(this->sendBuffer, SERIAL_SEND_BUFFER_LENGTH);
	}
	sem_post(&sendSem);
}
