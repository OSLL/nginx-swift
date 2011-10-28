/*!
 * main.cpp
 *
 *  Created on: 14.10.2011
 *      Author: dmitriy
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include "PythonDeserialize.h"

int main(int argc, char *argv[]) {
	char fileName[1024];

	if (argc < 2) {
		printf("Format: swift-ring-converter -i file\n");
		return 1;
	}

	int rez = 0;
	opterr = 0;
	while ((rez = getopt(argc, argv, "i:")) != -1) {
		switch (rez) {
		case 'i':
			strcpy(fileName, optarg);
			break;
		case '?':
			printf("Error found !\n");
			break;
		};
	};

	Ring rng = deserializeRing(fileName);
	save("ring.tpl", &rng);
	Ring rng2 = load("ring.tpl");
	save("ring2.tpl", &rng2);


	if(!equal(&rng,&rng2))
		printf("ring is not equal\n");

	destroy(&rng);
	destroy(&rng2);
	return 0;
}

