#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {

    int task = 0;
    printf("Select task from 1 to 4: "); scanf("%d",&task);

    if (task == 1) {
        int pid;
        pid = fork();
        printf("%d\n", pid);
        return 0;
    }else if (task == 2) {
        fork();
        printf("Hi\n");
        fork();
        printf("Hi\n"); //В общем создается 4 процесса
        return 0;
    }else if (task == 3) {
        if (fork() || fork()) fork();
        printf("forked! %d", fork());
        return 0;
    }else if (task == 4) {
        if (fork() && (!fork()))
        {
            if (fork() || fork()) fork();
        }
        printf("52\n");
        return 0;
    }else {
        fprintf(stderr,"Incorrect task number\n");
        return 1;
    }

    return 0;
}