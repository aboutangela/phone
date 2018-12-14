#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

union semun{
	int val;
	struct semid_ds * buf;
	unsigned short * array;
};

void create_shared(){
    int f = open("story", O_CREAT | O_TRUNC, 0666);
    int key = ftok(".", 'b');
    int shmid = shmget(key, 1000, IPC_CREAT | IPC_EXCL | 0666);

    int sem = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    union semun val;
    val.val = 1;
    semctl(sem, 0, SETVAL, val);
}

void print_story(){
    int f = open("story", O_RDONLY, 0666);
    char * story = calloc(1, 10000);
    read(f, story, 10000);
    printf("%s\n", story);
		close(f);
}

void remove_shared(){
    int key = ftok(".", 'b');

    int sem = semget(key, 1, 0666);

    struct sembuf sb;
    sb.sem_op = -1;
    sb.sem_num = 0;
    sb.sem_flg = 0;
    semop(sem, &sb, 1);

    semctl(sem, 0, IPC_RMID, 0);

    print_story();
    remove("story");

    int shmid = shmget(key, 1000, 0666);
    shmctl(shmid, IPC_RMID, NULL);
}

int main( int argc, char * argv[] ){
    if(argc == 1){
        printf("Command line flag required\n");
        exit(1);
    }
    if(strcmp(argv[1], "-c") == 0){
        create_shared();
    }
    if(strcmp(argv[1], "-r") == 0){
        remove_shared();
    }
    if(strcmp(argv[1], "-v") == 0){
        print_story();
    }
}

