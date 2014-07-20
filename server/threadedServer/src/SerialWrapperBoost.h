/*
 * SerialWrapperBoost.h
 *
 *  Created on: 14.06.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef SERIALWRAPPERBOOST_H_
#define SERIALWRAPPERBOOST_H_

#include <boost/asio.hpp>

class SerialWrapperBoost {
public:
	/**
	 * Constructor.
	 * \param port device name, example "/dev/ttyUSB0" or "COM4"
	 * \param baud_rate communication speed, example 9600 or 115200
	 * \throws boost::system::system_error if cannot open the
	 * serial device
	 */
	SerialWrapperBoost(std::string port, unsigned int baud_rate);

	virtual ~SerialWrapperBoost();

	/**
	 * Write a string to the serial device.
	 * \param s string to write
	 * \throws boost::system::system_error on failure
	 */
	void writeString(std::string s);

	void writeBuffer(char* buffer, unsigned int length);

	/**
	 * Blocks until a line is received from the serial device.
	 * Eventual '\n' or '\r\n' characters at the end of the string are removed.
	 * \return a string containing the received line
	 * \throws boost::system::system_error on failure
	 */
	std::string readLine();

	bool isOpen();
private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
};
#endif /* SERIALWRAPPERBOOST_H_ */
