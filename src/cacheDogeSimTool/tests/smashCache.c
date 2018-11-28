#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define MAXVAL 10000

struct wonk{
  int a;
} *shrdPtr;

pthread_mutex_t lock;

struct wonk *getNewVal(struct wonk**old){
  free(*old);
  *old = NULL;
  struct wonk *newval = (struct wonk*)malloc(sizeof(struct wonk));
  newval->a = 1;
  return newval;
}

void *updaterThread(void *arg){

  int i;
  for(i = 0; i < 10; i++){    
    pthread_mutex_lock(&lock);
    struct wonk *newval = getNewVal(&shrdPtr);
    shrdPtr = newval;
    pthread_mutex_unlock(&lock);
    usleep(10 + (rand() % 100) );
  }

}

void *sleeperThread(void *arg){

  usleep(100);

}


void *accessorThread(void *arg){

  u_int64_t *result = (u_int64_t*)malloc(sizeof(u_int64_t));; 
  *result = 0;

  while(*result < MAXVAL){
    pthread_mutex_lock(&lock);//400a4e
    if(shrdPtr != NULL){
      *result += shrdPtr->a;      
    }
    pthread_mutex_unlock(&lock);
    usleep(1 + (rand() % 3) );
  }

  pthread_exit(result); 
}

int main(int argc, char *argv[]){

  int res = 0;
  shrdPtr = (struct wonk*)malloc(sizeof(struct wonk));
  shrdPtr->a = 1;

  pthread_mutex_init(&lock,NULL);

  pthread_t acc[4];
  pthread_t upd;
  pthread_create(&acc[0],NULL,sleeperThread,(void*)shrdPtr);
  usleep(10);
  pthread_create(&acc[1],NULL,accessorThread,(void*)shrdPtr);
  usleep(10);
  pthread_create(&acc[2],NULL,sleeperThread,(void*)shrdPtr);
  usleep(10);
  pthread_create(&acc[3],NULL,accessorThread,(void*)shrdPtr);
  //usleep(10);
  //pthread_create(&upd,NULL,updaterThread,(void*)shrdPtr);

  usleep(10);

  pthread_join(acc[0],(void*)&res);
  usleep(10);
  pthread_join(acc[1],(void*)&res);
  usleep(10);
  pthread_join(acc[2],(void*)&res);
  usleep(10);
  pthread_join(acc[3],(void*)&res);
  //usleep(10);
  //pthread_join(upd,NULL);
  
  fprintf(stderr,"Final value of res was %d\n",res); 
}