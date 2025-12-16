#include "task4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int task4(int argc, char *argv[]) {
    if (argc < 3 ) {
        fprintf(stderr, "Err...");
        return 1;
    }
    const char* fileName = argv[1];
    const char* flag = argv[2];
    FILE* f;
    if(!strcmp(flag,"xor8")) {
        f = fopen(fileName,"rb");
        if(f == NULL) {
            fprintf(stderr, "Err...");
            return 1;
        }
        unsigned char result = 0;
        unsigned char buf = 0;
        while(fread(&buf,sizeof(unsigned char),1,f)) {
            result ^= buf;
        }
        printf("%02X\n", result);
        fclose(f);
    } else if(strcmp(flag, "xorodd") == 0) {
        f = fopen(fileName,"rb");
        if (f == NULL) {
            fprintf(stderr,"Err...");
            return 1;
        }
        unsigned char buf[4];
        unsigned char result[4] = {0};
        int filled = 0;
        while (fread(&buf[filled], sizeof(unsigned char), 1, f) == 1) {
            filled++;
            if (filled == 4) {
                int has_prime = 0;
                for (int i = 0; i < 4; ++i)
                    if (is_prime(buf[i])){
                        has_prime = 1;
                        break;
                    }

                if (has_prime) {
                    for (int i = 0; i < 4; ++i)
                        result[i] ^= buf[i];
                }
                buf[0] = buf[1];
                buf[1] = buf[2];
                buf[2] = buf[3];
                filled = 3;
            }
        }
        for (int i = 0; i < 4; ++i)
            printf("%02X ", result[i]);
        printf("\n");
        fclose(f);
    } else if(strcmp(flag, "mask") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: program <file> mask <hex>\n");
            return 1;
        }
        f = fopen(fileName, "rb");
        if (f == NULL) {
            fprintf(stderr,"Err...");
            return 1;
        }
        int count = 0;
        uint32_t value;
        uint32_t mask = (uint32_t)strtoul(argv[3], NULL, 16);
        unsigned char buf[4];
        while (fread(buf,sizeof(unsigned char), 4, f) == 4) {
            value = ((uint32_t)buf[0]) | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
            if ((value & mask) == mask) count++;
        }
        fclose(f);
        printf("Кол-во четырехбайтовых целых чисел соответствующих маске %s : %d ", argv[3], count);
    } else {
        fprintf(stderr, "Err...");
        return 1;
    }
    return 0;
}

int is_prime(unsigned char num) {
    if (num < 2) return 0;
    for (int i = 2; i*i <= num; ++i) {
        if (num % i == 0) return 0;
    }
    return 1;
}