#include "logging.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE (128)


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "[ERR]: specify action\n");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(argv[1], "pub") != 0){
        fprintf(stderr, "[ERR]: specify action for pub\n");
        return -1;
    }
    
    // Open pipe for reading
    // This waits for someone to open it for writing
    int rx = open(argv[3], O_RDONLY);
    if (rx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (true) {
        char buffer[BUFFER_SIZE];
        ssize_t ret = read(rx, buffer, BUFFER_SIZE - 1);
        if (ret == 0) {
            // ret == 0 indicates EOF
            fprintf(stderr, "[INFO]: pipe closed\n");
            return 0;
        } else if (ret == -1) {
            // ret == -1 indicates error
            fprintf(stderr, "[ERR]: read failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    
    (void)argc;
    fprintf(stderr, "usage: sub <register_pipe_name> <box_name>\n");
    return 0;
}
