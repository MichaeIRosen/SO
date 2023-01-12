#include "logging.h"
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

void send_msg(int tx, char const *str) {
    size_t len = strlen(str);
    size_t written = 0;

    while (written < len) {
        ssize_t ret = write(tx, str + written, len - written);
        if (ret < 0) {
            fprintf(stderr, "[ERR]: write failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        written += (size_t) ret;
    }
}

void send_registration(int fx, char* named_pipe, char* box_name) {
    char* reg = "";
    sprintf(reg, "%s %s %s\n","1", named_pipe, box_name);

    send_msg(fx, reg);
}

int main(int argc, char **argv) { // [./pub, pub, <reg_pipe>, <pipename>, <box_name>]
    if (argc < 2) {
        fprintf(stderr, "[ERR]: action not specified\n");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(argv[1], "pub") != 0){
        fprintf(stderr, "[ERR]: not an action of publisher\n");
        return -1;
    }

    // Remove pipe if it does not exist
    if (unlink(argv[3]) != 0 && errno != ENOENT) {
        fprintf(stderr, "[ERR]: unlink(%s) failed: %s\n", argv[3],
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Create pipe
    if (mkfifo(argv[3], 0640) != 0) {
        fprintf(stderr, "[ERR]: mkfifo failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Open pipe for writing
    // This waits for someone to open it for reading
    int tx = open(argv[3], O_WRONLY);
    if (tx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char* message = "";
    if(scanf("%s", message) < 0){
        fprintf(stderr, "[ERR]: failed to read message\n");
        exit(EXIT_FAILURE);
    }

    send_msg(tx, message);

    close(tx);
    

    int fx = open(argv[2], O_WRONLY);
    if (fx == -1) {
        fprintf(stderr, "[ERR]: open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

// [ code = 1, client_named_pipe_path (char[256]), box_name (char[32]) ]

// "1 pipe_name box_name"

    send_registration(fx, argv[3], argv[4]);

    close(fx);
    return 0;
}
