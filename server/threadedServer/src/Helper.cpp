/*
 * Helper.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dennis
 */

#include "Helper.h"

#include <stddef.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>

Helper::Helper() {
	// TODO Auto-generated constructor stub

}

Helper::~Helper() {
	// TODO Auto-generated destructor stub
}

int Helper::getMTUsize(std::string devicename) {
	FILE *f;
	char buf[128];
	char *line = NULL;
	ssize_t count;
	size_t len = 0;
	int mtu;

	snprintf(buf, sizeof(buf), "/sys/class/net/%s/mtu", devicename.c_str());
	f = fopen(buf, "r");
	if(!f) {
		perror("Error opening:");
		exit(EXIT_FAILURE);
	}
	count = getline(&line, &len, f);

	if (count == -1) {
		perror("Error opening:");
		exit(EXIT_FAILURE);
	}
	sscanf(line, "%d\n", &mtu);
	fclose(f);

	return mtu;
}
