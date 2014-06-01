/*
 * SerialWrapper.cpp
 *
 *  Created on: 01.06.2014
 *      Author: dennis
 */

#include "SerialWrapper.h"

SerialWrapper::SerialWrapper(std::string serialPortName) {
	// TODO Auto-generated constructor stub
	hw_serialPort.Open(serialPortName);
	if (!hw_serialPort.good())
		this->~SerialWrapper();

	hw_serialPort.SetBaudRate(SerialStreamBuf::BAUD_115200);
	if (!hw_serialPort.good())
		this->~SerialWrapper();

	hw_serialPort.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	if (!hw_serialPort.good())
		this->~SerialWrapper();
	hw_serialPort.SetParity(SerialStreamBuf::PARITY_NONE);
	if (!hw_serialPort.good())
		this->~SerialWrapper();
	hw_serialPort.SetNumOfStopBits(1);
	if (!hw_serialPort.good())
		this->~SerialWrapper();
	hw_serialPort.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
	if (!hw_serialPort.good())
		this->~SerialWrapper();
	hw_serialPort.unsetf(std::ios_base::skipws);
	if (!hw_serialPort.good())
		this->~SerialWrapper();
}

SerialWrapper::~SerialWrapper() {
	// TODO Auto-generated destructor stub
}

void SerialWrapper::sendPos(uint8_t groupID, uint8_t desiredXpos, uint8_t desiredYpos)
{
	this->sendBuffer[0] = groupID;
	this->sendBuffer[1] = desiredXpos;
	this->sendBuffer[2] = desiredYpos;
	this->sendBuffer[3] = SERIAL_ENDE_00_02;
	this->sendBuffer[4] = SERIAL_ENDE_01;
	this->sendBuffer[5] = SERIAL_ENDE_00_02;
	hw_serialPort.write(this->sendBuffer, SERIAL_SEND_BUFFER_LENGTH);
}
