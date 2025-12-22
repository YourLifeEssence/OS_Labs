#include "task3.h"
#include <stdio.h>

void task3(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Error\nUsage: program arguments -> <src> <dst>\n");
        return;
    }
    const char* fileSource = argv[1];
    const char* fileDest = argv[2];
    char buffer[4096];
    size_t bytes;

    FILE* f1 = fopen(fileSource,"rb");
    FILE* f2 = fopen(fileDest,"wb");
    if (!f1 || !f2) {
        if (!f1 || !f2) {
            if (f1) fclose(f1);
            if (f2) fclose(f2);
        }
        fprintf(stderr,"Open failed file\n");
        return;
    }

    while ((bytes = fread(buffer, 1, sizeof(buffer), f1)) > 0) {
        size_t written = fwrite(buffer, 1, bytes, f2);
        if (written != bytes) {
            fprintf(stderr, "Write error\n");
            break;
        }
    }

    if (ferror(f1)) {
        fprintf(stderr, "Read error\n");
    }
    fclose(f1);
    fclose(f2);
}