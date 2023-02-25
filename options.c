#include "options.h"

bool isPositiveInteger(char* input){
    char *endptr;
    errno = 0; 
    long int num = strtol(input, &endptr, 10);
    if(errno != NULL || *endptr != '\0' || input == endptr || num < 0){
        return false;
    }
    else{
        return true; 
    }
}

int parseArguments(int argc, char **argv, long long *nbytes, char **inputParam, char **outputParam){
    //bool valid = true;

    bool optionI = false;
    bool optionO = false;

    int c; 
    while((c = getopt(argc, argv, "i:o:")) != -1){
        switch(c){
            case 'i':
                if(optarg){
                    *inputParam = strdup(optarg);
                    optionI = true;
                }
                break;
            case 'o':
                if(optarg){
                    *outputParam = strdup(optarg);
                    optionO = true;
                }
                break;
        }
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
            if(strcmp(*outputParam, "stdio") && isPositiveInteger(*outputParam)){
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
