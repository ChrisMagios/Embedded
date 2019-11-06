#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>

struct timespec start, stop;
struct sched_param schedParams;
pthread_t thread1;
pthread_attr_t attr;


void waste_msecs (unsigned int msecs){

	int counter = msecs;
	long int k =0;
	long int i = 0;

	for (k = msecs; k != 0; k--){
		for (i = 0; i < 66400; i++){
			counter  = counter * counter;
		}
	}

}

void*  mess_routine( void*  arg) {

	int i;
	int ms = (int) &arg[0];
	pthread_getschedparam(pthread_self(), NULL, &schedParams);
	printf("Thread: %d,runs with prio: %d\n", pthread_self(), schedParams.sched_priority);
	printf("Tested for: %d ms \n", ms);
	for (i = 0; i < 30; i++){
		usleep(100000);

		int retStart = clock_gettime(CLOCK_REALTIME, &start);
		if (retStart == -1) {
			perror("clock gettime Start");
			exit(-1);
		}

		waste_msecs(ms);

		int retStop = clock_gettime(CLOCK_REALTIME, &stop);
		if (retStop == -1) {
			perror("clock gettime Stop");
			exit(-1);
		}
		//Berechnen der Mess Ergebnisse
		int erg = stop.tv_nsec - start.tv_nsec;
		if(erg < 0){
			erg += 1000000000;
		}
		printf("Time in nanosecs: %ld\n",erg);
	}
}
int main(int argc, char *argv[]) {

	int ms = 100;

	// Set thread prio higher
	schedParams.sched_priority = 255;
	//schedParams.sched_curpriority = 100;

	// Set thread attributes
	void* id1;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);


	int setSched = pthread_attr_setschedparam(&attr, &schedParams);
	if (setSched != 0 ){
		printf ("pthread_setSched: %s\n", strerror(setSched));
		exit(-1);
	}


	int retCrea = pthread_create(&thread1, &attr, &mess_routine, ms);
	if ( retCrea != 0 ){
		printf ("pthread_create: %s\n", strerror(retCrea));
		exit(-1);
	}

	int retJoin = pthread_join(thread1, &id1);
	if ( retJoin != 0 ){
	     printf ("pthread_join: %s\n", strerror(retJoin));
	     exit(-1);
	}


	return EXIT_SUCCESS;
}
