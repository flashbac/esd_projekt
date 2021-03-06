/*
 * SerialWrapper.h
 *
 *  Created on: 01.06.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef SERIALWRAPPER_H_
#define SERIALWRAPPER_H_

#define SERIAL_ENDE_00_02 0xff
#define SERIAL_ENDE_01 0x00
#define SERIAL_SEND_BUFFER_LENGTH 6

#include "settings.h"
#include "SerialWrapperBoost.h"
#include <semaphore.h>
#include <stdint.h>

#define MAX_STEP_VALUE 10
#define MIN_Y_VALUE 0
#define MAX_Y_VALUE 180
#define MIN_X_VALUE 40
#define MAX_X_VALUE 146

class SerialWrapper {
public:
	static SerialWrapper& instance();

	virtual ~SerialWrapper();
	bool isOpen();
	void sendPos(uint8_t groupID, uint8_t x = 90, uint8_t y = 90);
	void sendDelta(uint8_t groupID, int8_t dx, int8_t dy);

private:
	SerialWrapperBoost* hw_serial;

	static SerialWrapper* _instance;

	char sendBuffer[SERIAL_SEND_BUFFER_LENGTH];
	sem_t sendSem;
	SerialWrapper(); // verhindert, dass ein Objekt von außerhalb von N erzeugt wird.
	// protected, wenn man von der Klasse noch erben möchte
	SerialWrapper(const SerialWrapper&); /* verhindert, dass eine weitere Instanz via
	 Kopier-Konstruktor erstellt werden kann */
	SerialWrapper & operator =(const SerialWrapper &); //Verhindert weitere Instanz durch Kopie
	uint8_t x;
	uint8_t y;

};

#endif /* SERIALWRAPPER_H_ */
