#include <stdio.h>
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

int main(int argc, char* argv[]) {
    system("chcp 65001");
    int num = 0;
    printf("Enter the task number: "); scanf("%d",&num); while (getchar() != '\n');
    switch (num) {
        case 1:
            task1(argc, argv);
            break;
        case 2:
            task2();
            break;
        case 3:
            task3(argc, argv);
            break;
        case 4:
            /*  Запись в файл байтиков через PowerShell, маска как пример рабочий - 0x00000012
                Массив байтов
                [byte[]]$bytes = 0x12,0xAF,0x34,0x56,0x7E,0x01,0xB3,0xCD,0x08,0x9F,0x44,0x55,0x23,0xE1,0x6A,0xFF

                Запись в бинарный файл
                [IO.File]::WriteAllBytes("C:\All My Storage\Development\Projects\Labs\fileSource4.bin", $bytes)
            */
            task4(argc, argv);
            break;
        default:
            printf("Task wasn't found");
    }

    return 0;
}