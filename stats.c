#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
int facnum = 0;
int j=0;
//create a struc to hold those much value for each factory 
//apply this struct in the 2 d arry 
typedef struct {
int made; 
int eaten;//which will = to made in the end
//these 3 for the last data illustrating 
double maxdelay;
double mindelay;
double totoaldelay;//for calculating the avg in the end 
} Factory;
////////////////////////////////stats_init
Factory ** factory_arr;//2d arry 
void stats_init(int numFactories) {
//need the dynamic meorery to hold the values
factory_arr = malloc((numFactories) * sizeof(Factory));
//2d part
j=0;
while( j<numFactories){// initialize 
Factory * factory = malloc(sizeof(Factory));
factory_arr[j] = factory;
factory->made=0;// initialize 
factory->eaten=0;// initialize 
factory->maxdelay=0.00;// initialize 
factory->mindelay=0.00;// initialize 
factory->totoaldelay=0.00;// initialize 
j++;
}

facnum = numFactories;
}

////////////////////////////stats_record_consumed
void stats_record_consumed(int factoryNumber, double delayInMS) {
Factory * fac = factory_arr[factoryNumber];
fac->eaten=fac->eaten+1;//consume one
//update the delay time 
double min=fac->mindelay;
double max=fac->maxdelay;
if (min==0.0||delayInMS<min)
{fac->mindelay=delayInMS;}
if (max==0.0||delayInMS>max) 
{fac->maxdelay=delayInMS;}
//update total delay time for the last avg calculation 
fac->totoaldelay=fac->totoaldelay+delayInMS;
}
////////////////////////stats_record_produced
void stats_record_produced(int factoryNumber) {
//make one, add it up
Factory * factory = factory_arr[factoryNumber];
factory->made=factory->made+1;//make one 
}
///////////////////stats_display
void stats_display() {
//tile for table
printf("%8s%10s%10s%15s%15s%15s\n","Factory #", "#Made","#Eaten","Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");
//data print out
for(int i = 0; i<facnum; i++) {
Factory * fac = factory_arr[i];
printf("%8d%8d%8d%15.5f%15.5f%15.5f\n", i, fac->made, fac->eaten, fac->mindelay, fac->totoaldelay / fac->eaten,fac->maxdelay);}
}



////////////////////////stats_cleanup
void stats_cleanup(void) {
//free memory 
//free 2d first
for(int i = 0; i<facnum; i++){
free(factory_arr[i]);}
//free head arr
free(factory_arr);
}

