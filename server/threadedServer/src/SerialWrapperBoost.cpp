/*
 * SerialWrapperBoost.cpp
 *
 *  Created on: 14.06.2014
 *      Author: dennis
 */

#include "SerialWrapperBoost.h"

SerialWrapperBoost::SerialWrapperBoost(std::string port, unsigned int baud_rate) :
		io(), serial(io, port) {
	serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}

SerialWrapperBoost::~SerialWrapperBoost() {
	// TODO Auto-generated destructor stub
}

/**
 * Write a string to the serial device.
 * \param s string to write
 * \throws boost::system::system_error on failure
 */
void SerialWrapperBoost::writeString(std::string s) {
	boost::asio::write(serial, boost::asio::buffer(s.c_str(), s.size()));
}

void SerialWrapperBoost::writeBuffer(char* buffer, unsigned int length) {
	boost::asio::write(serial, boost::asio::buffer(buffer, length));
}

/**
 * Blocks until a line is received from the serial device.
 * Eventual '\n' or '\r\n' characters at the end of the string are removed.
 * \return a string containing the received line
 * \throws boost::system::system_error on failure
 */
std::string SerialWrapperBoost::readLine() {
	//Reading data char by char, code is optimized for simplicity, not speed
	using namespace boost;
	char c;
	std::string result;
	for (;;) {
		asio::read(serial, asio::buffer(&c, 1));
		switch (c) {
		case '\r':
			break;
		case '\n':
			return result;
		default:
			result += c;
		}
	}
	return result;
}

bool SerialWrapperBoost::isOpen() {
	return serial.is_open();
}
