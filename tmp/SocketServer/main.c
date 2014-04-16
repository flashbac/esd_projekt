#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

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

commando("exit");

  i=0;
  while(1)
    {

      connfd = accept(sockfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
      sprintf(sendBuff,"Message from Server Nr.: %d", i);
      i++;
      //strcpy(sendBuff, "Message from server");
      write(connfd, sendBuff, strlen(sendBuff));



      close(connfd);
      sleep(1);
    }


  return 0;
}

int commando(char* commando)
{
	char* befehl;

	//find space or \n
	int i=0;
	i = memchr(commando,'X',strlen(commando));

	//for(i = 0;befehl[i] == '\0';i++)
	//{
	//	if( commando[i] == ' ')
	//	break;
	//}

	memcpy(befehl, commando, i);
	memset( (befehl + sizeof(char)* (i+1)), '/0', sizeof(befehl) );

	if (strcmp(befehl, "exit") == 0)
	{
	  printf("exit by client");
	  //return -1;
	}
	else if (strcmp(befehl, "print") == 0)
	{
		printf("%s",commando);
		return -1;
	}
	/* more else if clauses */
	else /* default: */
	{
		printf("%s","Befehl nicht gefunden.");
	}

	return -1;
}
