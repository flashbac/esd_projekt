/*
 * SerialWrapper.cpp
 *
 *  Created on: 01.06.2014
 *      Author: dennis
 */

#include "SerialWrapper.h"

#include <SerialStreamBuf.h>
#include <cstdint>
#include <iostream>
#include <string>

SerialWrapper& SerialWrapper::instance() {
	static SerialWrapper _instance;
	return _instance;
}

SerialWrapper::SerialWrapper() {
	sem_init(&sendSem, 0, 1);
	hw_serialPort.Open(SerialPortForServoGroupCommunication);
	hw_serialPort.SetBaudRate(SerialStreamBuf::BAUD_115200);
	hw_serialPort.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	hw_serialPort.SetParity(SerialStreamBuf::PARITY_NONE);
	hw_serialPort.SetNumOfStopBits(1);
	hw_serialPort.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
	hw_serialPort.unsetf(std::ios_base::skipws);
}

SerialWrapper::~SerialWrapper() {
	// TODO Auto-generated destructor stub
}

void SerialWrapper::sendPos(uint8_t groupID, uint8_t desiredXpos,
		uint8_t desiredYpos) {
	sem_wait(&sendSem);
	if (hw_serialPort.IsOpen() && hw_serialPort.good()) {
		this->sendBuffer[0] = groupID;
		this->sendBuffer[1] = desiredXpos;
		this->sendBuffer[2] = desiredYpos;
		this->sendBuffer[3] = SERIAL_ENDE_00_02;
		this->sendBuffer[4] = SERIAL_ENDE_01;
		this->sendBuffer[5] = SERIAL_ENDE_00_02;
		hw_serialPort.write(this->sendBuffer, SERIAL_SEND_BUFFER_LENGTH);
		hw_serialPort.flush();
	}
	sem_post(&sendSem);
}
