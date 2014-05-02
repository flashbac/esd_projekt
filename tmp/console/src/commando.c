#include "commando.h"

int commando(char* input)
{

 	char commando[80+1];
 	char parameter[80+1];



 	int leng;
 	leng = strlen(input);

 	if(input[leng-1] != '\n' )
 	{
 		printf("Zu viele Zeichen");
 		return -1;
 	}
 	else
 	{
 		char *space;
 		leng = strlen(input);
 		input[leng-1] = '\0'; // \n ersetzen durch \0
 	 	space = strchr(input, (int)32) ;

 		if (space == NULL) 	// Nur Commando
 		{
 			strcpy(commando, input);
 	 		parameter[0] = '\0';

 		}
 		else				// Commando und Parameter
 		{
 			strcpy(parameter, space+1);
 			*space = '\0';
 			strcpy(commando, input);
 		}
 		//printf("Commando: %s \n Parameter: %s",commando, parameter);
 	}

 /*
  * Befehl zuordnen
  */

	if (strcmp(commando, "exit") == 0)
	{
		//cout << "exit by client" << endl;
		printf("exit by client\n");

		return -1;
	}
	else if (strcmp(commando, "print") == 0)
	{
		printf("Parameter: %s\n",parameter);
		return 0;
	}
	/* more else if clauses */
	else /* default: */
	{
		printf("%s","Befehl nicht gefunden.\n");
	}

	return 0;
}
