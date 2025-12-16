#include "task1.h"
#include <stdio.h>

int createFile(const char* fileName) {
    const unsigned char data[] = {3,1,4,1,5,9,2,6,5,3,5};
    const size_t size = sizeof(data) / sizeof(data[0]);

    FILE* f = fopen(fileName, "wb");
    if (!f) {
        perror("Open file error");
        return 1;
    }
    fwrite(data,sizeof(unsigned char),size,f);
    fclose(f);
    printf("File is created\n");
    return 0;
}

void task1(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Use : %s <path_file>\n", argv[0]);
        return ;
    }
    const char* fileName = argv[1];
    const int result = createFile(fileName);

    if (result != 0) {
        printf("Error: failed to create file %s\n", fileName);
        return ;
    }

    FILE* f = fopen(fileName,"rb");
    if (!f) {
        perror("Open file error");
        return ;
    }
    unsigned char byte;
    while (fread(&byte,sizeof(unsigned char),1,f)) {
        printf("%u : %p, %d, %p, %d, %d, %d, %d, %p\n",
            byte,f->_ptr,f->_cnt,f->_base,f->_flag,f->_file,f->_charbuf,f->_bufsiz,f->_tmpfname);
    }
    printf("\n");
    fclose(f);

    unsigned char data[4];
    f = fopen(fileName,"rb");
    fseek(f,3,SEEK_SET);
    fread(data,sizeof(unsigned char),4,f);
    for (int i = 0; i < 4; ++i) {
        printf("%u ",data[i]);
    }
    fclose(f);
}