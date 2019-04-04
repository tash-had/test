#include "common.h"
#include "utils.h"
#include "path_utils.h"

int main(int argc, char **argv) {
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <image file name> <path to source file> <path to dest>\n", argv[0]);
        exit(1);
    }
    init_disk(argv[1]);




    char *src_path = argv[2];
    char *dest_path = argv[2];

    PathData_t *src = split_path(src_path, NULL);
    PathData_t *dst = split_path(dest_path, NULL);



}