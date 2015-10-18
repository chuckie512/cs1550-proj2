#include<linux/unistd.h>
#include<stdio.h>
#include<sys/mman.h>
#include<stdlib.h>




//I need this
struct cs1550_sem{
  int val;
  struct llnode *start;
  struct llnode *end;
};


//so I don't have to call the syscall all the time
void down(struct cs1550_sem *sem) {
  syscall(__NR_cs1550_down, sem);
}

//so I don't have to call the syscall all the time
void up(struct cs1550_sem *sem) {
  syscall(__NR_cs1550_up, sem);
}

int main(void){

  //create a mmap to share the sem between processes 
  void * sems = mmap(NULL, sizeof(struct cs1550_sem)*3, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
  
  //set the sem pointers
  struct cs1550_sem *north = ((struct cs1550_sem *)sems) + 0;
  struct cs1550_sem *south = ((struct cs1550_sem *)sems) + 1;
  struct cs1550_sem *mutex = ((struct cs1550_sem *)sems) + 2;

  //init the sems
  north->val  = 0;
  north->start= NULL;
  north->end  = NULL;

  south->val  = 0;
  south->start= NULL;
  south->end  = NULL;

  mutex->val  = 1;
  mutex->start= NULL;
  mutex->end  = NULL;
  

  printf("starting simulation\n\n");
  //time for the producers/ consumers!
  if(fork()==0){//north producer
    printf("NORTH: Producer created\n");
    while(1){
      while((rand()%10)<8){
        up(north);
        printf("NORTH: new car\n");
      }
      printf("NORTH: no more cars, sleeping for 20 seconds\n");
      sleep(20);
    }
  }
  else if(fork()==0){ //south producer
    printf("SOUTH: producer created\n");
    while(1){
      while((rand()%10)<8){
        up(south);
        printf("SOUTH: new car\n");
      }
      printf("SOUTH: no more cars, sleeping for 20 seconds\n");
      sleep(20);
    }
  }
  else if(fork()==0){ //north consumer
    printf("NORTH: consumer created\n");
    while(1){
      down(north);
      down(mutex); //only one car from EITHER direction at a time
      printf("NORTH car allowed through\n");
      up(mutex);
    }
  }
  else{ //south consumer
    printf("SOUTH: consumer created\n");
    while(1){
      down(south);
      down(mutex);
      printf("SOUTH car allowed through\n");
      up(mutex);
    }
  }
  return 0;
}
