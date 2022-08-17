/*
* Alexander Florendo
* Professor Zheng Li
* Summer 2022
* Program 2
*/

/*
 * specialcountmulthreads.c - this file implements the alphabetcountmulthreads function.
 */

#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h> 
#include <string.h>
#include "count.h"

int gFilesPerThreadCount[FILECOUNT]; //stores how many files each thread reads
char gFileNamesList[FILECOUNT][50];  //stores the filenames from ../data folder
int gNumThreads;
char *gPath;
long *gFreqReference;
int gCounter = 0;

//thread function 
void *specialCounter(void* arg){
  int startNum = 0; //starting index of file to read
  int endNum = 0;   //ending index of file to read 

  FILE *inputFile;
  char line[1024];
  char delimiter[] = "!;:.,?-\"[](){} "; //delimiter to remove these punctations

  //initialize mutex lock for thread use 
  pthread_mutex_t mutexLock;
  pthread_mutex_init(&mutexLock, NULL);

  pthread_mutex_lock(&mutexLock);
  endNum = gFilesPerThreadCount[gCounter];

  //if 0, set startNum to 0
  if(gCounter == 0)  
    startNum = (gFilesPerThreadCount[gCounter] - gFilesPerThreadCount[0]);
  //31 files does not split evenly, so at last iteration, set startnum to 31 - (numThreads + filesPerThread)
  else if (gCounter == gNumThreads - 1)
    startNum = FILECOUNT - (gNumThreads + gFilesPerThreadCount[0]);
  else 
    startNum = ((gFilesPerThreadCount[gCounter] - gFilesPerThreadCount[0]) + 1); 
  gCounter++;
  pthread_mutex_unlock(&mutexLock);

  printf("Thread id = %u starts process files with index from %i to %i\n",pthread_self(), startNum, endNum);

  //iterate only through starting and ending indexes 
  for (int i = startNum; i < endNum; i++){
    //prep and validate .txt file to read
    pthread_mutex_lock(&mutexLock);
    inputFile = fopen(gFileNamesList[i], "r");
    pthread_mutex_unlock(&mutexLock);

    printf("Thread id = %u is processing  %s\n", pthread_self(), gFileNamesList[i]);
    
    if (inputFile == NULL)
      printf("Unable to open file: %s\n", gFileNamesList[i]);
    else{
      //grab contents of .txt file line by line
      while (fgets(line, 1024, inputFile)){
        //from grabbed line, tokenize the sentence and split line by delimiter
          pthread_mutex_lock(&mutexLock);
          char *lineptr = strtok(line, delimiter);
          pthread_mutex_unlock(&mutexLock);
          //while current token is not null
          while (lineptr != NULL){
            //change word (lineptr) to lowercase 
            for (int i = 0; i < strlen(line); i++){

              pthread_mutex_lock(&mutexLock);
              char temp = lineptr[i];
              pthread_mutex_unlock(&mutexLock);

              pthread_mutex_lock(&mutexLock);
              lineptr[i] = tolower(temp);
              pthread_mutex_unlock(&mutexLock);
            }
            /*
            *if word matches special word, increment counter
            *strcmp is used bc it actually compares the contents of the array
            *vs. using '==' which would only compare if pointers are pointing at the same address
            */
            if (!strcmp(lineptr, "he")){
              pthread_mutex_lock(&mutexLock);
              gFreqReference[0]++;
              pthread_mutex_unlock(&mutexLock);
              }

            if (!strcmp(lineptr, "she")){
              pthread_mutex_lock(&mutexLock);
              gFreqReference[1]++;
              pthread_mutex_unlock(&mutexLock);
            }

            if (!strcmp(lineptr, "they")){
              pthread_mutex_lock(&mutexLock);
              gFreqReference[2]++;
              pthread_mutex_unlock(&mutexLock);
            }

            if (!strcmp(lineptr, "him")){
              pthread_mutex_lock(&mutexLock);
              gFreqReference[3]++;
              pthread_mutex_unlock(&mutexLock);
            }

            if (!strcmp(lineptr, "me")){
              pthread_mutex_lock(&mutexLock);
              gFreqReference[4]++;
              pthread_mutex_unlock(&mutexLock);
            }
              
              lineptr = strtok(NULL, delimiter);
              
          }
        } //end inner while
      }//end else
    } // end if(!strcmp)
  printf("Thread id = %u is done! \n", pthread_self());
}// specialcounter func


void specialcountmulthreads(char *path, char *filetowrite, long specialfreq[], int num_threads){
  int filesToReadNum;
  gPath = path;
  gNumThreads = num_threads;
  int counter = 0;

  DIR *directory;
  struct dirent *entry;
  directory = opendir(gPath);

  gFreqReference = specialfreq;
  char fileName[50];
  char *extensionValid;
  char extensionName[] = ".txt";
  char ch = '.';

  pthread_t threads[num_threads];

  // open output file and validate if it cant be opened.
  FILE *outputFile;
  outputFile = fopen(filetowrite, "w");
  if (outputFile == NULL){
    printf("Unable to open specialresult.txt\n");
    exit(1);
  }

  if (num_threads > FILECOUNT){
    printf("Number of threads exceeds number of files. Enter num <= 31. \n");
    exit(1);
  }
  //**calculate how many files each thread reads and store into array**
  gFilesPerThreadCount[0] = 0;
  filesToReadNum = (FILECOUNT / num_threads) - 1; //subtract 1 bc files index starts at 0
  for(int i = 0; i < num_threads; i++){
    gFilesPerThreadCount[i] = gFilesPerThreadCount[i-1] +  filesToReadNum;
    if(i == (num_threads - 1))
      gFilesPerThreadCount[num_threads - 1] = FILECOUNT;
  }
  //**prepare txt files and store into an array**
    //if directory is open, continue
  if (directory){
    entry = readdir(directory);
    while (entry != 0){
      //find the last occurence of '.' from entry name to grabe file extension
      extensionValid = strrchr(entry->d_name, ch);
      //if grabbed file extension (extensionValid) matches '.txt', continue
      if (!strcmp(extensionValid, extensionName)){
        //concatenate ../data with inputted file name 
        strcpy(fileName, gPath);
        strcat(fileName, "/");
        strcat(fileName, entry->d_name);
        //add files names to array
        strcpy(gFileNamesList[counter], fileName);
        counter++;
      }//end !strcmp
        entry = readdir(directory);
    }//end while
  }//end if

  //create threads
  for (int i = 0; i < num_threads; i++){
    pthread_create(threads + i, NULL, &specialCounter, NULL);  
  }

  //wait for each thread to complete 
  for (int i = 0; i < num_threads; i++){
    pthread_join(threads[i], NULL);
  }
  printf("\n");
  
    //displayoutput to result.txt for special count
    for (int i = 0; i < sizeof(specialfreq); i++)
    {
      switch (i)
      {
      case 0:
        fprintf(outputFile, "he -> %d\n", specialfreq[i]);
        break;
      case 1:
        fprintf(outputFile, "she -> %d\n", specialfreq[i]);
        break;
      case 2:
        fprintf(outputFile, "they -> %d\n", specialfreq[i]);
        break;
      case 3:
        fprintf(outputFile, "him -> %d\n", specialfreq[i]);
        break;
      case 4:
        fprintf(outputFile, "me -> %d\n", specialfreq[i]);
        break;
      }//end switch
    }//end for

    fclose(outputFile);
}//end specialcountmulthreads
