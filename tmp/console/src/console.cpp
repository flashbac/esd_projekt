//============================================================================
// Name        : console.cpp
// Author      : René Galow
// Version     :
// Copyright   : none
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commando.h"



int main() {

	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	char input[80+1];
	int zustand;
	while (zustand != -1)
	{
		printf("# ");

 		fgets(input, 80, stdin);

 		printf("%s", input );

 		//zustand = commando(input);
	}
 	printf("\nProgramm Ende\n");
 	return 0;

}
