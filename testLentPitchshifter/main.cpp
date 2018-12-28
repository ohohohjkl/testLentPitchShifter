/**************  Effects Program  *********************/

#include "LentPitShift.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include<conio.h>
using namespace stk;

void mainProcess(StkFloat shift_period, int numData, char* path);
int check_path(char * path)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, "file no exist!!! \n");
		return 1;
	}
	fclose(fp);
	return 0;
}

void usage() {
	printf(" executable.exe -shift (recommend <0.5) number_of_scaleData .\\folderName\\");
}

int find_args(int argc, char * argv[], char * arg)
{
	int i;
	for (i = 0; i < argc; ++i) {
		if (!argv[i]) continue;
		if (0 == strcmp(argv[i], arg)) {
			return 1;
		}
	}
	return 0;
}

int valid(char *path, char * num) {
	if (path == NULL || num == NULL) {
		printf("DD");
		return 0;
	}
	return 1;
}

void test_r() {
	StkFloat tempf = 0, step, shift_period = 0.3;
	StkFloat from = 1 + shift_period, to = 1 - shift_period;
	time_t t;
	//int size;
	step = (from - to) / 500;

	srand((unsigned)time(&t));
	for (int n = 0; n < 500; n++) {
		printf("%lf\n", (from - (rand() % (500 + 1))*step));
	}

	_getch();
}

void test_path() {
	int len = strlen("D:\\Work_place_game\\data\\");
	char *path = (char*)malloc(sizeof(char)*len);
	strcpy(path, "D:\\Work_place_game\\data\\");
	mainProcess(0.3, 100, path);
	_getch();
}

void test_pitch() {
	/*LentPitShift lshifter;
	StkFloat tempf = 0, step;
	int size;


	lshifter.setPeriod(0.7);
	fscanf(fin, "%d", &size);
	for (int i = 0; i < size; i++) {
		fscanf(fin, "%lf", &tempf);
		lshifter.tick(tempf, fout);
	}

	fclose(fin);
	fclose(fout);
	_getch();*/

	LentPitShift lshifter;
	StkFloat temp;
	int size;
	FILE *fin = fopen("D:\\Work_place_game\\data\\realData\\0_0.txt", "r");
	FILE *fout = fopen("D:\\Work_place_game\\data\\scaleData\\0_0.txt", "w");

	fscanf(fin, "%d", &size);
	lshifter.setPeriod(0.75);
	for (int i = 0; i < size; i++) {
		fscanf(fin, "%lf", &temp);
		StkFloat sample = lshifter.tick(temp, fout);
		//fprintf(fout,"%.9lf\n", sample);

		//printf("%lf - %.9lf\n", temp, sample);


	}
	fclose(fout);
	fclose(fin);
}
int main(int argc, char *argv[])
{
	//Variables
	//test_pitch();
	//_getch();
	//return 1;
	test_path();
	return 1;
	char *path = argv[argc - 1];
	char *numDataString = argv[argc - 2];
	int numData = atoi(numDataString);

	//int shifted = find_args(argc, argv, "-shift");
	StkFloat shift_period = 0.3;

	if (valid(path, numDataString))
	{
		for (int i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "-shift")) {
				shift_period = atoi(argv[++i]);
				break;
			}
		}

		printf("%d - %s", numData, path);
		//Process
		mainProcess(shift_period, numData, path);
		return 1;
	}
	usage();
}

void mainProcess(StkFloat shift_period, int numData, char* tpath) {
	LentPitShift lshifter;
	StkFloat  step;
	StkFloat from = 1 + shift_period, to = 1 - shift_period;
	time_t t;
	//int size;
	step = (from - to) / numData;

	/*srand((unsigned)time(&t));
	for (int n = 0; n < numData; n++) {
	lshifter.setPeriod(from - (rand() % (numData + 1))*step);
	}
	*/

	int  label_cur = 0, i = 0, bound = 0;
	char *label = (char *)malloc(sizeof(char) * 2);

	int size = 0, max_index;
	const char *default_ext = ".txt";

	size_t len_path = strlen(tpath);
	char *path = (char *)malloc(sizeof(char) * (len_path + 11));
	char *path2 = (char *)malloc(sizeof(char) * (len_path + 11));

	char *path_conf = (char *)malloc(sizeof(char) * (len_path + 10 + 11));


	len_path = strlen(path);

	strcpy(path, tpath);
	strcat(path, "realData\\");

	strcpy(path_conf, path);
	strcat(path_conf, "config.txt");

	int dem = 0;

	//load number of classes
	FILE *fconf = fopen(path_conf, "r");
	fscanf(fconf, "%d", &max_index);
	fclose(fconf);

	while (label_cur < (max_index + 1))
	{
		if (label_cur < 10) {
			label = (char *)malloc(sizeof(char));
		}
		else if (label_cur > 9 && label_cur < 100) {
			label = (char *)malloc(sizeof(char) * 2);
		}

		sprintf(label, "%d", label_cur);
		size_t len_path_tmp = strlen(path) + strlen(label) + 1;
		char *temp = (char*)malloc(sizeof(char) * len_path_tmp);
		strcpy(temp, path);
		strcat(temp, label);
		strcat(temp, "_");
		while (true) {
			char *path_file;
			char *index;
			if (i < 10) { index = (char *)malloc(sizeof(char)); }
			else if (i >= 10 && i < 100) {
				index = (char *)malloc(sizeof(char) * 2);
			}
			else {
				index = (char *)malloc(sizeof(char) * 3);
			}
			sprintf(index, "%d", i);
			size_t len = len_path_tmp + strlen(index) + 5;
			path_file = (char *)malloc(len * sizeof(char));
			strcpy(path_file, temp);
			strcat(path_file, index);
			strcat(path_file, default_ext);
			printf("path : %s \n", path_file);
			if (check_path(path_file)) {
				free(path_file);
				bound = 0;
				break;
			}
			else {
				FILE * fout;
				FILE * fin = fopen(path_file, "r");
				fscanf(fin, "%d", &size);
				StkFloat *tempf = (StkFloat*)malloc(sizeof(StkFloat)*size);
				for (int i = 0; i < size; i++) {
					fscanf(fin, "%lf", &tempf[i]);
				}
				fclose(fin);

				char *path_file2;
				char *index2 = (char*)malloc(sizeof(char) * 3);
				srand((unsigned)time(&t));

				for (int n = bound; n < numData + bound; n++) {
					lshifter.setPeriod(from - (rand() % (numData + 1))*step);
					strcpy(path2, tpath);
					strcat(path2, "scaleData\\");
					sprintf(index2, "%d", n);

					size_t len_path_tmp2 = strlen(path2) + strlen(label) + 1;
					char *temp2 = (char*)malloc(sizeof(char) * len_path_tmp);
					strcpy(temp2, path2);
					strcat(temp2, label);
					strcat(temp2, "_");
					size_t len = len_path_tmp + strlen(index) + 5;
					path_file2 = (char *)malloc(len * sizeof(char));
					strcpy(path_file2, temp2);
					strcat(path_file2, index2);
					strcat(path_file2, default_ext);
					printf("path_F : %s \n", path_file2);
					fout = fopen(path_file2, "w");
					fprintf(fout, "%d\n", size);
					for (int i = 0; i < size; i++) {
						lshifter.tick(tempf[i], fout);
					}
					fclose(fout);
				}
				free(tempf);
				bound += 100;
			}
			i++;
			dem++;
		}
		++label_cur;
		i = 0;
	}
}