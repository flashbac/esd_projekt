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
		//exit(EXIT_FAILURE);
	}
	count = getline(&line, &len, f);

	if (count == -1) {
		perror("Error opening:");
		//exit(EXIT_FAILURE);
	}
	sscanf(line, "%d\n", &mtu);
	fclose(f);

	return mtu;
}

int Helper::kbhit(void) {
	struct termios term, oterm;
	int fd = 0;
	int c = 0;

	tcgetattr(fd, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	term.c_lflag = term.c_lflag & (!ICANON);
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 1;
	tcsetattr(fd, TCSANOW, &term);
	c = getchar();
	tcsetattr(fd, TCSANOW, &oterm);
	if (c != -1)
		ungetc(c, stdin);
	return ((c != -1) ? 1 : 0);
}
