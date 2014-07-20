/*
 * ThreadSafeLogger.cpp
 *
 *  Created on: 17.06.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#include "ThreadSafeLogger.h"

ThreadSafeLogger& ThreadSafeLogger::instance() {
	static ThreadSafeLogger _instance;
	return _instance;
}

ThreadSafeLogger::ThreadSafeLogger() {
	sem_init(&sem_print, 0, 1);
}

ThreadSafeLogger::~ThreadSafeLogger() {
	// TODO Auto-generated destructor stub
}

void ThreadSafeLogger::print(std::string message) {
	sem_wait(&sem_print);
	std::cout << message <<'\n' << std::flush;
	sem_post(&sem_print);
}
