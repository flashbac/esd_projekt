/* Echzeitsysteme WS 11/12
   Dennis Rump
   Uebung 4
   20.11.2011

   fileio.h
   Selbsterstellte fileoperationen
*/
#include <stdio.h>
#include <stdlib.h>

/* defines fuer Open ( etl. enum besser) */
#define ERR_FILE_NOT_INIT -1
#define ERR_OPEN_DEST_FILE 2
#define ERR_OPEN_SRC_FILE 1
#define SUCCESS 0

/**
* function myOpen
* Opens the sourcefile and destinationfile
*
* /in src as char pointer: put here the Sourcefilename
* /in dest char poiter:	   put here the Destinationfilename
* /return a integer with errorcode, zero is success
**/
int myOpen(char* src, char* dest);

/**
* function myRead
* Read from the the sourcefile, that was open by MyOpen
*
* /in buf char pointer: put here the destination pointer
* /in len as size_len:  put here the count of bytes, that will be read
* /return the count of readed bytes
**/
size_t myRead(char* buf, size_t len);

/**
* function myWrite
* writes in the destinationfile, that was open bei myOpen
*
* /in buf as char pointer: put here the source pointer
* /in len  as size_t:	   put here the length to write
* /return the count of written bytes
**/
size_t myWrite(const char* buf, size_t len);

/**
* myClose
* close the filedescriptors of source and destination files
**/
void myClose(void);
