/*
 * PositionToAngle.cpp
 *
 *  Created on: 16.06.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#include "PositionToAngle.h"

#include <cstdlib>
#include <sstream>

PositionToAngle& PositionToAngle::instance() {
	static PositionToAngle _instance;
	return _instance;
}

PositionToAngle::PositionToAngle() {
	sem_init(&sem_faktor, 0, 1);
	multiply_X_faktor = MULTIPLY_FACTOR_DEFAULT_X;
	multiply_Y_faktor = MULTIPLY_FACTOR_DEFAULT_Y;
	threshold = THRESHOLD_DEFAULT;
}

PositionToAngle::~PositionToAngle() {
}

void PositionToAngle::setXFaktor(double newFaktor) {
	sem_wait(&sem_faktor);
	multiply_X_faktor = newFaktor;
	sem_post(&sem_faktor);
}

void PositionToAngle::setYFaktor(double newFaktor) {
	sem_wait(&sem_faktor);
	multiply_Y_faktor = newFaktor;
	sem_post(&sem_faktor);
}

double PositionToAngle::getXFaktor() {
	double tmpFaktor = 1.0;
	sem_wait(&sem_faktor);
	tmpFaktor = multiply_X_faktor;
	sem_post(&sem_faktor);

	return tmpFaktor;
}

double PositionToAngle::getYFaktor() {
	double tmpFaktor = 1.0;
	sem_wait(&sem_faktor);
	tmpFaktor = multiply_X_faktor;
	sem_post(&sem_faktor);

	return tmpFaktor;
}

void PositionToAngle::calculateAndSendNewPosition(face_t pos,
		int regionsizeWidth, int regionsizeHeigth) {
	int mittelpunktYKameraBild = 0;
	int mittelpunktXKameraBild = 0;

	int mittelpunktYrechteckVomGesamtenBild = 0;
	int mittelpunktXrechteckVomGesamtenBild = 0;

	// Abweichung in Prozent
	int ermittelteAbweichnungY = 0;
	int ermittelteAbweichnungX = 0;

	double abweichungYInProzent = 0;
	double abweichungXInProzent = 0;

	double faceToAreaFaktor = 0;

	int stepsX = 0;
	int stepsY = 0;
	static int lastX = 0;
	static int lastY = 0;

	//mittelpunkt des Kamerabildes ermitteln
	mittelpunktYKameraBild = regionsizeHeigth / 2;
	mittelpunktXKameraBild = regionsizeWidth / 2;
	//printf("    Mittelpunkt Bild Y: %d\n", mittelpunktYKameraBild);
	//printf("    Mittelpunkt Bild X: %d\n", mittelpunktXKameraBild);
	//mittelpunkt vom rechteck im Gesamten Bild bestimmen
	mittelpunktYrechteckVomGesamtenBild = (pos.height / 2) + pos.y;
	mittelpunktXrechteckVomGesamtenBild = (pos.width / 2) + pos.x;

	//printf("Mittelpunkt Rechteck Y: %d\n", mittelpunktYrechteckVomGesamtenBild);
	//printf("Mittelpunkt Rechteck X: %d\n", mittelpunktXrechteckVomGesamtenBild);

	ermittelteAbweichnungY = mittelpunktYKameraBild
			- mittelpunktYrechteckVomGesamtenBild;
	ermittelteAbweichnungX = mittelpunktXKameraBild
			- mittelpunktXrechteckVomGesamtenBild;

	//printf("          Abweichung Y: %d\n", ermittelteAbweichnungY);
	//printf("          Abweichung X: %d\n", ermittelteAbweichnungX);

	abweichungYInProzent = (double) ermittelteAbweichnungY * 100.0
			/ (double) regionsizeHeigth;
	abweichungXInProzent = (double) ermittelteAbweichnungX * 100.0
			/ (double) regionsizeWidth;

	//printf(" Prozent Abweichung Y: %f%%\n", abweichungYInProzent);
	//printf(" Prozent Abweichung X: %f%%\n", abweichungXInProzent);

	stepsY = (int) (0.5
			+ (double) OEFNUNGSWINKEL_KAMERA / 100.0
					* (double) abweichungYInProzent);
	stepsX = (int) (0.5
			+ (double) OEFNUNGSWINKEL_KAMERA / 100.0
					* (double) abweichungXInProzent);
	//stepsX = -stepsX; // Umrechnen jeh nachdem wir Kamera drauf sitzt

	//printf(" Abweichung in Grad Y: %d\n", stepsY);
	//printf(" Abweichung in Grad X: %d\n", stepsX);

	//// faktor wird kleiner, um so groeßer das rechteck ist -> dadruch mehr bewegung bei kleineren rechtecken
	//faceToAreaFaktor = (regionsizeHeigth * regionsizeWidth)
	//		/ (pos.height * pos.width);
//
//	if (abs(ermittelteAbweichnungY) > threshold) {
//		stepsY = ((ermittelteAbweichnungY * multiply_Y_faktor) / 100.0)
//				* faceToAreaFaktor;
//	}
//
//	if (abs(ermittelteAbweichnungX) > threshold) {
//		stepsX = ((ermittelteAbweichnungX * multiply_X_faktor) / 100.0)
//				* faceToAreaFaktor;
//	}

	// sende
	/*SerialWrapper& serial = SerialWrapper::instance();
	 if (serial.isOpen()) {

	 }*/

	if ((lastX != stepsX) || (lastY != stepsY)) {
		std::stringstream ss;
		ss << "[debug]\tBerechnete steps x:" << stepsX << " y:" << stepsY
				<< "\n";
		//ThreadSafeLogger::instance().print(ss.str());
		SerialWrapper::instance().sendDelta(AvailabeServoGroups, stepsX,
				stepsY);

		lastX = stepsX;
		lastY = stepsY;
	}
}
