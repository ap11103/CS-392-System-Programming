#include "insertion.h"

//Alisha Patel
//I pledge my honor that I have abided by the Stevens Honor System. 

/*
	We emphasize this: because this is a generic implementation, in insertion.c you must not use if-else to
	discriminate different data types in your code. Your code must be able to deal with any data type possible.
	Therefore, we limit the usage of the following primitive types:
	- int or size_t: only allowed as loop iterators;
	- char or u_int8_t: can be used as you wish;
	- Types that are used in the declarations of iSort() and iPrint(): feel free to use them for their _original_
      purpose;
	- All other types are not allowed to appear in insertion.c: no exceptions, no matter if you actually used
	  them, or what purpose it is. Once it appears, you will receive 30% reduction of the homework.

	You are free to create helper functions only in insertion.c; you must declare and implement them in the
    .c file instead of the header files. Do not change any of the header files.	
*/

//swap function based on the lab done in class
void swap(void* pa, void* pb, size_t width){
    //cast to char pointer -> to get the byte
    char* _pa = (char*)pa;
    char* _pb = (char*)pb;
    char temp;
    for (size_t i=0; i < width; i++){
       temp = _pa[i];
       _pa[i] = _pb[i];
       _pb[i] = temp;
	}

	
}
	


void iSort(void* base, size_t nel, size_t width, int (*compare)(void*,void*)) {
	
//to iterate through the list of elements
//width to offset for pointer arithmetic	 
	for(size_t i = width; i < nel*width; i+=width){
		size_t j = i; //iteratinng variable for while loop below

		//if the first element is smaller; perform a swap 
		while (j > 0 && (compare(base+(j-4), base+j) > 0)){
			swap(base+j, base+(j-4), width);
			j-=width;
		}
	}

	
}


void iPrint(void* base, size_t nel, size_t width, void (*print)(void*)) {
	
	//references print and then add width * i 
	//for pointer arithmetic to get each element	
	for(int i = 0; i < nel; i++){
		(*print)(base + (width*i));
		
	}
	
	
}

