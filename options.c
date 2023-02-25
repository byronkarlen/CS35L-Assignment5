#include "options.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

bool isPositiveInteger(char* input){
    char *endptr;
    errno = 0; 
    long int num = strtol(input, &endptr, 10);
    if(errno != 0 || *endptr != '\0' || input == endptr || num < 0){
        return false;
    }
    else{
        return true; 
    }
}

int parseArguments(int argc, char **argv, long long *nbytes, char **inputParam, char **outputParam){
    int numargs = argc;
    bool optionI = 0;
    bool optionO = 0;

    int c; 
    while((c = getopt(argc, argv, "i:o:")) != -1){
        switch(c){
            case 'i':
                optionI = 1;
                if(optarg){
                    *inputParam = strdup(optarg);
                    if(*inputParam == NULL){
                        return 1;
                    }
                }
                break;
            case 'o':
                optionO = 1;
                if(optarg){
                    *outputParam = strdup(optarg);
                    if(*outputParam == NULL){
                        return 1;
                    }                    
                }
                break;
        }
    }
    if(numargs != optionI*2 + optionO*2 + 2){ //if an option is missing a corresponding parameter
        return 1; 
    }
    if(optionI){
        if(*inputParam != NULL){
            if(strcmp(*inputParam, "rdrand") && strcmp(*inputParam, "mrand48_r") && (*inputParam)[0] != '/'){
                return 1;
            }
        }
        else{
            return 1;
        }
    }
    if(optionO){
        if(*outputParam != NULL){
            if(strcmp(*outputParam, "stdio") && !isPositiveInteger(*outputParam)){
                return 1;
            }
        }
        else{
            return 1;
        }
        
    }

    if(optind >= argc){ //no number of bytes was supplied
        return 1;
    }
    if(optind < argc){
        bool valid = false;
        char *endptr;
        errno = 0;
        *nbytes = strtoll (argv[optind], &endptr, 10);
        if (errno)
            perror (argv[optind]);
        else
            valid = (!*endptr || *endptr == '-') && 0 <= *nbytes;

        if(!valid){
            return 1;
        }
    }
    
    return 0;
}
 