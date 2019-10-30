#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/neutrino.h>


struct _clockperiod clockperiodNew, clockperiodOld;

void changeSystemTick(unsigned int microsecs){
	struct timespec res;

	// Berechnen des neuen Taktes
	clockperiodNew.nsec = microsecs * 1000000;
	clockperiodNew.fract = 0;

	// Ausgeben des alten Taktes
	int retgetRes = clock_getres(CLOCK_REALTIME, &res);
	if (retgetRes == -1) {
				perror("clock Res old");
				exit(-1);
			}
	printf("Old system period: %ld \n", res.tv_nsec);

	// Änderen des System Taktes
	int retPer = ClockPeriod(CLOCK_REALTIME, &clockperiodNew, &clockperiodOld, 0);
	if (retPer == -1) {
					perror("clock ret Per");
					exit(-1);
				}

	// Ausgeben des neuen Taktes
	retgetRes = clock_getres(CLOCK_REALTIME, &res);
	if (retgetRes == -1) {
					perror("clock Res new");
					exit(-1);
				}
	printf("New system period: %ld \n", res.tv_nsec);

}
int main(int argc, char *argv[]) {

	struct timespec start, stop;

	//changeSystemTick(1);

	// Stoppen des Start Zeitpunktes
	int retStart = clock_gettime(CLOCK_REALTIME, &start);
	if (retStart == -1) {
		perror("clock gettime Start");
		exit(-1);
	}

	int i;
	for (i = 0; i < 1000; ++i) {

		start.tv_nsec += 1000000L;
		start.tv_nsec = start.tv_nsec % 1000000000;
		if ( start.tv_nsec == 0) {
			++start.tv_sec;
		}


		int retNano = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &start, NULL);
		if (retNano != 0) {
			printf("Error Nano: %s \n", strerror(retNano));
			exit(-1);
		}
	}

	// Stoppen des End Zeitpunktes
	int retStop = clock_gettime(CLOCK_REALTIME, &stop);
			if (retStop == -1) {
				perror("clock gettime Stop");
				exit(-1);
			}

	if ((int)(stop.tv_nsec  - start.tv_nsec) != 0) {
			printf("Something wrong at: %d with start: %ld, and stop: %ld \n", i, start.tv_nsec, stop.tv_nsec);
			exit(-1);
			}
	printf("Success!\n");
	return EXIT_SUCCESS;
}
