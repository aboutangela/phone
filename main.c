#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

int main(){
    int key = ftok(".", 'b');
    int sem = semget(key, 1, 0666);

    int shmid = shmget(key, 1000, 0666);
    char * line_buf = shmat(shmid, (void *) 0, 0);

    int f = open("story", O_APPEND, 0666);

    while(1){
        struct sembuf sb;
        sb.sem_op = -1;
        sb.sem_num = 0;
        sb.sem_flg = 0;

        printf("Checking resource availibility...");
        semop(sem, &sb, 1);
        printf("resources availible!\n");

        fgets(line_buf, 1024, stdin);
        write(f, line_buf, 1024);

        sb.sem_op = 1;
        semop(sem, &sb, 1);
    }
    close(f);
    return 0;
}
