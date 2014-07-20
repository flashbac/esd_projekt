/*
 * PositionToAngle.h
 *
 *  Created on: 16.06.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef POSITIONTOANGLE_H_
#define POSITIONTOANGLE_H_

#include <semaphore.h>
#include "./Kommunikation/KommunikationTypes.h"
#include "settings.h"
#include "ThreadSafeLogger.h"
#include "SerialWrapper.h"
#define OEFNUNGSWINKEL_KAMERA 30

class PositionToAngle {
public:
	static PositionToAngle& instance();
	virtual ~PositionToAngle();

	void setXFaktor(double newFaktor);
	void setYFaktor(double newFaktor);
	double getXFaktor();
	double getYFaktor();
	void calculateAndSendNewPosition(face_t pos, int regionsizeWidth, int regionsizeHeigth);

private:
	PositionToAngle();
	PositionToAngle(const PositionToAngle&);
	PositionToAngle & operator =(const PositionToAngle &);

	double multiply_X_faktor;
	double multiply_Y_faktor;
	double threshold;
	sem_t sem_faktor;
};

#endif /* POSITIONTOANGLE_H_ */
