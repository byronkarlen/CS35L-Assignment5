#ifndef RAND64_SW_H
#define RAND64_SW_H

#include <stdlib.h>
#include <stdio.h>

void software_rand64_init (char*);

unsigned long long software_rand64 (void);

void software_rand64_fini (void);

void software_mrand48_r_init (char*);

unsigned long long software_mrand48_r (void);

void software_mrand48_r_fini (void);


#endif