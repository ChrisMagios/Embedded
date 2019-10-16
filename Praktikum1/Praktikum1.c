#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void*  function( void*  arg)
{
	sleep((int) &arg[0]);
	printf( "This is thread %d and waits time %d\n", pthread_self(), (int)&arg[0]  );

   return( pthread_self());
}

int main(int argc, char *argv[]) {

	pthread_t thread1;
	pthread_t thread2;
	void* id1;
	void* id2;

	// Set thread attributes
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE );




	// Create Threads
	int retCrea1 = pthread_create(&thread1, &attr, &function, 6);
	int retCrea2 = pthread_create(&thread2, &attr, &function, 3);

	if ( retCrea1 != 0 ){
	     printf ("pthread_create1: %s\n", strerror(retCrea1));
	     exit(-1);
	}
	if ( retCrea2 != 0 ){
		 printf ("pthread_create2: %s\n", strerror(retCrea2));
		 exit(-1);
	}

	//Join threads to this one (main)
	int retJoin1 = pthread_join(thread1, &id1);
	int retJoin2 = pthread_join(thread2, &id2);

	if ( retJoin1 != 0 ){
	     printf ("pthread_join1: %s\n", strerror(retJoin1));
	     exit(-1);
	}
	if ( retJoin2 != 0 ){
	     printf ("pthread_join2: %s\n", strerror(retJoin2));
	     exit(-1);
	}


	if (thread1 != id1) {
		printf("THREAD ID: %d is not the same as: %d !\n", thread1, id1);
	}

	if (thread2 != id2) {
		printf("THREAD ID: %d is not the same as: %d !\n", thread2, id2);
	}



	printf( "ThreadId 1: %d, ThreadId 2: %d\n", thread1, thread2);

	return EXIT_SUCCESS;
}





