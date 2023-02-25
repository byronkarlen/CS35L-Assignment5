/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


#include <errno.h>
#include <immintrin.h>
#include <stdbool.h>
#include <cpuid.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"
#include "options.h"

void deallocateMemory(char* i, char* o){
  free(i);
  free(o);
}
/* Main program, which outputs N bytes of random data.  */
int main (int argc, char **argv){
  /* Check arguments.  */
  long long nbytes;

  char *inputParam = NULL;
  char *outputParam = NULL;

  int inputKey = 0; //1 if input is "rdrand", 2 if "mrand48_r", 3 if "filename"
  int outputKey = 0; //1 if output is stdio, 2 if a number
  long int N = -1;

  if(parseArguments(argc, argv, &nbytes, &inputParam, &outputParam)){
    fprintf(stderr, "%s: usage: %s NBYTES [-i <arg>] [-o <arg>]\n", "randall", "randall");
    deallocateMemory(inputParam, outputParam);
    return 1;
  }

  /* gather the correct command-line arguments */
  if(inputParam != NULL){ //If an inputParam was correctly supplied
    if(strcmp(inputParam, "rdrand") == 0){
      inputKey = 1; 
      printf("input parameter: rdrand\n");
    }
    else if(strcmp(inputParam,"mrand48_r") == 0){
      inputKey = 2; 
      printf("input parameter: mrand48_r\n");
    }
    else{ //inputParam is a filename
      inputKey = 3;
      printf("input parameter is this filename: %s\n", inputParam);
    } 
  }
  if(outputParam != NULL){
    if(strcmp(outputParam,"stdio") == 0){
      outputKey = 1; 
      printf("output parameter: stdio\n");
    }
    else{ //inputParam is a number
      outputKey = 2; 
      char *endptr;
      N = strtol(outputParam, &endptr, 10);
      printf("output parameter is this number: %ld\n" + N);
    } 
  }
  printf("number of bytes: %lld\n", nbytes);

  deallocateMemory(inputParam, outputParam);
  return 0;

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0){
    deallocateMemory(inputParam, outputParam);
    return 0;
  }

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (char*);
  unsigned long long (*rand) (void);
  void (*finalize) (void);
  
  if (rdrand_supported ()){
    initialize = hardware_rand64_init;
    rand = hardware_rand64;
    finalize = hardware_rand64_fini;
  }
  else{
    if(inputKey == 1){ 
      fprintf(stderr, "rdrand is not supported on this machine.\n");
    }
    else if(inputKey == 2){
      initialize = software_mrand48_r_init;
      rand = software_mrand48_r;
      finalize = software_mrand48_r_fini;
    }
    else{
      initialize = software_rand64_init;
      rand = software_rand64;
      finalize = software_rand64_fini;
    }
  }

  char* inputfile = NULL;
  if(inputKey == 3)
    inputfile = inputParam;
  initialize(inputfile);

  int wordsize;
  if(outputKey == 2){
    wordsize = N;
  }
  else{
    wordsize = sizeof rand ();
  }
  
  int output_errno = 0;

  do{
    unsigned long long x = rand ();
    int outbytes = nbytes < wordsize ? nbytes : wordsize;
    if (!writebytes (x, outbytes)){
	    output_errno = errno;
	    break;
    }
    nbytes -= outbytes;
  }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno){
    errno = output_errno;
    perror ("output");
  }

  finalize ();

  deallocateMemory(inputParam, outputParam);
  return !!output_errno;
}
