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

    int f = open("story", O_WRONLY | O_APPEND, 0666);
	
    struct sembuf sb;
    sb.sem_op = -1;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    semop(sem,&sb,1);

    int size = line_buf[0];
    if (!size)
      printf("start the story! \n");
    else{
      char * last = &line_buf[1];
      printf("last line: %s\n",last);
      printf("continue the story! \n");
    }

    char input[127];
    fgets(input,127,stdin);

    int i = 0;
    while(input[i])
      i++;
    size = i;

    line_buf[0] = size; 
    strcpy(&line_buf[1],input);
    
    write(f, line_buf, 1024);
    shmdt(line_buf);
    
    sb.sem_op = 1;
    semop(sem, &sb, 1);
    close(f);
    return 0;
}

