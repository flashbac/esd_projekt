/* Echzeitsysteme WS 11/12
   Dennis Rump
   Uebung 4
   20.11.2011
*/
#include "fileio.h"

/**
* Fieldescriptoren
**/
FILE *fpSRC;
FILE *fpDEST;


/**
* function myOpen
* Opens a file
*
* /in src as char pointer: put here the Sourcefilename
* /in dest char poiter:   put here the Destinationfilename
* /return a integer with errorcode zero is success
**/
int myOpen(char* src, char* dest){
  if( (fpSRC = fopen( src, "r")) == NULL ){
    return ERR_OPEN_SRC_FILE;
  }
  
  if( (fpDEST = fopen( dest, "w")) == NULL ){
    return ERR_OPEN_DEST_FILE;
  } 
  return SUCCESS;
}

/**
* function myRead
* Read from the the sourcefile, that was open by MyOpen
*
* /in buf char pointer: put here the destination pointer
* /in len as size_len:  put here the count of bytes, that will be read
* /return the count of readed bytes
**/
size_t myRead(char* buf, size_t len){
  // ueberpruefen ob file initialisiert wurde
  if( fpSRC == NULL ){
    // fehlerfall
    return ERR_FILE_NOT_INIT;
  }
  
  // Lesen der Quelldatei und rueckgabe der gelesen
  // zeichen
  return fread( buf, sizeof(char), (size_t) len, fpSRC );
}

/**
* function myWrite
* writes in the destinationfile, that was open bei myOpen
*
* /in buf as char pointer: put here the source pointer
* /in len  as size_t:      put here the length of write
* /return the count of written bytes
**/
size_t myWrite(const char* buf, size_t len){
  // ueberpruefen ob file initialisiert wurde
  if( fpDEST == NULL ){
    // fehlerfall
    return ERR_FILE_NOT_INIT;
  }

  // Schreiben in die zieldatei und rueckgabe der geschrieben
  // zeichen
  return fwrite( buf, sizeof(char), (size_t) len, fpDEST );
}

/**
* myClose
* close the filedescriptors of source and destination files
**/
void myClose(void){
  //dateien schliessen
  fclose(fpSRC);
  fclose(fpDEST);
}
