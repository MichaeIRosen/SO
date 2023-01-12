#include "logging.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "[ERR]: specify action\n");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(argv[1], "mbroker") != 0){
        fprintf(stderr, "[ERR]: specify action for mbroker\n");
        return -1;
    }

    

    return -1;
}
