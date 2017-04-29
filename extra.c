/*
 Constantinos Kavadias, ckavadias@student.unimelb.edu.au
 STUDENT ID 664790, Assignment 2, extra.c module, COMP20007
 
 >> warnings will appear at compilation in regards to passing unsigned
 	char to char variables, this is only done in instances where numeric 
 	comparison or data copying is required and so the warnings can be ignored
*/
#include "extra.h"
#include "hash.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 0
#define MAXSTRLEN 256
#define ASCII 127  //ensures extended series isnt utilised as they may not 
                  // render well on some devices

/*randoms array as described in universal hash in hash.c module */
extern unsigned int randoms[MAXSTRLEN];

/*Extra functions, further definitions below */
void free_array(unsigned char** array, int size);
void debug(char* message, unsigned int num);
bool is_prime(unsigned int n);
unsigned int next_prime(unsigned int n);
void print_output(int Num_randoms, int n, unsigned char** solutions);
void check_count(int count, int n, char* function);

/*print debug message with option to include an int value, when debugging on*/
void debug(char* message, unsigned int num){
	if (DEBUG){
		
		/*if both integer and string output required*/
		if (num){
			fprintf(stderr, "%s %u\n", message, num);
			return;
		}
		
		/*string only ouput*/
		fprintf(stderr, "%s\n" , message);
	}
}

/* function to determine if prime, adapted from 
http://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/isprime.c */
bool is_prime(unsigned int n){
	unsigned int divisor;
	
	/*try and find a divisor for the candidate number*/
	for (divisor = 2; divisor*divisor <= n; divisor++){
		if (n%divisor == 0){
			return false;
		}
	}
	return true;
}

/* test candidates for primes and return first prime encountered */
unsigned int next_prime(unsigned int n){
	
	/* assuming input size always greater than 0*/
	while(n){
		if(is_prime(n)){
			return n;
		}
		n++;
		debug("Next candidate: ", n);
	}
	fprintf(stderr, "Fatal error, prime not found in acceptable interval");
	exit(EXIT_FAILURE);
}

/*print out put for collide_dumb and collide_clever*/
void print_output(int Num_randoms, int n, unsigned char** solutions){
	int i;
	
	printf("%d\n", Num_randoms);
	for ( i = 0; i < Num_randoms; i++){
		printf("%u\n", randoms[i]);
	}
	for ( i = 0; i < n; i++){
		printf("%s\n", solutions[i]);
	}
}

/*free an array of arrays size elements*/
void free_array(unsigned char** array, int size){
	int i;
	
	for(i = 0; i < size; i++){
		free(array[i]);
		debug("Freed entry number: ", i);
	}
	free(array);
}

/*check equivalence of count and int, print error if not equal */
void check_count(int count, int n, char* function){
	if(count != n){
		fprintf(stderr, "\n%s bad termination %d solutions recorded, %d wanted",
			function, count, n);
		exit(EXIT_FAILURE);
	}
	return;
}

/* Determine appropriate size of a hash table given input size n */
unsigned int determine_size(unsigned int n) {
    return next_prime(2*n) ;
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_dumb(unsigned int size, unsigned int seed, int n) {
	int Num_randoms = 0, count = 0, i, last;
	unsigned char *test = NULL, **solutions = NULL;
	
	(void)seed;
	
	test = (unsigned char*)malloc(sizeof(unsigned char)*(MAXSTRLEN + 1));
	solutions  = (unsigned char**)malloc(sizeof(unsigned char*)*n);
	
	debug("Number of solutions wanted: ", n);
	debug("Size is: ", size);
	
	while ( count < n && Num_randoms <= MAXSTRLEN){
		Num_randoms++;
		
		debug("_dumb loop call: ", Num_randoms);
		
		/*test all linguistic character of ASCII avoiding the extended series
		  if they make universal_hash go to 0, record as a solution
		  if not enough one character solutions are found, build upon the 
		  existing ones to create multi character solutions*/
		for( i = 33; i < ASCII && count < n; i++){
			test = (unsigned char*)malloc(sizeof(unsigned char)*
				(Num_randoms + 1));
			
			if( Num_randoms > 1 ){
				strcpy(test, solutions[last]);
			}
			test[Num_randoms - 1] = i;
			test[Num_randoms] = '\0';
			
			if(!universal_hash(test, size)){
				debug("SUCCESS IS: ", i);
				
				solutions[count] = (unsigned char*)malloc(sizeof(unsigned char)*
				(Num_randoms + 1));
				strcpy(solutions[count], test);
				
				debug(solutions[count], count + 1);
			
				count++;
			}
		}
	last = count - 1;
	}
	check_count(count, n, "_collide_dumb");
	print_output(Num_randoms, n, solutions);
	free_array(solutions, n);
	free(test);
}
		
/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_clever(unsigned int size, unsigned int seed, int n) {
	unsigned int integer, Num_randoms = 0;
	int count = 0, last = 0;
	unsigned char **solutions = NULL;
	
	(void)seed;
	
	solutions  = (unsigned char**)malloc(sizeof(unsigned char*)*n);
	
	debug("Number of solutions wanted: ", n);
	debug("Size is: ", size);
	
	/* works on principle that any integer multiple of size will hash to 0
	   as the elements of randoms are always integers as long as the character
	   is an integer multiple of size, the whole expression will be and so will
	   hash to 0*/
	while ( count < n && Num_randoms <= MAXSTRLEN){
		Num_randoms++;
		
		debug("_clever loop call: ", Num_randoms);
		
		/*create multiples of size, record as solutions, excluding extended
			ASCII series*/
		for( integer = 1; integer*size < ASCII && count < n; integer++){
			solutions[count] = (unsigned char*)malloc(sizeof(unsigned char)*
				(Num_randoms + 1));
			
			/*ensure only linguistic characters used and not 0-32 series*/
			if(integer*size > 32){	
				
			/* in event not enough solutions found in first iteration, 
			   need to increase number of characters in string, to avoid rework
			   copy a string one less than the current length and add a new 
			   character multiple to it to ensure it satisfies all previous
			   factors in randoms*/
			   if( Num_randoms > 1 ){
			   	   strcpy(solutions[count], solutions[last]);
			   }
			   solutions[count][Num_randoms - 1] = integer*size;
			
			   /*check that assertion is correct, abort if not*/
			   if(universal_hash(solutions[count], size)){
			   	   fprintf(stderr, "\n_collide_clever, assertion breached");
					exit(EXIT_FAILURE);
			   }
			   solutions[count][Num_randoms] = '\0';
			
			   debug("SUCCESS IS: ", integer*size);
			   debug(solutions[count], count + 1);
			
			   count++;
			}
		}
	last = count - 1;
	}
	check_count(count, n, "_collide_clever");
	print_output(Num_randoms, n, solutions);
	free_array(solutions, n);
}
