#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <string.h>
#include<unistd.h>


sem_t sem;
struct timespec start, stop;
struct sched_param schedParams;
pthread_t thread1, thread2;
pthread_attr_t attr1, attr2;
struct sched_param schedParams;

int tacts = 0;
int task1ToWaste = 2;
int task2ToWaste = 3;

void waste_msecs (unsigned int msecs){
	int counter = msecs;
	long int k =0;
	long int i = 0;

	for (k = msecs; k != 0; k--){
		for (i = 0; i < 66500; i++){
			counter  = counter * counter;
		}
	}
}
/*
 * Function for thread 1
 */
void* second(void* args) {

	int msec = *(int*) args;

	// Waste time after semapore
	while (1 == 1) {
		int semWait = sem_wait(&sem);
		if (semWait < 0) {
			perror("wait Semaphore");
			exit(-1);
		}
		waste_msecs(msec);
	//	printf ("Waste time TASK\n");

	}


	return NULL;
}

void* first(void* args){
	int ms = *(int*) args;
	while (1 == 1){
	//	printf ("Waste time MAIN\n");
		++tacts;
		waste_msecs(ms);

		int retStart = clock_gettime(CLOCK_REALTIME, &start);
		if (retStart == -1) {
			perror("clock gettime Start");
			exit(-1);
		}

		start.tv_nsec += 2000000L;
		start.tv_nsec = start.tv_nsec % 1000000000;
		if ( start.tv_nsec == 0) {
			++start.tv_sec;
		}
		int retNano = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &start, NULL);
			if (retNano != 0) {
				printf("Error Nano: %s \n", strerror(retNano));
				exit(-1);
		}

		if (tacts == 3) {
			tacts = 0;

			int semPost = sem_post(&sem);
			if (semPost < 0) {
				perror("Post Semaphore");
				exit(-1);
			}
		}
	}
		return NULL;
}

int main(int argc, char *argv[]) {

	// Set thread 1 attributes
	schedParams.sched_priority = 150;
	pthread_attr_init(&attr1);
	int setSched = pthread_attr_setschedparam(&attr1, &schedParams);
	if (setSched != 0 ){
		printf ("pthread_setSched: %s\n", strerror(setSched));
		exit(-1);
	}
	pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr1,PTHREAD_EXPLICIT_SCHED);

	// Set thread 2 attributes
	schedParams.sched_priority = 100;
	pthread_attr_init(&attr2);
	setSched = pthread_attr_setschedparam(&attr2, &schedParams);
	if (setSched != 0 ){
		printf ("pthread_setSched: %s\n", strerror(setSched));
		exit(-1);
	}
	pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr2,PTHREAD_EXPLICIT_SCHED);

	// Init semaphore
	int retSemInit = sem_init(&sem, 0, 0);
	if (retSemInit == -1) {
			perror("Init Semaphore");
			exit(-1);
	}

	// Init Thread
	int retCrea1 = pthread_create(&thread1, &attr1, &first, &task1ToWaste);
	if ( retCrea1 != 0 ){
		printf ("pthread_create1: %s\n", strerror(retCrea1));
		exit(-1);
	}

	retCrea1 = pthread_create(&thread2, &attr2, &second, &task2ToWaste);
	if ( retCrea1 != 0 ){
		printf ("pthread_create2: %s\n", strerror(retCrea1));
		exit(-1);
	}

	int retJoin = pthread_join(thread1, NULL);
	if ( retJoin != 0 ){
		 printf ("pthread_join1: %s\n", strerror(retJoin));
		 exit(-1);
	}
	retJoin = pthread_join(thread2, NULL);
	if ( retJoin != 0 ){
	     printf ("pthread_join2: %s\n", strerror(retJoin));
	     exit(-1);
	}
	return EXIT_SUCCESS;
}


// Helper functions-----------------------------------------


