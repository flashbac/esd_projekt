/*
 * ThreadSafeLogger.h
 *
 *  Created on: 17.06.2014
 *      Author: Dennis Rump & René Galow - Beuth Hochschule Berlin
 *      Lizenz: GPL v2
 */

#ifndef THREADSAFELOGGER_H_
#define THREADSAFELOGGER_H_

#include <semaphore.h>
#include <iostream>
#include <string>

class ThreadSafeLogger {
public:
	virtual ~ThreadSafeLogger();
	static ThreadSafeLogger& instance();
	void print(std::string message);

private:
	ThreadSafeLogger();
	ThreadSafeLogger(const ThreadSafeLogger&);
	ThreadSafeLogger & operator =(const ThreadSafeLogger &);

	sem_t sem_print;
};

#endif /* THREADSAFELOGGER_H_ */
