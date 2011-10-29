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
#include <pcreposix.h>
#include "PythonDeserialize.h"

void parseFileName(const char* inpStr, char* fileName) {
	char *pattern = "(\\w+)\\.ring\\.gz";
	regmatch_t p[20];
	regex_t f;
	if (regcomp(&f, pattern, 0))
	{
		printf("Error in pattern! %s\n", pattern);
		exit(1);
	}
	if (regexec(&f, inpStr, 20, p, 0)) {
		printf("Invalid file name!\n");
		exit(1);
	}

	int i,j;
	for (i=0,j = p[1].rm_so; j < p[1].rm_eo; j++, i++)
		fileName[i]=inpStr[j];
	fileName[i+1]='\0';
}

int main(int argc, char *argv[]) {
	Py_Initialize();
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

	char name[256];
	parseFileName(fileName,name);

	char newName[1024];

	strcpy(newName,name);
	strcat(newName,".tpl");

	Ring rng = deserializeRing(fileName);
	save(newName, &rng);
	destroy(&rng);
	Py_Finalize();
	return 0;
}

