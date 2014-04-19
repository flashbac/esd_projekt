#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "commando.h"

int main(void)
{
  int sockfd = 0,connfd = 0;
  int i;
  struct sockaddr_in serv_addr;

  char sendBuff[1025];
  int numrv;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd >= -1)
	  printf("socket retrieve success\n");
  else
  {
	  printf("Can not open socket.\n");
	  return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);


   	bind(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(sockfd, 10) == -1){
    	printf("Failed to listen\n");
    	return -1;
    }

    i=0;
  	int zustand = 0;

    while(1)
    {

    	connfd = accept(sockfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
    	sprintf(sendBuff,"Connect to Server.\n");


    	//strcpy(sendBuff, "Message from server");
    	write(connfd, sendBuff, strlen(sendBuff));

    	int n;
    	char recvBuff[200];
    	while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    	{
    	        recvBuff[n] = 0;
    	        if(fputs(recvBuff, stdout) == EOF)
    	        {
    	            printf("\n Error : Fputs error\n");
    	        }
    	}

    	while (zustand != -1)
    	{
    		zustand = commando(recvBuff);
    	}

    	close(connfd);
    	sleep(1);
    }
    return 0;
}
