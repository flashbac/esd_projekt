//============================================================================
// Name        : console.cpp
// Author      : René Galow
// Version     :
// Copyright   : none
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>


using namespace std;

int main() {

	char input[80+1];
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	printf("# ");
	scanf("%s",input);
	fgets( input, 80, stdin );

	cout << input << endl ;




	return 0;
}
