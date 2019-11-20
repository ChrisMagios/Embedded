#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <fcntl.h>
#include <string.h>

typedef unsigned char byte ;
FILE    *fp;

char* path = "/dev/leds";

int main(int argc, char *argv[]) {
	int fds = open(path, O_WRONLY);
	int size = 3;
	char inputBuffer[size];
	char ergBuffer[9];



	fp = stdin;
	while(input_line( fp, inputBuffer, size ) != NULL) {
		printf("Input from User: %s \n", inputBuffer );

		byte firstLed = 0;
		byte secondLed = 0;
		byte thirdLed = 0;
		byte fourthLed = 0;
		byte erg = 0;

		firstLed = inputBuffer[0] - '0';
		secondLed = inputBuffer[1] - '0';
		printf("First %d, Second %d\n", firstLed, secondLed);
		// XOR first and second
		thirdLed = firstLed ^ secondLed;

		// OR first and second
		fourthLed = firstLed | secondLed;

		// Calc erg
		erg = fourthLed << 3 | thirdLed << 2 | secondLed << 1 | firstLed;

		printf("%s \n", itoa(erg, ergBuffer, 2));

		int size_written = write(fds, &erg, 1);
		if( size_written != sizeof( erg ) ) {
			perror( "Error writing: " + *path );
			return EXIT_FAILURE;
		}
	}




	return EXIT_SUCCESS;
}
