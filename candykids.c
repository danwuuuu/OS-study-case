#include "bbuff.h" 
#include "stats.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
//tow struc from 
_Bool stopfac=0;
int random;
typedef struct {
 int factory_number;
 double time_stamp_in_ms;
} candy_t;

double timeInMS(void) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME,&now);
  return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

//factory candy 
void * producing(void * facnumber) {
int * f_num= (int*)facnumber;
random=0;
while (!stopfac) {
random = rand()%4;//generate num 0,1,2,3 

//print fac # and the sates 
printf("	");
printf("Factory %d ship candy & wait %ds\n", *f_num,random);  
    
candy_t *candyy = malloc(sizeof(candy_t));
candyy->time_stamp_in_ms= timeInMS();
candyy->factory_number = *f_num; 
bbuff_blocking_insert(candyy);
stats_record_produced(*f_num);
//sleep
sleep(random);
}
//prin when done
printf("Candy-factory %d done\n", *f_num);
free(facnumber);  return NULL;
}
//kid 
void * kid(void * input) {
random=0;
while (1) {
candy_t* candey = (candy_t*) bbuff_blocking_extract();
int facnum=candey->factory_number;
stats_record_consumed(facnum,timeInMS()-candey->time_stamp_in_ms);
free(candey);
random = rand()%1;//exactly one 
sleep(random);
}
return NULL;
}
//////////////////////////////main/////////////////////////////////////////////////////////////////////
int main (int argc, char **argv) {

int index=0;
int NUM_FACTORIES = 0;
int NUM_KIDS = 0;
int NUM_SECONDS = 0;
time_t begin = time(0);

srand(time(NULL));// should only be called once

///////check validaty of input//////////////////////////
if(argc!=4)
{
 printf("%s \n", "Error:not three input entered. Please enter three positve numbers");//error mes
 exit(-1);
}
  
for (int index = 1; index<argc; index++) {
char *last;
int argAsInt = strtol(argv[index],&last,10);
    if(argAsInt<=0)
	{
	printf("%s \n", "Error:nonpositive input. Please enter three positive numbers");//error mes
	exit(-1);
	}
}

//make sure all the input are valid
//cin value into varables 
NUM_FACTORIES = atoi(argv[1]);
NUM_KIDS=atoi(argv[2]);
NUM_SECONDS =atoi(argv[3]);

////////////////////////////////////start simmulation now///////////////////////////////

//initialize 
bbuff_init();
stats_init(NUM_FACTORIES);
 //luanch threads 
pthread_t * fac_id;
//lanch facthread
index=0;
static pthread_t * f_tid;
f_tid = malloc(sizeof(pthread_t)*NUM_FACTORIES); 
while(index<NUM_FACTORIES){
int *factory_number= malloc(sizeof(int)); 
*factory_number = index;
pthread_create(&f_tid[index], NULL, producing, factory_number);
index++;
}
fac_id = f_tid;
//end


pthread_t * kid_id;
//lanch kidthread
index=0;
static pthread_t *k_tid;
k_tid = malloc(sizeof(pthread_t)*NUM_KIDS); 

while(index<NUM_KIDS){
pthread_create(&k_tid[index], NULL, kid, NULL);
index++;
}
kid_id = k_tid;
//end


while (difftime(time(0), begin)<NUM_SECONDS) {
sleep(1);// #1 by requirement 
int past = (int) difftime(time(0), begin);
past=past-1;
printf("Time %ds: \n", past);
}


printf("Stoping candy factories...\n");
//is stopping candy making
stopfac = 1;
index=0;
while(index<NUM_FACTORIES){
pthread_join(fac_id[index],NULL);
index++;
}

///Waiting for all candy to be consumed
while (!bbuff_is_empty()) {
printf("%s \n","Waiting for all candy to be consumed");
sleep(1);
}

//stop kids
printf("Stoping kids.\n");
for (int i = 0; i<NUM_KIDS; i++) {
pthread_cancel(kid_id[i]);
pthread_join(kid_id[i],NULL);
}


//STATS
printf("Statistics:\n");
stats_display();
  
// free all malloc memory 
free(fac_id);
free(kid_id);
stats_cleanup();

return 0;
}
