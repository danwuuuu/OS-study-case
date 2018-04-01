#include "bbuff.h"

sem_t mutex;
sem_t full;
sem_t empty;
int itemNumber = 0;
void * BUFFER[BUFFER_SIZE];

_Bool bbuff_is_empty(void) {
//wait till other finish
sem_wait(&mutex);

_Bool reslt;
if(itemNumber==0)
{reslt=1;}
else  {reslt=0;}

sem_post(&mutex);
return reslt;
}

void bbuff_init(void) {
sem_init(&mutex,1,1);
sem_init(&full,1,0);
sem_init(&empty,1,BUFFER_SIZE);
}

void bbuff_blocking_insert(void* item) {
// wait for space avaliable 
sem_wait(&empty); 
//wait for cs
sem_wait(&mutex);
//cs
BUFFER[itemNumber] = item;
itemNumber = itemNumber+1;
//cs end
//unlock
sem_post(&mutex);
sem_post(&full);
}

void* bbuff_blocking_extract(void) {
//wait
sem_wait(&full); 
//wait for cs 
sem_wait(&mutex); 

//cs starts
itemNumber = itemNumber-1;
void* result = BUFFER[itemNumber];
// cs end

//release 
sem_post(&mutex);
sem_post(&empty);
return result;
}

