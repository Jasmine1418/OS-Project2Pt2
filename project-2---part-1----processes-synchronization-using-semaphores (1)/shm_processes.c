#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  ClientProcess(int []);

void deposit(int* account);

void withdraw(int* account);


int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
     int fd, i, nloop=5, zero=0,*counter_ptr;
     sem_t *mutex;

     fd = open("log.txt", O_RDWR|O_CREAT,S_IRWXU);
     write(fd,&zero,sizeof(int));
     counter_ptr = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
     close(fd);

     if((mutex = sem_open("examplesaaaebbmaphore", O_CREAT, 0644, 1)) == SEM_FAILED){
       perror("semaphore initialization\n");
       exit(1);
     }

     pid = fork();
     
     if(pid>0){
       for(i=0; i>-1; i++){
         sleep(rand()%6);
         printf("Dear Old Dad: Attempting to Check Balance\n");
         sem_wait(mutex);
         int rnum = rand()%101;
         if(rnum%2 == 0){
           if(*counter_ptr<100){
             deposit(counter_ptr);
           }
           else{
             printf("Dear old Dad: thinks Student has enough Cash ($%d)\n", *counter_ptr);
           }
         }
         else{
           printf("Dear Old Dad: Last checking balance: $%d\n", *counter_ptr);
         }
         sem_post(mutex);
       }
       
       exit(1);
     }
     else if(pid==0){
       for(i=0; i>-1; i++){
         sleep(rand()%6);
         printf("Poor student: Attempting to check balance\n");
         sem_wait(mutex);
         int rnum = rand();
         
         if(rnum%2==0){
           withdraw(counter_ptr);
         }
         else{
           printf("Poor Student: Last Checking balance: $%d\n", *counter_ptr);
         }
         sem_post(mutex);
       }

       printf(" Client is going to exit\n");
       exit(0);
     }

     wait(&status);
     printf("Server has detected the completion of its child\n");
     printf("Server has detached its shared memory... \n");
     printf("Server has removed its shared memory... \n");
     printf("Server has exited... \n");
     exit(0);
}

void deposit(int* account){

  int balance = *account;
  int amt = rand() % 101;
  if(amt%2==0){
    balance += amt;
    printf("Dear old Dad: Deposits $%d; Balance = $%d\n", amt, balance);
    *account = balance;
  }
  else{
    printf("Dear old Dad: Doesn't have money\n");
  }
}

void withdraw(int* account){
  int balance = *account;
  int amount_needed = rand() % 51;
  printf("Poor student needs $%d\n", amount_needed);
  
  if(amount_needed<=balance){
    balance -= amount_needed;
    printf("Poor student: withdraws $%d; Balance = $%d\n", amount_needed, balance);
    *account = balance;
  }
  else{
    printf("Poor Student: Not enough money ($%d)\n", balance);
  }
}

void  ClientProcess(int []);
