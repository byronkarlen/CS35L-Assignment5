#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

bool isPositiveInteger(char* input);

int parseArguments(int argc, char **argv, long long *nbytes, char **inputParam, char **outputParam);

#endif