#include "task3.h"
#include <stdio.h>

void task3(int argc, char* argv[]) {
    const char* fileSource = argv[1];
    const char* fileDest = argv[2];
    char buffer[4096];
    size_t bytes;

    FILE* f1 = fopen(fileSource,"rb");
    FILE* f2 = fopen(fileDest,"wb");
    if (!f1 || !f2) {
        if (!f2) {
            fprintf(stderr, "Its err f2");
        }
        fprintf(stderr,"Open failed file\n");
        return ;
    }

    while ((bytes = fread(buffer,1,sizeof(buffer),f1)) > 0) {
        fwrite(buffer,1,bytes,f2);
    }
    fclose(f1);
    fclose(f2);
}