/* charcount.h - This header file include type definitions (including function prototypes) and macros 
   used for the programing assignment two.
*/

#include <stdio.h> 

#define SPECIALSIZE 5     //The total number of special word "he","she","they","him" and "me" (case insensitive)
#define FILECOUNT 31

void *specialCounter();
void specialcountmulthreads(char *path, char *filetowrite, long specialfreq[], int num_threads);

