#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <fcntl.h>
#include <string.h>


#define LEFTEND 500
#define RIGHTEND 2500

enum Signal {
	HIGH ='1',
	LOW = '0',
};

int cycle = 20000000; //us
FILE    *fp;
char* path = "/dev/gpio1/12";
struct timespec start;

void waste_usecs (unsigned int usecs){

	int counter = usecs;
	long int k =0;
	long int i = 0;

	for (k = usecs; k != 0; k--){
		for (i = 0; i < 66; i++){
			counter  = counter * counter;
		}
	}
}

void sendPWMSignal (int signal, int fds) {
	char buffer = signal;

	int size_written = write(fds, &buffer, sizeof( buffer ));
	if( size_written != sizeof( buffer ) ) {
		char errorStr[100];
		sprintf(errorStr,"Error writing: %s", path);
		perror(errorStr);
		return;
	}
}


int main(int argc, char *argv[]) {
	int size = 3;
	char inputBuffer[size];
	fp = stdin;
	int i;
	int remainingCycleTime;
	int signalTime;


	printf("Please Input servo adjustment in percent!\n");

	input_line( fp, inputBuffer, size);
/*
	int third = (inputBuffer[2] - '0') * 100;
	int second = (inputBuffer[1] - '0') * 10;
	int first = (inputBuffer[0] - '0');


	int percentage = third << 3 | second << 2 | first << 1;
	percentage =   third + second + first;
*/
	int percentage = 100;
	printf("Input from User: %d\n", percentage);

	if (percentage > 100) {
		printf("Percantage has to be between 1 - 100!\n");
		return EXIT_FAILURE;
	}




	int retStart = clock_gettime(CLOCK_REALTIME, &start);
	if (retStart == -1) {
		perror("clock gettime Start");
		exit(-1);
	}

	int fds = open(path, O_WRONLY);
	int inc = 1;
	for (i = 1;; i += inc) {
		signalTime = i * 20  + LEFTEND;

		if (signalTime >= RIGHTEND || signalTime <= LEFTEND) {
			printf("Switch at : %d\n", signalTime);
			inc = -inc;
		}

		if ( signalTime > RIGHTEND) {
			printf("Reached right frequency range border!");
			return EXIT_FAILURE;
		}

		sendPWMSignal(HIGH, fds);

		waste_usecs(signalTime);

		start.tv_nsec += cycle;
		if ( start.tv_nsec >= 1000000000) {
			start.tv_nsec = start.tv_nsec % 1000000000;
			++start.tv_sec;
		}

		sendPWMSignal(LOW, fds);

		int retNano = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &start, NULL);
		if (retNano != 0) {
				printf("Error Nano: %s \n", strerror(retNano));
				exit(-1);
		}
	}


	return EXIT_SUCCESS;
}
