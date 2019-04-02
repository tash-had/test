#include "common.h"
#include "utils.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <path>\n", argv[0]);
        exit(1);
    }
    init_disk(argv[1]); 

    char *path = argv[2];
    if (path[0] != '\\'){
        perror('Not absolute path');
        return ENOENT;
    }

    char *curr_token = strtok(path, "\\");
    char *next_token;
    while (curr_token != NULL){
        next_token = strtok(path, "\\");
        curr_token = next_token;
        

    }
}