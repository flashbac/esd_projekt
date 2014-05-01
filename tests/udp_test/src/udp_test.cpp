//============================================================================
// Name        : udp_client_test.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>

#include <arpa/inet.h> // für ipadress to string conversion
//#include <stdio.h> // raus damit

#include <termios.h> // für kbhit()
#include <stdio.h>
//#include <unistd.h>
//#include <sys/select.h>
#include <string.h>

#include <unistd.h> // für sleep

using namespace std;

int getch() {
    static int ch = -1, fd = 0;
    struct termios neu, alt;
    fd = fileno(stdin);
    tcgetattr(fd, &alt);
    neu = alt;
    neu.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(fd, TCSANOW, &neu);
    ch = getchar();
    tcsetattr(fd, TCSANOW, &alt);
    return ch;
}

int kbhit(void) {
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

int main(int argc, char *argv[]) {
	bool isSrv = false;
	unsigned short port = 1;

	// list arguments
	cout << "\npassed Arguments:\n";
	for(int i=0; i<argc;i++) cout << "\tArgument " << i << " " << argv[i] << "\n";
	cout << "\n";

	if(argc>2)
	{
		cout << "Parameter vorhanden anzahl: " << argc << "\n";
		std::string str(argv[1]);
		std::istringstream ss(argv[2]);
		ss >> port;
		if(str == "srv")
		{
			isSrv = true;
			cout << "Start in Servermode\t Port:" << port << "\n";
		}else if(str == "cli")
		{
			cout << "Start in Clientmode\t Port:" << port << "\n";
		}else
		{
			cout << "unkown parameter\n exit!\n";
			return -1;
		}
	}else
	{
		cout << "need parameter\n\tsrv for Servermode\n\tcli for Clientmode\n\n exiting\n";
		return -1;
	}

	int handle = socket( AF_INET,
	                     SOCK_DGRAM,
	                     IPPROTO_UDP );

	if ( handle <= 0 )
	{
		cout << "failed to create socket\n";
	    return false;
	}else
	{
		cout << "handle for socket ok\n";
	}

	if(isSrv) //server
	{
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port =
			htons( (unsigned short) port );

		if ( bind( handle,
				   (const sockaddr*) &address,
				   sizeof(sockaddr_in) ) < 0 )
		{
			cout << "failed to bind socket\n";
			return -1;
		}else
		{
			cout << "bind ok\n";
		}

		// setzte nonblocking mode
		int nonBlocking = 1;
		if ( fcntl( handle,
		            F_SETFL,
		            O_NONBLOCK,
		            nonBlocking ) == -1 )
		{
		    cout << "failed to set non-blocking\n";
		    return false;
		}else
		{
			cout << "non-blocking mode active\n";
		}

		cout << "waiting for data\n";
		// main loop for server
		while ( !kbhit() )
		{;
		    unsigned char packet_data[256];

		    unsigned int max_packet_size =
		        sizeof( packet_data );

		    #if PLATFORM == PLATFORM_WINDOWS
		    typedef int socklen_t;
		    #endif

		    sockaddr_in from;
		    unsigned int fromLength = sizeof( from );

		    int bytes = recvfrom( handle,
		                          (char*)packet_data,
		                          max_packet_size,
		                          0,
		                          (sockaddr*)&from,
		                          &fromLength );

		    if ( bytes > 0 )
		    {
		     //   break;

		    unsigned int from_address =
		        ntohl( from.sin_addr.s_addr );

		    unsigned int from_port =
		        ntohs( from.sin_port );

		    // process received packet
		    char str[INET_ADDRSTRLEN];
		    inet_ntop(AF_INET, &(from_address), str, INET_ADDRSTRLEN);
		    cout << "received Data from " << str << "\n";
		    }
		}

	}
	else // client
 	{
		while(!kbhit()){
			unsigned char packet_data[256];
			unsigned int packet_size = 256;
			struct sockaddr_in dest_address;
			inet_pton(AF_INET, "127.0.0.1", &(dest_address.sin_addr));
			dest_address.sin_port = htons( port );
			dest_address.sin_family = AF_INET;

			int sent_bytes =
				sendto( handle,
						(const char*)packet_data,
						packet_size,
						0,
						(sockaddr*)&dest_address,
						sizeof(sockaddr_in) );

			if ( sent_bytes != packet_size )
			{
				cout << "failed to send packet\n";
				return false;
			}else
			{
				cout << "256 byte gesende\n";
			}
			sleep(1);
		}

	}


	cout << "ende.\n";
	return 0;
}
