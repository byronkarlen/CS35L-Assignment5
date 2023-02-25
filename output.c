#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

bool writebytes (unsigned long long x, int nbytes){
  do{
    if (putchar (x) < 0)
	    return false;
    x >>= CHAR_BIT;
    nbytes--;
  }
  while (0 < nbytes);

  return true;
}

bool writebytes2(unsigned long long x, int outbytes) {
    int blockSize = outbytes;
    char* buffer = malloc(blockSize);

    if (buffer == NULL) 
      return false;

    for (int i = 0; i < blockSize; i++) {
      buffer[i] = (char)x;
      x >>= CHAR_BIT;
    }

    int bytesWritten = write(STDOUT_FILENO, buffer, blockSize);
    if (bytesWritten < 0) {
        free(buffer);
        return false;
    }

    free(buffer);
    return true;
}
