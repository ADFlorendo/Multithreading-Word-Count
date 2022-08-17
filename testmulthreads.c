/* This file is to test your program#2.
 * 
 */

#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include<unistd.h> 
#include <stdbool.h> 
#include "count.h"

/*
 * Print the frequencies of special words stored in array: specialfreq[] to output screen in the format as:
 * letter -> frequency (one letter a line)
 * Input: specialfreq - array having the frequency of each special word
          size     - the total number of special words
 * example:
 * he -> 250932
 * she -> 181764
 */
 
void displayalphabetfreq(long specialfreq[], int size)
{	
	
	for(int i = 0; i < size; i++)
	  {
		switch (i)
		{
			case 0:
			printf("%s -> %d\n", "he", specialfreq[i]);
			break;
			
			case 1:
			printf("%s -> %d\n", "she", specialfreq[i]);
			break;
			
			case 2:
			printf("%s -> %d\n", "they", specialfreq[i]);
			break;	
			
			case 3:
			printf("%s -> %d\n", "him", specialfreq[i]);
			break;	
			
			case 4:
			printf("%s -> %d\n", "me", specialfreq[i]);
			break;	
			
			defalut:
			printf("%s", "Wrong number of special words ... ");
		}				
	  }
}

int main(int argc, char *argv[])
{
	
	  printf("Please enter 2 arguments only eg.\"./testmulthreads #_of__threads!!\"\n");
	  
	  int num_threads = atoi(argv[1]);  
	  
	  char *path = "../data";           // the data *.txt files are under this folder
	  char *filetowrite = "../result/result.txt";    // the frequency of all alphabetical letters will be written in this file
	  
	  long specialfreq[SPECIALSIZE] = {0}; // array to store the frequency of each alphablet letter, which should be alway up-to-date;
	  
	  specialcountmulthreads(path, filetowrite, specialfreq, num_threads);  // process the data files using mutiple threads
	  
	  printf("The results are counted as follows : \n");
	  
	  displayalphabetfreq(specialfreq, SPECIALSIZE);  // print the frequency stored in the array to output screen
}
