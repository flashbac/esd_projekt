/*
  Echtzeitsysteme WS 11/12
  Dennis Rump
  S771004
  31.10.2011

  Uebung4 EZS
  Dieses Programm kopiert eine Datei ueber zwei threads
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>
#include <sched.h>
#include <termios.h> 
#include "fileio.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PufferLaenge 10	   // anzahl der Elemnete
#define BlockGroesse 16  // 1k
#define INTER_THREAD 0

using namespace std;

sem_t sem_freeSpace;
sem_t sem_numberToWrite;

pthread_t Treader;
pthread_t Twriter;

pthread_t T1;
pthread_t T2;

char* ringpuffer[PufferLaenge];

int errn = 0;

char * getNextFree(){
// nächst freien Puffer geben
  static unsigned int Rcounter = 0;
  // slot reservieren bzw. auf freien solt warten
  sem_wait(&sem_freeSpace);
  //ringindex inc
  Rcounter++;
  
  // rueckhgabe des naechsten ringpointers
  return ringpuffer[ Rcounter = ( Rcounter % PufferLaenge ) ];
}

char * getNextToRead(){
// naechsten zu schreibenen puffer geben
static unsigned int Rcounter = 0;

  // auf naechste element warten
  sem_wait(&sem_numberToWrite);
  
  // ringindex inc
  Rcounter++;

  //rueckgabe des naechsten ringpointers
  return ringpuffer[ Rcounter = ( Rcounter % PufferLaenge ) ];
}

void reader(char * Filename){
//Daten einlesen
  unsigned short running = 1;

  while(running){
    // pointer fuer zielspeicher und "erden"
    char * tmp = NULL;
    //naechsten freine speicherplatz besorgen
    tmp = getNextFree();
    
    //laengen pointer holen
    int *pi = (int*) tmp;
    // <BlockGroesse> einlesen (und nach zieladresse schreiben)
    //  und laenge schreiben (nach *pi)
    *pi = myRead( (tmp + 4), BlockGroesse);
    
    //wenn nix weiter eingelesen wurde dann ende
    // + fehlerfall
    if( *pi <=0 ){
      running = 0;
      if( *pi < 0){
        perror("Sourcefile not ready to read\n");
        errn = 1;
      }
    }
  // schreibslot freigeben
  sem_post(&sem_numberToWrite);
  }
}
void writer(char * Filename){
//Daten schreiben
  int unsigned short running = 1;

  while(running){ //autoabbruch wenn keine daten mehr vorhanden
    // zeiger erstellen und "erden"
    char * tmp = NULL;
    //naechsten zu schreibenen Pointer holen
    tmp = getNextToRead();
    
    // lenge rausfiltern
    int *pi = (int*) tmp;
    
    // wenn laenge groesser Null schreiben
    // wenn nicht thread ende
    if(*pi > 0){
      myWrite( (tmp + 4), *pi);
    }else{
      running = 0;
      if( *pi < 0 ){
        perror("Destinationfile not ready to Write\n");
        errn = 1;
      }
    }
   
   //Speicherslot freigeben
   sem_post(&sem_freeSpace);
  }
}

void thread1(char * Filename){
 String face_cascade_name = "/usr/share/opencv/lbpcascades/lbpcascade_frontalface.xml";
 String eyes_cascade_name = "/usr/share/opencv/lbpcascades/haarcascade_eye_tree_eyeglasses.xml";
 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 string window_name = "Capture - Face detection";
 RNG rng(12345);

CvCapture* capture;
   Mat frame;

   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   //-- 2. Read the video stream
   capture = cvCaptureFromCAM( -1 );
   if( capture )
   {
     while( true )
     {
   frame = cvQueryFrame( capture );

   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
}

void thread2(char * Filename){

}

int main(int argc, char* argv[]){

/*
  if((argc < 3) || (argc >3)){
    printf("\n\nFkt. need two filenames\nub4 <filenameToRead> <filenameToWrite>\n\n");
    return 2; //durch define ersetzten
  }
  int i =0;
  // init des ringpuffers
  for(i=0;i<PufferLaenge;i++){
    //speicher allokieren für Block plus 4Byte für length
    ringpuffer[i] = malloc( (sizeof(char) * BlockGroesse) + 4 );
  }
  // Wie oft darf geschreiben werden
  if(sem_init(&sem_freeSpace,INTER_THREAD,PufferLaenge) < 0){
    perror("Error: init Semaphore freeSpace");
    return 1; // durch define ersetzten
  }
  if(sem_init(&sem_numberToWrite,INTER_THREAD,0) < 0){
    perror("Error: init Semaphore numberToWrite");
  }

  if ( myOpen( argv[1], argv[2] ) == (ERR_OPEN_SRC_FILE || ERR_OPEN_DEST_FILE)){
    perror("Error: Can't open file\n");
    return 1;
  }
  if(pthread_create(&Treader, NULL, (void*) reader, (void*) ringpuffer) == -1){
    perror("Error: can't create producer Thread\n");
    return 3; // define
  }
  if(pthread_create(&Twriter, NULL, (void*) writer, (void*) ringpuffer) == -1){
    perror("Error: can't create consumer Thread\n");
    return 3; //define
  }
  printf("Datei %s wird kopiert nach %s\n",argv[1],argv[2]);
  
  // mit dem join auf den writer thread warten
  long status;
  if( pthread_join(Twriter, (void**) &status) == -1 ){
    perror("Error while copy\n");
  }
 
  // Datein schließen
  myClose(); 
  // speicher weider frei geben
  for(i=0;i<PufferLaenge;i++){
    free(ringpuffer[i]);
  }
  if(!errn) printf("Datei wurde kopiert\n");
*/

  if(pthread_create(&T1, NULL, (void*) thread1, (void*) ringpuffer) == -1){
    perror("Error: can't create producer Thread\n");
    return 3; // define
  }
  if(pthread_create(&T2, NULL, (void*) thread2, (void*) ringpuffer) == -1){
    perror("Error: can't create consumer Thread\n");
    return 3; //define
  }
  return 0;
}
