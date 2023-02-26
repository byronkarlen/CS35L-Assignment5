#include "rand64-sw.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
void software_rand64_init (char* inputfile){
  if(inputfile == NULL){
    urandstream = fopen ("/dev/random", "r");
    if (! urandstream)
      abort ();
  }
  else{
    urandstream = fopen (inputfile, "r");
    if (! urandstream)
      abort ();
  }
}

/* Return a random value, using software operations.  */
unsigned long long software_rand64 (void){
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

void software_rand64_fini (void)
{
  fclose (urandstream);
}

void software_mrand48_r_init (char* inputfile){

}

unsigned long long software_mrand48_r (void){
  struct drand48_data buffer;
    srand48_r(time(NULL), &buffer);

    // Generate a random number
    long int result;
    int ret = mrand48_r(&buffer, &result);
    if(ret){
      abort();
    }

    return result;
}

void software_mrand48_r_fini (void){

}