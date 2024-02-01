#include "utils.h"
#include <string.h>
#include <stdlib.h>




//Alisha Patel
//I pledge my honor that I have abided by the Stevens Honor System. 

/*
	You are free to use any data type you like in this file.
    However, other than the five functions declared in "utils.h",
    DO NOT create any other functions.

	
*/

int cmpr_int(void* x, void* y) {
//based on comparison return 0, 1, -1 to be in insertion sort
	if (*(int*) x == *(int*) y){
		return 0;
	}

	if (*(int*) x > *(int*) y){
		return 1;
	}

	if (*(int*) x < *(int*) y){
		return -1;
	}

}

int cmpr_float(void* x,void* y) {
//based on comparison return 0, 1, -1 to be in insertion sort	
	if (*(float*) x == *(float*) y){
		return 0;
	}	

	if (*(float*) x > *(float*) y){
		return 1;
	}

	if (*(float*) x < *(float*) y){
		return -1;
	}
}

void print_int(void* x) {
//prints each elemnent of void pointer in integer	
	printf("%d\n", *(int*)x);
}

void print_float(void* x) {
//prints each element in float 	
	printf("%f\n", *(float*)x);
}


void* read_array(char* filename, char* format, size_t* len) {
//assignes the file pointer	
	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
        //if the file reads null; outputs an error
        fprintf(stderr, "File failed to open.\n");
        exit(1);
    }

	//allocates space temporarily to count len in next while loop
	void* arr = malloc(sizeof(int));
	//void* arr2 = malloc(sizeof(int));
	
	
	//to increment len to allocate space for real 
	while(fscanf(fp, format, arr) != -1){
		(*len)++;
	}

	//*len  = i;
	//index offset
	int a = 0;

	//compares iif the format is integer and allocates space in specified type
	if (strcmp(format, "%d") == 0){
		arr = malloc((*len) * sizeof(int));
		//goes back to the base address to get each element
		rewind(fp);

		//add index to arr to format the pointers correctly
		while(fscanf(fp, format, arr + a) != -1){
			a+=sizeof(int);
		}
	}

	if (strcmp(format, "%f") == 0){
		arr = malloc(*len * sizeof(float));
		rewind(fp);

		while(fscanf(fp, format, arr + a) != -1){
			a+=sizeof(float);
		}
	}

	//closes the file pointer
	fclose(fp);
	//free(arr2);
	return arr;
}

